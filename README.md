# CaissonCeiling

本项目是基于 Unreal Engine 的隆福寺藻井数字展示项目。当前项目已经明确采用：

1. `C++` 负责核心交互、状态和流程控制
2. 蓝图负责 UI 表现、镜头演出、资源接线和关卡摆放
3. 蓝图状态说明必须以项目内导出的蓝图文档为准，而不是靠运行时猜测

当前项目主需求已经完整实现，开发重心已经从主玩法搭建转入整体打磨、表现优化、正式文案、音效、结果页和打包前验收。

## 1. AI / 开发者接手时的第一原则

无论是 AI 还是开发者，在开始分析项目前，都应先阅读项目内文档，而不是直接跳到 `.uasset` 或凭经验推断蓝图状态。

推荐阅读顺序：

1. [Docs/README.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/README.md)
2. [Docs/开发流程.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/开发流程.md)
3. [Docs/Blueprint_Architecture.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprint_Architecture.md)
4. [Docs/Level4_Puzzle.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Level4_Puzzle.md)
5. [CaissonCeiling_Migration_Guide.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/CaissonCeiling_Migration_Guide.md)

如果要确认某个蓝图当前具体状态，优先查看：

1. [Docs/Blueprints/README.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/README.md)
2. [Docs/BlueprintExports/README.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/BlueprintExports/README.md)

其中：

1. `Docs/Blueprints/*.md` 适合快速阅读
2. `Docs/BlueprintExports/*.json` 适合深查节点、变量和函数图细节

## 2. AI 默认工作约定

后续协作默认遵循下面这条规则：

1. 先读项目文档
2. 再结合 C++ 和蓝图导出文档分析现状
3. 不能脱离项目文档自行假设蓝图接线已经改变
4. 如果蓝图现状不明确，先要求导出当前需要分析的蓝图，再继续判断

## 3. 标准开发流

### 3.1 正常功能开发

1. 用户先提出本次要开发的模块或目标
2. AI 先阅读相关文档和蓝图导出文档
3. AI 输出本次开发计划
4. 用户确认计划后，AI 负责修改代码
5. AI 引导开发者在 UE 里完成必须手动做的蓝图接线、资源配置和摆放
6. 开发者在 UE 中手动验证
7. AI 根据验证结果继续收尾

### 3.2 开发完成后的固定动作

1. 更新相关文档
2. 引导开发者从 UE 手动导出这次新改过的蓝图 `.T3D`
3. 只更新新的或刚改过的蓝图文档，不默认全量重建
4. 让开发者执行增量更新脚本
5. 抽查对应 `json/md` 是否写入成功

## 4. 调试与蓝图现状确认流程

如果开发中卡住，或者 AI 需要确认蓝图当前真实接线、变量、函数图、按钮逻辑、事件绑定等，不要直接靠猜。

应该按下面流程进行：

1. AI 明确指出当前需要确认的蓝图
2. AI 引导开发者在 UE 中手动导出这些蓝图的 `.T3D`
3. 把导出的 `.T3D` 放到 `Tools/`
4. 只更新这些新的蓝图文档
5. AI 读取新生成的 `Docs/Blueprints/*.md` 和 `Docs/BlueprintExports/*.json`
6. 再继续分析、定位问题和给出调试方案

## 5. 蓝图文档更新入口

1. [Tools/README.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Tools/README.md)
2. [Docs/generate_blueprint_docs.ps1](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/generate_blueprint_docs.ps1)

日常开发完成后，默认执行“只更新新的蓝图”：

```powershell
powershell -ExecutionPolicy Bypass -File Docs\generate_blueprint_docs.ps1
```

只有改了导出脚本、Markdown 生成规则，或者需要统一刷新所有蓝图文档时，才建议全量重建：

```powershell
powershell -ExecutionPolicy Bypass -File Docs\generate_blueprint_docs.ps1 --sync-all-docs
```

## 6. 一句话原则

后续 AI 接手本项目时，必须：

1. 先看文档
2. 先看蓝图导出文档
3. 先确认真实状态
4. 再做计划、写代码、指导 UE 手动操作和更新文档

## 7. 当前完整流程状态

当前已跑通的主流程为：

```text
MainMenu
-> Level1
-> Level2
-> Level3 Dusting
-> Level3 Oiling
-> Level4 Introduction
-> Level4 Puzzle
-> W_Level5 Placeholder
```

`Level4` 当前已完成：

1. 普通/专家难度
2. 四阶段拼图配置，其中普通模式跳过 `CloudFrame2`
3. 每片碎片普通/专家固定生成 Transform
4. 鼠标命中、拖拽、右键选择/旋转、吸附和完成判定
5. `FinalAssembly` 阶段禁用旋转
6. 完成后关闭 `BP_ShowcaseModel_Level4` 并进入 `W_Level5` 占位页

后续工作应优先围绕打磨和最终交付，不应再返工主玩法架构。
