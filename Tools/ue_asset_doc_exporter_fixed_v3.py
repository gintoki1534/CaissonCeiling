#!/usr/bin/env python3
from __future__ import annotations

import argparse
import fnmatch
import json
import os
import re
import subprocess
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, Iterable, List, Optional, Tuple


@dataclass
class UEObject:
    class_name: Optional[str]
    name: Optional[str]
    export_path: Optional[str]
    lines: List[str] = field(default_factory=list)
    children: List["UEObject"] = field(default_factory=list)


# ----------------------------
# Generic parsing helpers
# ----------------------------

def split_top_level(text: str, sep: str = ",") -> List[str]:
    parts: List[str] = []
    current: List[str] = []
    depth = 0
    in_quote = False
    i = 0
    while i < len(text):
        ch = text[i]
        if ch == '"':
            in_quote = not in_quote
            current.append(ch)
        elif not in_quote and ch == "(":
            depth += 1
            current.append(ch)
        elif not in_quote and ch == ")":
            depth = max(0, depth - 1)
            current.append(ch)
        elif not in_quote and depth == 0 and text.startswith(sep, i):
            parts.append("".join(current).strip())
            current = []
            i += len(sep) - 1
        else:
            current.append(ch)
        i += 1
    tail = "".join(current).strip()
    if tail:
        parts.append(tail)
    return parts


def strip_quotes(value: Optional[str]) -> Optional[str]:
    if value is None:
        return None
    value = value.strip()
    if len(value) >= 2 and value[0] == '"' and value[-1] == '"':
        return value[1:-1]
    return value


def parse_header_field(line: str, key: str) -> Optional[str]:
    m = re.search(rf'{re.escape(key)}=("[^"]*"|\S+)', line)
    if not m:
        return None
    return strip_quotes(m.group(1))


def short_object_name(ref: Optional[str]) -> Optional[str]:
    if not ref:
        return None
    ref = ref.strip()
    if ref == "None":
        return None
    m = re.search(r"'([^']+)'", ref)
    if m:
        ref = m.group(1)
    ref = ref.split("/")[-1]
    if ":" in ref and "." not in ref:
        return ref.split(":")[-1]
    if "." in ref:
        return ref.split(".")[-1]
    return ref


def class_display_name(class_name: Optional[str]) -> Optional[str]:
    if not class_name:
        return None
    value = class_name.strip()
    if "'" in value:
        m = re.search(r"'([^']+)'", value)
        if m:
            value = m.group(1)
    value = value.split("/")[-1]
    return value


def class_short_name(class_name: Optional[str]) -> Optional[str]:
    display = class_display_name(class_name)
    if not display:
        return None
    return display.split(".")[-1]


def class_matches(class_name: Optional[str], *candidates: str) -> bool:
    display = class_display_name(class_name)
    short = class_short_name(class_name)
    values = {v for v in (display, short) if v}
    for candidate in candidates:
        if candidate in values:
            return True
        if display and display.endswith(candidate):
            return True
    return False


def parse_object_tree(text: str) -> List[UEObject]:
    roots: List[UEObject] = []
    stack: List[UEObject] = []
    for raw_line in text.splitlines():
        stripped = raw_line.strip()
        if not stripped:
            continue
        if stripped.startswith("Begin Object"):
            obj = UEObject(
                class_name=parse_header_field(stripped, "Class"),
                name=parse_header_field(stripped, "Name"),
                export_path=parse_header_field(stripped, "ExportPath"),
            )
            if stack:
                stack[-1].children.append(obj)
            else:
                roots.append(obj)
            stack.append(obj)
        elif stripped == "End Object":
            if stack:
                stack.pop()
        else:
            if stack:
                stack[-1].lines.append(stripped)
    return roots


def iter_objects(obj: UEObject) -> Iterable[UEObject]:
    yield obj
    for child in obj.children:
        yield from iter_objects(child)


def find_first(iterable: Iterable[UEObject], predicate) -> Optional[UEObject]:
    for item in iterable:
        if predicate(item):
            return item
    return None




def hydrate_missing_class_names(roots: List[UEObject]) -> None:
    by_export_path: Dict[str, str] = {}
    by_name_unique: Dict[str, str] = {}
    name_counts: Dict[str, set[str]] = {}

    def collect(obj: UEObject) -> None:
        if obj.class_name:
            if obj.export_path:
                by_export_path[obj.export_path] = obj.class_name
            if obj.name:
                name_counts.setdefault(obj.name, set()).add(obj.class_name)
        for child in obj.children:
            collect(child)

    for root in roots:
        collect(root)

    for name, classes in name_counts.items():
        if len(classes) == 1:
            by_name_unique[name] = next(iter(classes))

    def apply(obj: UEObject) -> None:
        if not obj.class_name:
            if obj.export_path and obj.export_path in by_export_path:
                obj.class_name = by_export_path[obj.export_path]
            elif obj.name and obj.name in by_name_unique:
                obj.class_name = by_name_unique[obj.name]
        for child in obj.children:
            apply(child)

    for root in roots:
        apply(root)

def first_line_value(lines: List[str], prefix: str) -> Optional[str]:
    for line in lines:
        if line.startswith(prefix):
            return line[len(prefix):].strip()
    return None


def parse_assignment_payload(text: str) -> Dict[str, Any]:
    text = text.strip()
    if text.startswith("(") and text.endswith(")"):
        text = text[1:-1]
    result: Dict[str, Any] = {}
    for token in split_top_level(text, ","):
        if not token or "=" not in token:
            continue
        key, value = token.split("=", 1)
        key = key.strip()
        value = value.strip()
        if value.startswith("(") and value.endswith(")"):
            inner = value[1:-1].strip()
            # Some UE fields use parenthesized raw lists rather than key=value payloads,
            # e.g. LinkedTo=(Node PinId,) or SubPins=(Node PinA,Node PinB,).
            # Only recurse when the inner payload actually looks like assignments.
            if "=" in inner:
                result[key] = parse_assignment_payload(value)
            else:
                result[key] = value
        elif value.startswith('"') and value.endswith('"'):
            result[key] = value[1:-1]
        else:
            result[key] = value
    return result


