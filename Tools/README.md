# Tools

本目录存放蓝图文档更新相关脚本，以及从 UE 手动导出的 `.T3D` 文件。

## 1. 当前脚本

1. `ue_asset_doc_exporter_fixed_v3.py`
   - 主脚本
   - 负责把 `Tools/*.T3D` 转成 `Docs/BlueprintExports/*.json`
   - 并进一步生成 `Docs/Blueprints/*.md`
   - 默认会去掉 `PinId` 一类 opaque ID，优先输出适合 AI/开发阅读的结构化结果
   - 如需保留这类调试字段，可追加 `--include-opaque-ids`
2. `ue_export_missing_t3d_unreal.py`
   - 给 UnrealEditor 调用的辅助脚本
   - 用于自动导出缺失的 `.T3D`

兼容入口：

1. `Docs/generate_blueprint_docs.ps1`
   - PowerShell 包装脚本
   - 不带参数时默认执行“增量更新”
   - 带参数时会原样透传给 Python 主脚本

## 2. 目录约定

1. 从 UE 手动导出的蓝图文本放在 `Tools/`
2. 文件名建议和蓝图资产名保持一致，例如：
   - `W_Level2.T3D`
   - `BP_ShowcaseModel.T3D`
   - `BP_CaissonController.T3D`
3. 结构化导出结果写入：
   - `Docs/BlueprintExports/*.json`
4. 可读文档写入：
   - `Docs/Blueprints/*.md`

## 3. 最常用命令

### 3.1 增量更新

适用场景：

1. 你刚从 UE 导出了 1 个或几个新的 `.T3D`
2. 只想更新新增或更新过的蓝图文档

命令：

```powershell
powershell -ExecutionPolicy Bypass -File Docs\generate_blueprint_docs.ps1
```

等价命令：

```powershell
py Tools\ue_asset_doc_exporter_fixed_v3.py --sync-pending-docs
```

规则：

1. 如果某个 `.T3D` 还没有对应的 `json/md`，会处理
2. 如果某个 `.T3D` 比现有 `json` 或 `md` 更新，也会处理
3. 其他未变化文件会跳过

### 3.2 全量重建

适用场景：

1. 你刚改了导出脚本
2. 你想把 `Tools/` 下所有 `.T3D` 对应的 `json/md` 全部重建

命令：

```powershell
powershell -ExecutionPolicy Bypass -File Docs\generate_blueprint_docs.ps1 --sync-all-docs
```

等价命令：

```powershell
py Tools\ue_asset_doc_exporter_fixed_v3.py --sync-all-docs
```

如果你需要保留调试用的 opaque ID，可追加：

```powershell
py Tools\ue_asset_doc_exporter_fixed_v3.py --sync-all-docs --include-opaque-ids
```

### 3.3 先看会处理哪些文件

增量预览：

```powershell
powershell -ExecutionPolicy Bypass -File Docs\generate_blueprint_docs.ps1 --dry-run
```

全量预览：

```powershell
powershell -ExecutionPolicy Bypass -File Docs\generate_blueprint_docs.ps1 --sync-all-docs --dry-run
```

### 3.4 单文件转换

适用场景：

1. 只想快速处理一个 `.T3D`

命令：

```powershell
py Tools\ue_asset_doc_exporter_fixed_v3.py Tools\W_Level2.T3D
```

这会同时生成：

1. `Docs\BlueprintExports\W_Level2.json`
2. `Docs\Blueprints\W_Level2.md`

### 3.5 只从 JSON 重新生成 Markdown

适用场景：

1. 改了 Markdown 生成逻辑
2. 不想重新解析 `.T3D`

命令：

```powershell
py Tools\ue_asset_doc_exporter_fixed_v3.py --generate-md-from-json
```

### 3.6 调试时保留 opaque ID

适用场景：

1. 需要和 UE 导出文本逐项对照
2. 需要排查连线解析或 pin 级问题

命令：

```powershell
py Tools\ue_asset_doc_exporter_fixed_v3.py --sync-pending-docs --include-opaque-ids
```

## 4. 推荐工作流

### 4.1 日常更新

1. 在 UE 中手动导出你刚改过的蓝图 `.T3D` 到 `Tools/`
2. 执行增量更新命令
3. 检查对应的：
   - `Docs/BlueprintExports/*.json`
   - `Docs/Blueprints/*.md`
4. 只更新这次新导出的蓝图，不默认全量重建

### 4.2 脚本升级后

1. 先执行全量预览
2. 再执行全量重建
3. 抽查关键蓝图，例如：
   - `W_Level2`
   - `BP_ShowcaseModel`
   - `BP_CaissonController`

### 4.3 调试时的推荐用法

如果 AI 或开发者只是想确认当前蓝图现状，不是做全量整理，推荐这样做：

1. 明确当前需要检查的蓝图
2. 只从 UE 导出这些蓝图的 `.T3D`
3. 执行默认增量更新
4. AI 再阅读新生成的：
   - `Docs/Blueprints/*.md`
   - `Docs/BlueprintExports/*.json`
5. 再继续分析和调试

## 5. 运行输出说明

脚本现在会逐个打印处理结果，例如：

```text
Processed: W_Level2.T3D
  JSON -> D:\...\Docs\BlueprintExports\W_Level2.json
  MD   -> D:\...\Docs\Blueprints\W_Level2.md
```

最后还会打印输出目录摘要。

## 6. 常见问题

### 6.1 为什么增量更新没有处理我新导出的文件？

先检查：

1. `.T3D` 是否真的放在 `Tools/`
2. 文件名是否与蓝图资产名一致
3. 新导出的 `.T3D` 时间戳是否确实比现有 `json/md` 更新

### 6.2 为什么 PowerShell 能运行，但 Python 直接运行失败？

包装脚本会优先尝试：

1. `py`
2. `python`

如果本机没有安装可用的 Python 解释器，PowerShell 包装脚本也会失败。

### 6.3 文档里为什么有信息，但不是完整逐节点蓝图复原？

当前策略是：

1. `json` 保留更完整的结构化数据，但默认会去掉仅用于内部匹配的 opaque ID
2. `md` 输出人类可读摘要和关键节点

需要深查时，优先看对应的 `json`。
