# BlueprintExports

这里存放蓝图导出的原始结构化数据，建议统一使用 `json` 文件。

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
3. 这里保留原始导出内容，不手动整理、不删字段。
4. 后续 AI 会基于这里的原始数据，整理到 `Docs/Blueprints/` 下的人类可读文档。

更新流程：

1. 先从 UE 手动导出对应蓝图的 `.T3D` 到 `Tools/`
2. 再运行：
   - `Docs/generate_blueprint_docs.ps1`
   - 或查看 `Tools/README.md`
3. 生成后的 `json` 会落到本目录
4. 对应的可读 `md` 会落到 `Docs/Blueprints/`