# ----------------------------
# Pin / node parsing
# ----------------------------

def parse_linked_to(value: Optional[str]) -> List[Dict[str, str]]:
    if not value:
        return []
    value = value.strip()
    if not (value.startswith("(") and value.endswith(")")):
        return []
    inner = value[1:-1].strip()
    if not inner:
        return []
    result: List[Dict[str, str]] = []
    for item in split_top_level(inner, ","):
        parts = item.strip().split()
        if len(parts) >= 2:
            result.append({"node": parts[0], "pin_id": parts[1]})
    return result


def parse_pin_line(line: str) -> Dict[str, Any]:
    content = line[len("CustomProperties Pin "):].strip()
    if content.startswith("(") and content.endswith(")"):
        content = content[1:-1]
    raw = parse_assignment_payload(f"({content})")
    direction_raw = raw.get("Direction")
    direction = "output" if direction_raw == "EGPD_Output" else "input"
    return {
        "id": raw.get("PinId"),
        "name": raw.get("PinName"),
        "direction": direction,
        "category": raw.get("PinType.PinCategory"),
        "subcategory": raw.get("PinType.PinSubCategory"),
        "subcategory_object": raw.get("PinType.PinSubCategoryObject"),
        "default_value": raw.get("DefaultValue"),
        "default_object": raw.get("DefaultObject"),
        "linked_to": parse_linked_to(raw.get("LinkedTo")),
        "raw": raw,
    }


def parse_member_name(payload: Optional[str], key: str = "MemberName") -> Optional[str]:
    if not payload:
        return None
    m = re.search(rf'{re.escape(key)}="([^"]+)"', payload)
    return m.group(1) if m else None


def parse_member_parent(payload: Optional[str]) -> Optional[str]:
    if not payload:
        return None
    m = re.search(r"MemberParent=([^,\)]+)", payload)
    return class_short_name(m.group(1)) if m else None


def parse_node(obj: UEObject) -> Dict[str, Any]:
    pins = [parse_pin_line(line) for line in obj.lines if line.startswith("CustomProperties Pin ")]

    title = obj.name or "Unknown"
    kind = class_short_name(obj.class_name) or "Unknown"
    meta: Dict[str, Any] = {}

    if kind == "K2Node_Event":
        event_ref = first_line_value(obj.lines, "EventReference=")
        member_name = parse_member_name(event_ref)
        title = member_name or title
        kind = "event"
        meta["event_name"] = member_name
    elif kind == "K2Node_ComponentBoundEvent":
        component = strip_quotes(first_line_value(obj.lines, "ComponentPropertyName="))
        delegate = strip_quotes(first_line_value(obj.lines, "DelegatePropertyName="))
        title = f"{component}.{delegate}" if component and delegate else title
        kind = "component_bound_event"
        meta["component"] = component
        meta["delegate_property"] = delegate
        meta["custom_function_name"] = strip_quotes(first_line_value(obj.lines, "CustomFunctionName="))
    elif kind == "K2Node_CallFunction":
        function_ref = first_line_value(obj.lines, "FunctionReference=")
        member_name = parse_member_name(function_ref)
        member_parent = parse_member_parent(function_ref)
        title = f"{member_parent}.{member_name}" if member_parent and member_name else (member_name or title)
        kind = "call_function"
        meta["function_name"] = member_name
        meta["function_owner"] = member_parent
    elif kind == "K2Node_DynamicCast":
        target_type = first_line_value(obj.lines, "TargetType=")
        target_name = class_short_name(target_type)
        title = f"Cast To {target_name}" if target_name else title
        kind = "dynamic_cast"
        meta["target_type"] = target_name
    elif kind == "K2Node_VariableSet":
        var_ref = first_line_value(obj.lines, "VariableReference=")
        var_name = parse_member_name(var_ref)
        title = f"Set {var_name}" if var_name else title
        kind = "variable_set"
        meta["variable_name"] = var_name
    elif kind == "K2Node_SpawnActorFromClass":
        spawn_class = None
        for pin in pins:
            if pin["name"] == "Class":
                spawn_class = short_object_name(pin.get("default_object"))
                break
        title = f"SpawnActor {spawn_class}" if spawn_class else "SpawnActor"
        kind = "spawn_actor"
        meta["spawn_class"] = spawn_class
    elif kind == "K2Node_Self":
        title = "Self"
        kind = "self"
    elif kind == "K2Node_FunctionEntry":
        func_name = strip_quotes(first_line_value(obj.lines, "CustomGeneratedFunctionName="))
        if not func_name:
            function_ref = first_line_value(obj.lines, "FunctionReference=")
            func_name = parse_member_name(function_ref)
        title = func_name or title
        kind = "function_entry"
        meta["function_name"] = func_name
    elif kind == "K2Node_SetVariableOnPersistentFrame":
        title = "SetVariableOnPersistentFrame"
        kind = "persistent_frame_set"

    important_inputs: List[Dict[str, Any]] = []
    for pin in pins:
        if pin["direction"] != "input":
            continue
        important = False
        if pin.get("default_object") not in (None, "None"):
            important = True
        if pin.get("default_value") not in (None, "None", "", "false", "0", "0.0"):
            important = True
        if pin.get("name") in {"Class", "WidgetClass", "WidgetToClose", "PlayerIndex", "Object", "self", "ShowcaseActor"}:
            important = True
        if pin.get("linked_to"):
            important = True
        if important:
            important_inputs.append({
                "name": pin.get("name"),
                "category": pin.get("category"),
                "default_value": pin.get("default_value"),
                "default_object": pin.get("default_object"),
                "linked_to": pin.get("linked_to"),
            })

    return {
        "id": obj.name,
        "class": class_short_name(obj.class_name),
        "kind": kind,
        "title": title,
        "meta": meta,
        "pins": [
            {
                "id": p["id"],
                "name": p["name"],
                "direction": p["direction"],
                "category": p["category"],
                "subcategory": p["subcategory"],
                "default_value": p["default_value"],
                "default_object": p["default_object"],
                "linked_to": p["linked_to"],
            }
            for p in pins
        ],
        "important_inputs": important_inputs,
        "raw_export_path": obj.export_path,
    }


