import json
import os

import unreal


def read_required_env(name: str) -> str:
    value = os.environ.get(name, "").strip()
    if not value:
        raise RuntimeError(f"Missing required environment variable: {name}")
    return value


def main() -> None:
    asset_paths = json.loads(read_required_env("CODEX_T3D_ASSET_PATHS"))
    output_dir = read_required_env("CODEX_T3D_OUTPUT_DIR")

    os.makedirs(output_dir, exist_ok=True)

    failures = []
    for asset_path in asset_paths:
        asset = unreal.EditorAssetLibrary.load_asset(asset_path)
        if not asset:
            failures.append({"asset_path": asset_path, "reason": "load_failed"})
            continue

        task = unreal.AssetExportTask()
        task.object = asset
        task.filename = os.path.join(output_dir, f"{asset.get_name()}.T3D")
        task.automated = True
        task.prompt = False
        task.replace_identical = True
        task.use_file_archive = False
        task.write_empty_files = False
        task.exporter = unreal.ObjectExporterT3D()

        if not unreal.Exporter.run_asset_export_task(task):
            failures.append({"asset_path": asset_path, "reason": "export_failed", "filename": task.filename})

    if failures:
        raise RuntimeError("Failed to export some assets: " + json.dumps(failures, ensure_ascii=False))

    print(f"Exported {len(asset_paths)} asset(s) into {output_dir}")


if __name__ == "__main__":
    main()
