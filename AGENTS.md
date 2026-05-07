# AGENTS.md

This file provides guidance to Codex (Codex.ai/code) when working with code in this repository.

## Project Overview

隆福寺藻井数字展示项目 — a UE 5.6 PC digital exhibition for the Longfu Temple Caisson Ceiling. Three experience phases: Level1 (explore), Level2 (find & activate 3 targets), Level3 (Dusting + Oiling dual-stage restoration gameplay).

## Architecture: C++ Core + Blueprint Presentation

This is the single most important principle. Never violate this boundary.

**C++ owns**: input binding, ray-cast interaction, step/phase state machines, UI lifecycle, cross-page shared state, tool selection, scoring.

**Blueprints own**: widget layout, animations, icons, camera transitions, material/resource wiring, level placement, visual feedback.

**Anti-patterns that must never return**:
- Widget maintaining `CurrentStep` or any true flow state
- Widget doing `LineTraceByChannel` / `GetHitResultUnderCursor`
- Widget managing other widget lifecycle directly (use `OpenCaissonWidget`/`CloseCaissonWidget`)
- Blueprint re-implementing input or interaction logic
- Same state maintained in both C++ and blueprint
- Using `SceneCapture + RT_ModelViewer` for main model display (use real-world model instead)
- Full-screen background Image in UMG blocking model interaction

## Documentation-First Workflow

Before writing any code or analyzing blueprints, read project docs first. Never guess blueprint state.

**Reading order for onboarding**:
1. `Docs/开发流程.md` — current dev stage and next steps
2. `Docs/Blueprint_Architecture.md` — blueprint responsibilities and verified wiring
3. `CaissonCeiling_Migration_Guide.md` — refactoring principles and C++/blueprint boundaries
4. `Docs/Blueprints/*.md` — per-blueprint readable docs; `Docs/BlueprintExports/*.json` for deep node-level detail

**When blueprint state is unclear**: request a `.T3D` export from UE, run the doc tool, then analyze the generated `json/md`. Never assume wiring without verification.

## Blueprint Documentation Tool

After modifying blueprints in UE, export `.T3D` to `Tools/` and regenerate docs.

**Incremental update** (most common):
```powershell
powershell -ExecutionPolicy Bypass -File Docs\generate_blueprint_docs.ps1
```

**Full rebuild** (only after script changes):
```powershell
powershell -ExecutionPolicy Bypass -File Docs\generate_blueprint_docs.ps1 --sync-all-docs
```

**Single file**:
```powershell
py Tools\ue_asset_doc_exporter_fixed_v3.py Tools\W_Level3.T3D
```

## Standard Development Flow

1. User states the target → AI reads relevant docs and blueprint exports → AI outputs a plan
2. User confirms → AI modifies C++ code → AI guides user through UE blueprint wiring
3. User verifies in UE → AI updates docs → User exports changed `.T3D` → Run incremental doc update
4. Spot-check generated `json/md` for correctness

## Key C++ Classes

| Class | Responsibility |
|-------|---------------|
| `ACaissonPlayerController` | Main controller: Enhanced Input binding, click/hover interaction, Level2 step progression, Level3 flow delegation, UI lifecycle (`OpenCaissonWidget`/`CloseCaissonWidget`) |
| `ACaissonPawn` | Camera + model rotation via `ModelPivotComp`. Right-click held = rotation enabled. |
| `UCaissonInteractComponent` | Unified clickable component. Broadcasts `OnInteractClicked`, `OnHoverBegin`, `OnHoverEnd`. Handles pulse-light highlighting. |
| `ULevel3FlowComponent` | Level3 dual-stage state machine (Dusting/Oiling). Tool selection, progress tracking, result evaluation, visual transition locks. |
| `ULevel3RepairAreaComponent` | Repair area hit detection and coverage tracking |
| `ALevel3RepairRegionActor` | Blueprint shell for repair regions |
| `Level3Types.h` | Enums (`ELevel3Phase`, `ELevel3SubStage`) and structs (`FLevel3ToolSpec`, `FLevel3StageConfig`, `FLevel3ProgressState`, `FLevel3ResultState`) |