def build_connections(modules: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
    pin_index: Dict[Tuple[str, str], Dict[str, Any]] = {}
    for module in modules:
        for pin in module["pins"]:
            if module["id"] and pin.get("id"):
                pin_index[(module["id"], pin["id"])] = {**pin, "node": module["id"]}

    edges: Dict[Tuple[str, str, str, str], Dict[str, Any]] = {}

    for module in modules:
        for pin in module["pins"]:
            for link in pin.get("linked_to", []):
                other = pin_index.get((link["node"], link["pin_id"]))
                if pin["direction"] == "output":
                    source_node = module["id"]
                    source_pin = pin["name"]
                    source_pin_id = pin["id"]
                    target_node = link["node"]
                    target_pin = other["name"] if other else None
                    target_pin_id = link["pin_id"]
                    category = pin.get("category")
                elif other and other["direction"] == "output":
                    source_node = link["node"]
                    source_pin = other["name"]
                    source_pin_id = link["pin_id"]
                    target_node = module["id"]
                    target_pin = pin["name"]
                    target_pin_id = pin["id"]
                    category = other.get("category")
                else:
                    source_node = module["id"]
                    source_pin = pin["name"]
                    source_pin_id = pin["id"]
                    target_node = link["node"]
                    target_pin = other["name"] if other else None
                    target_pin_id = link["pin_id"]
                    category = pin.get("category")

                if not source_node or not target_node:
                    continue
                key = (source_node, source_pin_id or "", target_node, target_pin_id or "")
                edges[key] = {
                    "from": {"node": source_node, "pin_name": source_pin, "pin_id": source_pin_id},
                    "to": {"node": target_node, "pin_name": target_pin, "pin_id": target_pin_id},
                    "category": category,
                }

    return sorted(edges.values(), key=lambda x: (x["from"]["node"], x["to"]["node"], x["from"]["pin_name"] or ""))


def build_exec_traces(modules: List[Dict[str, Any]], connections: List[Dict[str, Any]]) -> List[List[str]]:
    title_by_id = {m["id"]: m["title"] for m in modules}
    exec_edges: Dict[str, List[str]] = {}
    for edge in connections:
        if edge.get("category") == "exec":
            exec_edges.setdefault(edge["from"]["node"], []).append(edge["to"]["node"])

    starts = [m["id"] for m in modules if m["kind"] in {"event", "component_bound_event", "function_entry"}]

    traces: List[List[str]] = []

    def dfs(node_id: str, path: List[str], seen: set[str]) -> None:
        nxt = exec_edges.get(node_id, [])
        if not nxt:
            traces.append(path[:])
            return
        for child in nxt:
            if child in seen:
                traces.append(path[:] + [title_by_id.get(child, child)])
                continue
            dfs(child, path + [title_by_id.get(child, child)], seen | {child})

    for start in starts:
        dfs(start, [title_by_id.get(start, start)], {start})

    unique: List[List[str]] = []
    seen = set()
    for trace in traces:
        key = tuple(trace)
        if key not in seen:
            seen.add(key)
            unique.append(trace)
    return unique


# ----------------------------
# Graph, widget tree, variables
# ----------------------------

def is_detailed_graph(obj: UEObject) -> bool:
    return class_matches(obj.class_name, "Engine.EdGraph", "EdGraph") and any(
        line.startswith("Schema=") or line.startswith("GraphGuid=") for line in obj.lines
    )


def parse_graph(obj: UEObject) -> Dict[str, Any]:
    nodes = [child for child in obj.children if child.lines]
    modules = [parse_node(node) for node in nodes]
    connections = build_connections(modules)
    graph_name = obj.name or short_object_name(obj.export_path) or "UnknownGraph"
    schema = first_line_value(obj.lines, "Schema=")
    return {
        "name": graph_name,
        "schema": class_display_name(schema) if schema else schema,
        "module_count": len(modules),
        "connection_count": len(connections),
        "modules": modules,
        "connections": connections,
        "exec_traces": build_exec_traces(modules, connections),
    }


def parse_widget_tree(widget_tree_obj: Optional[UEObject]) -> Dict[str, Any]:
    if not widget_tree_obj:
        return {"root_widget": None, "widgets": [], "relationships": []}

    widget_objs = [
        child for child in iter_objects(widget_tree_obj)
        if child is not widget_tree_obj and child.lines and not class_matches(child.class_name, "UMG.WidgetTree", "WidgetTree")
    ]

    slot_objs = [obj for obj in widget_objs if (class_short_name(obj.class_name) or "").endswith("Slot")]
    real_widgets = [obj for obj in widget_objs if not (class_short_name(obj.class_name) or "").endswith("Slot")]

    widgets: Dict[str, Dict[str, Any]] = {}
    for obj in real_widgets:
        entry: Dict[str, Any] = {
            "name": obj.name,
            "class": class_short_name(obj.class_name),
            "display_label": strip_quotes(first_line_value(obj.lines, "DisplayLabel=")) or obj.name,
            "is_variable": (first_line_value(obj.lines, "bIsVariable=") == "True"),
            "slot": short_object_name(first_line_value(obj.lines, "Slot=")),
            "parent": None,
            "children": [],
            "properties": {},
        }
        for prop_key in [
            "Text",
            "BackgroundColor",
            "ColorAndOpacity",
            "Brush",
            "Visibility",
            "AutoWrapText",
            "bExpandedInDesigner",
        ]:
            value = first_line_value(obj.lines, f"{prop_key}=")
            if value is not None:
                entry["properties"][prop_key] = value
        widgets[obj.name or ""] = entry

    relationships: List[Dict[str, Any]] = []
    for slot in slot_objs:
        parent = short_object_name(first_line_value(slot.lines, "Parent="))
        content = short_object_name(first_line_value(slot.lines, "Content="))
        relation = {
            "slot_name": slot.name,
            "slot_class": class_short_name(slot.class_name),
            "parent": parent,
            "child": content,
            "layout_data": first_line_value(slot.lines, "LayoutData="),
        }
        if parent and content:
            relationships.append(relation)
            if content in widgets:
                widgets[content]["parent"] = parent
            if parent in widgets:
                widgets[parent]["children"].append(content)

    root_widget = short_object_name(first_line_value(widget_tree_obj.lines, "RootWidget="))
    return {
        "root_widget": root_widget,
        "widgets": list(widgets.values()),
        "relationships": relationships,
    }


def parse_variable_line(line: str) -> Dict[str, Any]:
    payload = line.split("=", 1)[1].strip()
    data = parse_assignment_payload(payload)
    var_type = data.get("VarType", {}) if isinstance(data.get("VarType"), dict) else {}
    return {
        "name": data.get("VarName"),
        "guid": data.get("VarGuid"),
        "friendly_name": data.get("FriendlyName"),
        "category": data.get("Category"),
        "property_flags": data.get("PropertyFlags"),
        "type": {
            "pin_category": var_type.get("PinCategory"),
            "pin_subcategory": var_type.get("PinSubCategory"),
            "pin_subcategory_object": var_type.get("PinSubCategoryObject"),
        },
        "metadata": data.get("MetaDataArray"),
    }


def parse_guid_map(line: Optional[str]) -> Dict[str, str]:
    if not line:
        return {}
    result: Dict[str, str] = {}
    for name, guid in re.findall(r'\("([^"]+)"\s*,\s*([0-9A-F]+)\)', line):
        result[name] = guid
    return result


def collect_special_settings(widget_tree: Dict[str, Any], blueprint: Dict[str, Any]) -> List[Dict[str, Any]]:
    settings: List[Dict[str, Any]] = []
    if blueprint.get("tick_prediction"):
        settings.append({
            "target": blueprint.get("name"),
            "property": "TickPrediction",
            "value": blueprint.get("tick_prediction"),
            "reason": blueprint.get("tick_prediction_reason"),
        })

    for widget in widget_tree.get("widgets", []):
        for key, value in widget.get("properties", {}).items():
            if key in {"Visibility", "BackgroundColor", "AutoWrapText", "Text", "Brush", "ColorAndOpacity"}:
                settings.append({
                    "target": widget["name"],
                    "property": key,
                    "value": value,
                })

    for relation in widget_tree.get("relationships", []):
        if relation.get("layout_data"):
            settings.append({
                "target": relation.get("child"),
                "property": "LayoutData",
                "value": relation.get("layout_data"),
            })
    return settings


# ----------------------------
# Top-level export
# ----------------------------

def build_export(text: str) -> Dict[str, Any]:
    roots = parse_object_tree(text)
    hydrate_missing_class_names(roots)
    all_objects = [obj for root in roots for obj in iter_objects(root)]

    blueprint = find_first(
        all_objects,
        lambda o: class_matches(o.class_name, "UMGEditor.WidgetBlueprint", "WidgetBlueprint"),
    )
    if not blueprint:
        blueprint = find_first(
            all_objects,
            lambda o: class_matches(o.class_name, "Engine.Blueprint", "Blueprint"),
        )
    if not blueprint:
        blueprint = find_first(
            all_objects,
            lambda o: class_short_name(o.class_name) in {"WidgetBlueprint", "Blueprint"},
        )
    if not blueprint:
        raise ValueError("No Blueprint root object found in input.")

    blueprint_class = class_short_name(blueprint.class_name) or class_display_name(blueprint.class_name)
    is_widget_blueprint = class_matches(blueprint.class_name, "UMGEditor.WidgetBlueprint", "WidgetBlueprint")

    blueprint_info = {
        "name": blueprint.name,
        "asset_path": blueprint.export_path,
        "blueprint_class": blueprint_class,
        "parent_class": class_display_name(first_line_value(blueprint.lines, "ParentClass=")),
        "generated_class": class_display_name(first_line_value(blueprint.lines, "GeneratedClass=")),
        "tick_prediction": first_line_value(blueprint.lines, "TickPrediction="),
        "tick_prediction_reason": strip_quotes(first_line_value(blueprint.lines, "TickPredictionReason=")),
    }

    graphs = [parse_graph(obj) for obj in iter_objects(blueprint) if is_detailed_graph(obj)]
    graphs.sort(key=lambda g: (g["name"] != "EventGraph", g["name"]))
    event_graph = next((g for g in graphs if g["name"] == "EventGraph"), None)
    other_graphs = [g for g in graphs if g["name"] != "EventGraph"]

    widget_tree_obj = find_first(
        iter_objects(blueprint),
        lambda o: class_matches(o.class_name, "UMG.WidgetTree", "WidgetTree") and any(line.startswith("RootWidget=") for line in o.lines),
    )
    widget_tree = parse_widget_tree(widget_tree_obj)

    variables = {
        "new_variables": [parse_variable_line(line) for line in blueprint.lines if line.startswith("NewVariables(")],
        "generated_variables": [parse_variable_line(line) for line in blueprint.lines if line.startswith("GeneratedVariables(")],
        "widget_variable_guid_map": parse_guid_map(first_line_value(blueprint.lines, "WidgetVariableNameToGuidMap=")),
    }

    export = {
        "blueprint": blueprint_info,
        "event_graph": event_graph,
        "other_graphs": other_graphs,
        "widget_tree": widget_tree,
        "variables": variables,
    }
    export["special_settings"] = collect_special_settings(widget_tree, blueprint_info)
    export["doc_view"] = {
        "blueprint_name": blueprint_info["name"],
        "blueprint_class": blueprint_class,
        "module_count": event_graph["module_count"] if event_graph else 0,
        "connection_count": event_graph["connection_count"] if event_graph else 0,
        "component_events": [
            {
                "title": module["title"],
                "id": module["id"],
                "component": module["meta"].get("component"),
                "delegate_property": module["meta"].get("delegate_property"),
            }
            for module in (event_graph["modules"] if event_graph else [])
            if module["kind"] == "component_bound_event"
        ],
        "exec_traces": event_graph["exec_traces"] if event_graph else [],
        "widget_outline": [
            {
                "name": widget["name"],
                "class": widget["class"],
                "parent": widget["parent"],
                "children": widget["children"],
            }
            for widget in widget_tree["widgets"]
        ],
        "notices": [
            "This export is built from full asset text, so it includes EventGraph, other detailed graphs, member variables, and generated variables that are serialized into the asset.",
            "Runtime-only state and values not serialized into the asset cannot be reconstructed from this exporter.",
        ],
    }
    if is_widget_blueprint:
        export["doc_view"]["notices"].insert(
            1,
            "WidgetBlueprint exports also include WidgetTree structure and widget-specific designer properties.",
        )
    return export


def read_t3d_text(path: Path, encoding: str = "utf-16") -> str:
    text = path.read_text(encoding=encoding)
    text = text.lstrip("\ufeff")
    text = text.replace("\r\n", "\n").replace("\r", "\n")
    return text


def repo_root_from_script() -> Path:
    return Path(__file__).resolve().parent.parent


def default_export_output_path(input_path: Path, repo_root: Path) -> Path:
    return repo_root / "Docs" / "BlueprintExports" / f"{input_path.stem}.json"


def default_md_output_path(json_path: Path, doc_dir: Path) -> Path:
    return doc_dir / f"{json_path.stem}.md"


def collect_matching_files(base_dir: Path, patterns: List[str]) -> List[Path]:
    if not base_dir.exists():
        return []
    matches = [
        path for path in base_dir.rglob("*")
        if path.is_file() and any(fnmatch.fnmatch(path.name, pattern) for pattern in patterns)
    ]
    return sorted(matches, key=lambda p: str(p).lower())


def collect_pending_t3d_files(tools_dir: Path, export_dir: Path, doc_dir: Path) -> List[Path]:
    pending: List[Path] = []
    for t3d_path in collect_matching_files(tools_dir, ["*.T3D", "*.t3d"]):
        json_path = export_dir / f"{t3d_path.stem}.json"
        md_path = doc_dir / f"{t3d_path.stem}.md"
        if json_path.exists() and md_path.exists():
            t3d_mtime = t3d_path.stat().st_mtime
            json_mtime = json_path.stat().st_mtime
            md_mtime = md_path.stat().st_mtime
            if t3d_mtime <= json_mtime and t3d_mtime <= md_mtime:
                continue
        pending.append(t3d_path)
    return pending


def export_t3d_to_json(input_path: Path, output_path: Path, encoding: str) -> Path:
    text = read_t3d_text(input_path, encoding)
    data = build_export(text)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(data, ensure_ascii=False, indent=2), encoding="utf-8")
    return output_path


