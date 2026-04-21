# BlueprintExports

这里存放蓝图导出的结构化数据，建议统一使用 `json` 文件。

命名建议：

1. `W_Level1.json`
2. `W_Level1_Introdection.json`
3. `W_Level2.json`
4. `BP_ShowcaseModel.json`
5. `BP_CaissonController.json`
6. `BP_LevelTarget_TiangongTower.json`
7. `BP_LevelTarget_CloudPattern.json`
8. `BP_LevelTarget_StarMap.json`

维护约定：

1. 一个蓝图对应一个 `json` 文件。
2. 文件名尽量和蓝图资产名一致。
3. 默认输出面向 AI 和开发阅读，会保留可读结构信息，但会去掉 `PinId` 一类仅用于内部匹配的 opaque ID。
4. 如果需要排查导出器或做精确对照，可通过 `--include-opaque-ids` 导出带这类 ID 的调试版本。
5. 后续 AI 会基于这里的结构化数据，整理到 `Docs/Blueprints/` 下的人类可读文档。

更新流程：

1. 先从 UE 手动导出对应蓝图的 `.T3D` 到 `Tools/`
2. 再运行：
   - `Docs/generate_blueprint_docs.ps1`
   - 或查看 `Tools/README.md`
3. 生成后的 `json` 会落到本目录
4. 对应的可读 `md` 会落到 `Docs/Blueprints/`

调试模式示例：

```powershell
powershell -ExecutionPolicy Bypass -File Docs\generate_blueprint_docs.ps1 --include-opaque-ids
```