## Key Blueprint Assets

| Asset | Role |
|-------|------|
| `BP_CaissonController` | Shell for `ACaissonPlayerController` — only input resource config |
| `BP_CaissonGameMode` | Shell for `ACaissonGameMode` — only resource config |
| `BP_ShowcaseModel` | Level1/2 showcase Pawn: model, camera, background plane, camera transitions (`FocusTargetById`, `ReturnToDefaultView`) |
| `BP_ShowcaseModel_Level3` | Level3 showcase Pawn: `ActivateLevel3Presentation`, `UpdateDustVisual`, `UpdateOilVisual`, `ReturnToLevel3DefaultView` |
| `W_Level2` | Listens to C++ broadcasts, refreshes UI only |
| `W_Level3` | Drives Dusting/Oiling tool panels, progress display, sub-stage switching within same widget |
| `W_Level3_Introdection` | Level3 entry: spawns/reuses `BP_ShowcaseModel_Level3`, calls `ActivateLevel3Presentation` |
| `BP_LevelTarget_*` | 3 interactive targets with `CaissonInteractComponent`, only visual feedback on hover/click |
| `BP_Level3RepairRegion_Dusting` | Repair region hit shell for Level3 |

## Level3 Flow Architecture

```
W_Level3_Introdection → CaissonPlayerController.StartLevel3Dusting()
                       → OpenCaissonWidget(W_Level3)

W_Level3 binds: OnLevel3PhaseChanged, OnLevel3ToolSelected,
                OnLevel3ProgressChanged, OnLevel3ResultReady

Player clicks repair area → Controller.OnPrimaryInteractPressed()
                           → Level3FlowComponent.ApplySelectedToolToHit()
                           → Broadcasts progress → W_Level3 refreshes
                           → BP_ShowcaseModel_Level3.UpdateDustVisual/OilVisual

Dusting complete → Continue → AdvanceLevel3ToOiling()
                 → Same W_Level3, right panel switches to Oiling tools

Oiling complete → Continue → CompleteLevel3ResultPresentation()
               → Completed placeholder state
```

Tool mechanics: "select tool, click model once, settle tool effect once" — NOT hold-and-brush.

Metrics: Cleanliness / Integrity / Aesthetics, range -2 to 2.

## Level2 Interaction Chain

```
Click → Controller ray-cast → HandleLevel2Interaction()
      → Sequential check (TiangongTower → CloudPattern → StarMap)
      → OnInteractClicked broadcast → target blueprint plays activation
      → AdvanceStep → OnStepChanged broadcast → W_Level2 refreshes
      → OnLevel2InspectStarted → camera zoom + intro UI
      → ReturnToDefaultView → OnReturnToDefaultFinished → next target
```

## Conventions

- All source text encoding: UTF-8
- Enhanced Input only (`IMC_Default`, `IA_Look`, `IA_Click`, `IA_RightClick`)
- Right-click = hold to rotate model (no longer a debug skip)
- Widget pages open/close via `OpenCaissonWidget` / `CloseCaissonWidget`
- New interactive objects must use `UCaissonInteractComponent`, never custom click logic in blueprint
- Highlight/activation effects should NOT replace all materials on multi-material targets; use lights/emissive/CustomDepth instead
- `BP_ShowcaseModel_BC2`, `M_UI_Model`, `RT_ModelViewer` are legacy assets — do not treat as primary display path

## Post-Development Checklist

- Did core flow logic end up in blueprint? (Move to C++)
- Duplicate state variables? (Remove from blueprint)
- Widget managing other widget lifecycle? (Use Controller API)
- Interaction detection in UMG? (Move to C++)
- Orphan nodes / debug PrintString left behind? (Clean up)
- Blueprint docs updated? (Export `.T3D`, run incremental update)
- `Blueprint_Architecture.md` updated if wiring changed?