def add_line(lines: List[str], text: str = "") -> None:
    lines.append(text)


def add_numbered_item(lines: List[str], text: str) -> None:
    add_line(lines, f"1. {text}")


def build_md_from_json_data(data: Dict[str, Any], json_filename: str) -> str:
    lines: List[str] = []

    blueprint = data["blueprint"]
    add_line(lines, f"# {blueprint['name']}")
    add_line(lines)

    add_line(lines, "## 1. 蓝图定位")
    add_line(lines)
    add_line(lines, f"本文档由 Docs/BlueprintExports/{json_filename} 自动整理生成，用于记录当前蓝图的真实导出结构。")
    add_line(lines)

    add_line(lines, "## 2. 基础信息")
    add_line(lines)
    add_numbered_item(lines, f"蓝图名：{blueprint['name']}")
    add_numbered_item(lines, f"资源路径：{blueprint['asset_path']}")
    if blueprint.get("blueprint_class"):
        add_numbered_item(lines, f"蓝图类型：{blueprint['blueprint_class']}")
    if blueprint.get("parent_class"):
        add_numbered_item(lines, f"父类：{blueprint['parent_class']}")
    if blueprint.get("generated_class"):
        add_numbered_item(lines, f"生成类：{blueprint['generated_class']}")
    if blueprint.get("tick_prediction"):
        add_numbered_item(lines, f"Tick 预测：{blueprint['tick_prediction']}")
    if blueprint.get("tick_prediction_reason"):
        add_numbered_item(lines, f"Tick 原因：{blueprint['tick_prediction_reason']}")

    add_line(lines)
    add_line(lines, "## 3. 当前状态")
    add_line(lines)
    add_numbered_item(lines, "本文档基于蓝图导出数据自动生成。")
    add_numbered_item(lines, "事件链、控件树、变量和特殊设置以导出内容为准。")
    add_numbered_item(lines, "如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。")

    widget_tree = data.get("widget_tree")
    doc_view = data.get("doc_view", {})
    if widget_tree:
        add_line(lines)
        add_line(lines, "## 4. 控件树")
        add_line(lines)

        if widget_tree.get("root_widget"):
            add_numbered_item(lines, f"根控件：{widget_tree['root_widget']}")

        widget_outline = doc_view.get("widget_outline") or []
        if widget_outline:
            add_line(lines)
            add_line(lines, "```text")
            for widget in widget_outline:
                parent = widget.get("parent") or "ROOT"
                children = ", ".join(widget.get("children") or []) if widget.get("children") else "无"
                add_line(lines, f"{widget['name']} [{widget['class']}] | parent={parent} | children={children}")
            add_line(lines, "```")

    component_events = doc_view.get("component_events") or []
    exec_traces = doc_view.get("exec_traces") or []

    add_line(lines)
    add_line(lines, "## 5. 事件图主链")
    add_line(lines)

    if component_events:
        add_line(lines, "### 5.1 组件事件")
        add_line(lines)
        for evt in component_events:
            delegate = evt.get("delegate_property") or "未知委托"
            add_numbered_item(lines, f"{evt.get('component')} -> {delegate}")
        add_line(lines)

    if exec_traces:
        add_line(lines, "### 5.2 主执行链")
        add_line(lines)
        for trace in exec_traces:
            add_numbered_item(lines, " -> ".join(trace))
    else:
        add_numbered_item(lines, "当前导出中未发现可整理的主执行链。")

    add_line(lines)
    add_line(lines, "## 6. 关键变量")
    add_line(lines)

    variables = data.get("variables", {})
    new_vars = variables.get("new_variables") or []
    if new_vars:
        add_line(lines, "### 6.1 自定义变量")
        add_line(lines)
        for var in new_vars:
            var_type = var["type"]["pin_category"]
            if var["type"].get("pin_subcategory"):
                var_type += f"/{var['type']['pin_subcategory']}"
            if var["type"].get("pin_subcategory_object"):
                var_type += f" -> {var['type']['pin_subcategory_object']}"
            add_numbered_item(lines, var["name"])
            add_line(lines, f"   类型：{var_type}")
            if var.get("friendly_name"):
                add_line(lines, f"   显示名：{var['friendly_name']}")
        add_line(lines)

    generated_vars = variables.get("generated_variables") or []
    if generated_vars:
        add_line(lines, "### 6.2 自动生成的控件变量")
        add_line(lines)
        for var in generated_vars:
            add_numbered_item(lines, f"{var['name']} -> {var['type']['pin_subcategory_object']}")
    elif not new_vars:
        add_numbered_item(lines, "当前导出中未发现自定义变量或生成变量。")

    special_settings = data.get("special_settings") or []
    add_line(lines)
    add_line(lines, "## 7. 特殊设置")
    add_line(lines)
    if special_settings:
        for setting in special_settings:
            target = str(setting.get("target") or "")
            prop = str(setting.get("property") or "")
            value = str(setting.get("value") or "")
            add_numbered_item(lines, f"{target}.{prop} = {value}")
            if setting.get("reason"):
                add_line(lines, f"   原因：{setting['reason']}")
    else:
        add_numbered_item(lines, "当前导出中未发现额外特殊设置。")

    def summarize_important_input(input_info: Dict[str, Any]) -> Optional[str]:
        name = input_info.get("name") or "Unknown"
        linked_to = input_info.get("linked_to") or []
        default_object = input_info.get("default_object")
        default_value = input_info.get("default_value")

        if linked_to:
            targets = ", ".join(
                f"{item.get('node')}:{item.get('pin_name') or item.get('pin_id')}"
                for item in linked_to
            )
            return f"{name} <- {targets}"
        if default_object not in (None, "", "None"):
            return f"{name} = {default_object}"
        if default_value not in (None, "", "None"):
            return f"{name} = {default_value}"
        return None

    def append_graph_key_modules(lines: List[str], graph: Dict[str, Any]) -> None:
        modules = graph.get("modules") or []
        if not modules:
            return

        interesting_kinds = {
            "component_bound_event",
            "call_function",
            "dynamic_cast",
            "variable_set",
            "spawn_actor",
            "function_entry",
        }
        key_modules = [m for m in modules if m.get("kind") in interesting_kinds][:12]
        if not key_modules:
            return

        add_numbered_item(lines, "关键节点：")
        for module in key_modules:
            details = module.get("title") or module.get("id") or "Unknown"
            add_line(lines, f"   - {details}")
            summarized_inputs = []
            for input_info in module.get("important_inputs") or []:
                summary = summarize_important_input(input_info)
                if summary:
                    summarized_inputs.append(summary)
            for summary in summarized_inputs[:4]:
                add_line(lines, f"     输入：{summary}")

    other_graphs = data.get("other_graphs") or []
    add_line(lines)
    add_line(lines, "## 8. 其他图表")
    add_line(lines)
    if other_graphs:
        for index, graph in enumerate(other_graphs, start=1):
            add_line(lines, f"### 8.{index} {graph['name']}")
            add_line(lines)
            add_numbered_item(lines, f"图类型：{graph['schema']}")
            add_numbered_item(lines, f"模块数：{graph['module_count']}")
            add_numbered_item(lines, f"连接数：{graph['connection_count']}")
            for trace in graph.get("exec_traces") or []:
                add_numbered_item(lines, f"执行链：{' -> '.join(trace)}")
            append_graph_key_modules(lines, graph)
            add_line(lines)
    else:
        add_numbered_item(lines, "当前导出中无其他图表。")

    add_line(lines, "## 9. 当前可直接确认的结论")
    add_line(lines)
    if exec_traces:
        add_numbered_item(lines, "本蓝图至少存在上文列出的执行链，可作为后续人工确认和接线补全文档的基础。")
    else:
        add_numbered_item(lines, "本蓝图当前更适合作为结构记录，真实逻辑仍需人工在编辑器中进一步确认。")
    if widget_tree:
        add_numbered_item(lines, "Widget 树已经从导出中整理出来，后续可以继续补充哪些控件参与了实际逻辑。")
    if new_vars or generated_vars:
        add_numbered_item(lines, "变量区已经整理完成，后续重点是确认哪些变量仍然在真实运行链中使用。")

    add_line(lines)
    add_line(lines, "## 10. 后续补充建议")
    add_line(lines)
    add_numbered_item(lines, "如果该蓝图已经过运行验证，建议补充一节“已运行验证链路”。")
    add_numbered_item(lines, "如果导出中存在历史残留变量、残留事件或旧链路，建议后续在本文档中手动标记。")
    add_numbered_item(lines, "如果本蓝图和别的蓝图存在强关联，建议在文档中补“对外依赖”。")

    return "\n".join(lines) + "\n"


def write_md_from_json(json_path: Path, md_path: Path) -> Path:
    data = json.loads(json_path.read_text(encoding="utf-8"))
    content = build_md_from_json_data(data, json_path.name)
    md_path.parent.mkdir(parents=True, exist_ok=True)
    md_path.write_text(content, encoding="utf-8")
    return md_path


def batch_convert_t3d_files(
    inputs: List[Path],
    repo_root: Path,
    encoding: str,
    output_dir: Optional[Path] = None,
) -> List[Path]:
    written: List[Path] = []
    for input_path in inputs:
        target_dir = output_dir if output_dir is not None else (repo_root / "Docs" / "BlueprintExports")
        output_path = target_dir / f"{input_path.stem}.json"
        written.append(export_t3d_to_json(input_path, output_path, encoding))
    return written


def batch_generate_md_files(json_paths: List[Path], doc_dir: Path) -> List[Path]:
    written: List[Path] = []
    for json_path in json_paths:
        written.append(write_md_from_json(json_path, default_md_output_path(json_path, doc_dir)))
    return written


def print_processed_outputs(t3d_paths: List[Path], json_paths: List[Path], md_paths: List[Path]) -> None:
    for t3d_path, json_path, md_path in zip(t3d_paths, json_paths, md_paths):
        print(f"Processed: {t3d_path.name}")
        print(f"  JSON -> {json_path}")
        print(f"  MD   -> {md_path}")


def sync_pending_blueprint_docs(
    tools_dir: Path,
    export_dir: Path,
    doc_dir: Path,
    repo_root: Path,
    encoding: str,
    dry_run: bool = False,
) -> Dict[str, List[Path]]:
    pending_t3d = collect_pending_t3d_files(tools_dir, export_dir, doc_dir)
    json_targets = [export_dir / f"{path.stem}.json" for path in pending_t3d]
    md_targets = [doc_dir / f"{path.stem}.md" for path in pending_t3d]

    if dry_run:
        return {"pending_t3d": pending_t3d, "json_written": json_targets, "md_written": md_targets}

    json_written = batch_convert_t3d_files(pending_t3d, repo_root, encoding, export_dir)
    md_written = batch_generate_md_files(json_written, doc_dir)
    print_processed_outputs(pending_t3d, json_written, md_written)
    return {"pending_t3d": pending_t3d, "json_written": json_written, "md_written": md_written}


def sync_all_blueprint_docs(
    tools_dir: Path,
    export_dir: Path,
    doc_dir: Path,
    repo_root: Path,
    encoding: str,
    dry_run: bool = False,
) -> Dict[str, List[Path]]:
    all_t3d = collect_matching_files(tools_dir, ["*.T3D", "*.t3d"])
    json_targets = [export_dir / f"{path.stem}.json" for path in all_t3d]
    md_targets = [doc_dir / f"{path.stem}.md" for path in all_t3d]

    if dry_run:
        return {"all_t3d": all_t3d, "json_written": json_targets, "md_written": md_targets}

    json_written = batch_convert_t3d_files(all_t3d, repo_root, encoding, export_dir)
    md_written = batch_generate_md_files(json_written, doc_dir)
    print_processed_outputs(all_t3d, json_written, md_written)
    return {"all_t3d": all_t3d, "json_written": json_written, "md_written": md_written}


def resolve_unreal_editor_binary(explicit_path: Optional[str]) -> Path:
    if explicit_path:
        editor_path = Path(explicit_path)
        if not editor_path.exists():
            raise FileNotFoundError(f"UnrealEditor-Cmd not found: {editor_path}")
        return editor_path

    candidates = [
        Path(r"D:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"),
        Path(r"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"),
        Path(r"D:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe"),
        Path(r"C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe"),
    ]
    for candidate in candidates:
        if candidate.exists():
            return candidate
    raise FileNotFoundError("Unable to find UnrealEditor-Cmd.exe. Please pass --unreal-editor.")


def asset_package_path_from_uasset(project_content_dir: Path, uasset_path: Path) -> str:
    relative = uasset_path.relative_to(project_content_dir).with_suffix("")
    return "/Game/" + "/".join(relative.parts)


def collect_target_blueprint_assets(project_content_dir: Path, patterns: List[str]) -> List[Path]:
    assets: List[Path] = []
    for pattern in patterns:
        assets.extend(project_content_dir.rglob(pattern))
    unique_assets = sorted({path.resolve() for path in assets if path.is_file()}, key=lambda p: str(p).lower())
    return unique_assets


def find_missing_t3d_asset_paths(project_content_dir: Path, tools_dir: Path, patterns: List[str]) -> List[str]:
    asset_paths: List[str] = []
    for uasset_path in collect_target_blueprint_assets(project_content_dir, patterns):
        expected_t3d = tools_dir / f"{uasset_path.stem}.T3D"
        if expected_t3d.exists():
            continue
        asset_paths.append(asset_package_path_from_uasset(project_content_dir, uasset_path))
    return asset_paths


def export_missing_t3d_files(
    repo_root: Path,
    unreal_editor: Optional[str],
    project_file: Optional[str],
    asset_patterns: List[str],
    dry_run: bool = False,
) -> List[str]:
    tools_dir = repo_root / "Tools"
    project_content_dir = repo_root / "Content"
    missing_asset_paths = find_missing_t3d_asset_paths(project_content_dir, tools_dir, asset_patterns)
    if dry_run or not missing_asset_paths:
        return missing_asset_paths

    project_path = Path(project_file) if project_file else next(repo_root.glob("*.uproject"), None)
    if project_path is None or not project_path.exists():
        raise FileNotFoundError("Unable to locate .uproject file. Please pass --project-file.")

    editor_binary = resolve_unreal_editor_binary(unreal_editor)
    helper_script = repo_root / "Tools" / "ue_export_missing_t3d_unreal.py"
    environment = os.environ.copy()
    environment["CODEX_T3D_ASSET_PATHS"] = json.dumps(missing_asset_paths, ensure_ascii=False)
    environment["CODEX_T3D_OUTPUT_DIR"] = str(tools_dir)

    command = [
        str(editor_binary),
        str(project_path),
        f"-ExecutePythonScript={helper_script}",
        "-unattended",
        "-nop4",
        "-nosplash",
        "-nullrhi",
        "-log",
    ]
    subprocess.run(command, check=True, cwd=repo_root, env=environment)

    return missing_asset_paths


def main() -> None:
    repo_root = repo_root_from_script()
    default_tools_dir = repo_root / "Tools"
    default_export_dir = repo_root / "Docs" / "BlueprintExports"
    default_doc_dir = repo_root / "Docs" / "Blueprints"

    parser = argparse.ArgumentParser(
        description="Convert UE Blueprint T3D exports into README-compliant JSON, and optionally export missing T3D files in one step."
    )
    parser.add_argument("input", nargs="?", help="Path to a single exported UE asset text file (.T3D or similar)")
    parser.add_argument("-o", "--output", help="Single-file output JSON path")
    parser.add_argument("--encoding", default="utf-16", help="Input text encoding. Default: utf-16")
    parser.add_argument(
        "--batch-from-tools",
        action="store_true",
        help="Convert all T3D files under Tools/ into Docs/BlueprintExports/*.json",
    )
    parser.add_argument(
        "--tools-dir",
        default=str(default_tools_dir),
        help=f"Directory containing exported T3D files. Default: {default_tools_dir}",
    )
    parser.add_argument(
        "--export-dir",
        default=str(default_export_dir),
        help=f"Directory for README-style JSON outputs. Default: {default_export_dir}",
    )
    parser.add_argument(
        "--doc-dir",
        default=str(default_doc_dir),
        help=f"Directory for generated Markdown docs. Default: {default_doc_dir}",
    )
    parser.add_argument(
        "--export-missing-t3d",
        action="store_true",
        help="Use UnrealEditor to export all matching Blueprint assets that do not yet have Tools/*.T3D",
    )
    parser.add_argument(
        "--asset-pattern",
        action="append",
        dest="asset_patterns",
        help="Glob pattern under Content/ for assets to export. Defaults to UI Blueprints only.",
    )
    parser.add_argument("--unreal-editor", help="Path to UnrealEditor-Cmd.exe")
    parser.add_argument("--project-file", help="Path to the .uproject file")
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Show which assets/files would be processed without writing anything",
    )
    parser.add_argument(
        "--sync-pending-docs",
        action="store_true",
        help="Process only T3D files missing generated JSON or Markdown, then generate both outputs in one run.",
    )
    parser.add_argument(
        "--generate-md-from-json",
        action="store_true",
        help="Generate Markdown docs from JSON files in export-dir.",
    )
    parser.add_argument(
        "--sync-all-docs",
        action="store_true",
        help="Rebuild JSON and Markdown docs for all T3D files under tools-dir.",
    )
    args = parser.parse_args()

    tools_dir = Path(args.tools_dir)
    export_dir = Path(args.export_dir)
    doc_dir = Path(args.doc_dir)
    asset_patterns = args.asset_patterns or ["UI/BP_*.uasset", "UI/W_*.uasset", "UI/LevelTargets/BP_*.uasset"]

    if args.export_missing_t3d:
        missing_assets = export_missing_t3d_files(
            repo_root=repo_root,
            unreal_editor=args.unreal_editor,
            project_file=args.project_file,
            asset_patterns=asset_patterns,
            dry_run=args.dry_run,
        )
        if missing_assets:
            print("Missing T3D assets:")
            for asset_path in missing_assets:
                print(f"  {asset_path}")
            if args.dry_run:
                return
            print(f"Exported {len(missing_assets)} missing T3D file(s) into {tools_dir}")
        else:
            print("No missing T3D files found.")

    if args.sync_pending_docs:
        result = sync_pending_blueprint_docs(
            tools_dir=tools_dir,
            export_dir=export_dir,
            doc_dir=doc_dir,
            repo_root=repo_root,
            encoding=args.encoding,
            dry_run=args.dry_run,
        )
        if args.dry_run:
            print("Pending T3D files:")
            for path in result["pending_t3d"]:
                print(f"  {path}")
            return

        count = len(result["pending_t3d"])
        print(f"Processed {count} pending T3D file(s).")
        print(f"JSON output dir: {export_dir}")
        print(f"Markdown output dir: {doc_dir}")
        return

    if args.sync_all_docs:
        result = sync_all_blueprint_docs(
            tools_dir=tools_dir,
            export_dir=export_dir,
            doc_dir=doc_dir,
            repo_root=repo_root,
            encoding=args.encoding,
            dry_run=args.dry_run,
        )
        if args.dry_run:
            print("All T3D files to process:")
            for path in result["all_t3d"]:
                print(f"  {path}")
            return

        count = len(result["all_t3d"])
        print(f"Processed {count} T3D file(s) in full rebuild mode.")
        print(f"JSON output dir: {export_dir}")
        print(f"Markdown output dir: {doc_dir}")
        return

    if args.batch_from_tools:
        inputs = collect_matching_files(tools_dir, ["*.T3D", "*.t3d"])
        if args.dry_run:
            print("T3D files to convert:")
            for input_path in inputs:
                print(f"  {input_path}")
            return
        written = batch_convert_t3d_files(inputs, repo_root, args.encoding, export_dir)
        print(f"Converted {len(written)} T3D file(s) into {export_dir}")
        return

    if args.generate_md_from_json:
        json_paths = collect_matching_files(export_dir, ["*.json"])
        if args.dry_run:
            print("JSON files to convert into Markdown:")
            for json_path in json_paths:
                print(f"  {json_path}")
            return
        written = batch_generate_md_files(json_paths, doc_dir)
        print(f"Generated {len(written)} Markdown doc(s) into {doc_dir}")
        return

    if not args.input:
        parser.error("Please provide an input file, or use --sync-pending-docs / --sync-all-docs / --batch-from-tools / --generate-md-from-json / --export-missing-t3d.")

    input_path = Path(args.input)
    output_path = Path(args.output) if args.output else default_export_output_path(input_path, repo_root)
    if args.dry_run:
        print(f"Would convert: {input_path} -> {output_path}")
        return
    export_t3d_to_json(input_path, output_path, args.encoding)
    md_path = write_md_from_json(output_path, default_md_output_path(output_path, doc_dir))
    print(f"Wrote JSON: {output_path}")
    print(f"Wrote Markdown: {md_path}")


if __name__ == "__main__":
    main()
