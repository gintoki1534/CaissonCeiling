# 文档总索引

本目录用于集中描述当前项目的开发阶段、蓝图职责、蓝图导出文档以及维护入口，方便开发者和 AI 在接手时快速找到“当前真实状态”。

如果是第一次接手项目，或者 AI 刚开始分析项目，请先从根目录的 [README.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/README.md) 开始，再顺着本页继续往下读。

## 1. 先看哪几份文档

如果要理解当前项目的整体开发状态，建议按下面顺序阅读：

1. [开发流程.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/开发流程.md)
   - 当前阶段目标
   - `Level2` 进度
   - 后续开发顺序
2. [Blueprint_Architecture.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprint_Architecture.md)
   - 当前蓝图职责边界
   - 已确认接线方式
   - C++ 与蓝图连接关系
3. [Level4_Puzzle.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Level4_Puzzle.md)
   - `Level4` 拼图当前实现
   - 普通/专家难度规则
   - 固定生成 Transform、拼图阶段、吸附、完成 UI 与 `Level5` 占位流转约定
4. [../CaissonCeiling_Migration_Guide.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/CaissonCeiling_Migration_Guide.md)
   - 当前重构原则
   - C++/蓝图分层结论

## 2. 当前蓝图状态从哪里看

如果要找某个蓝图当前到底接成什么样，优先看下面两个目录：

1. [Blueprints/README.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/README.md)
   - 人类可读版蓝图文档
   - 适合快速扫结构和关键链路
2. [BlueprintExports/README.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/BlueprintExports/README.md)
   - 原始结构化导出
   - 适合 AI 或需要确认详细节点时使用

当前这轮开发最关键的蓝图文档：

1. [W_Level2.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/W_Level2.md)
2. [BP_ShowcaseModel.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_ShowcaseModel.md)
3. [BP_ShowcaseModel_Level3.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_ShowcaseModel_Level3.md)
4. [W_Level3_Introdection.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/W_Level3_Introdection.md)
5. [W_Level3.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/W_Level3.md)
6. [BP_Level3RepairRegion_Dusting.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_Level3RepairRegion_Dusting.md)
7. [BP_CaissonController.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_CaissonController.md)
8. [W_Level2.json](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/BlueprintExports/W_Level2.json)
9. [BP_ShowcaseModel.json](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/BlueprintExports/BP_ShowcaseModel.json)
10. [BP_ShowcaseModel_Level3.json](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/BlueprintExports/BP_ShowcaseModel_Level3.json)
11. [W_Level3_Introdection.json](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/BlueprintExports/W_Level3_Introdection.json)
12. [W_Level3.json](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/BlueprintExports/W_Level3.json)
13. [W_Level4_Introdection.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/W_Level4_Introdection.md)
14. [W_Level4.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/W_Level4.md)
15. [BP_ShowcaseModel_Level4.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_ShowcaseModel_Level4.md)
16. [BP_L4_Layout_CloudFrame1.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_L4_Layout_CloudFrame1.md)
17. [BP_L4_Layout_CloudFrame2.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_L4_Layout_CloudFrame2.md)
18. [BP_L4_Layout_StarMap.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_L4_Layout_StarMap.md)
19. [BP_L4_Layout_FinalAssembly.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_L4_Layout_FinalAssembly.md)
20. [W_Level5.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/W_Level5.md)

## 3. 当前项目所处阶段

当前可以把项目理解为处于下面这个状态：

1. `MainMenu` 已可进入流程。
2. `Level1` 已切换到真实世界模型展示链路。
3. `Level2 Step 1` 的顺序点击三处目标已跑通。
4. `Level2` 已接通“正确点击目标 -> 镜头拉近 -> 显示介绍 UI -> 点击介绍 UI -> 回原视角”的流程闭环。
5. `W_Level2` 当前已新增继续提示容器，并已接通 `Level2 -> W_Level3_Introdection` 跳转链路。
6. `BP_ShowcaseModel` 当前已新增 `DeactivateLevel2Presentation`，用于 `Level2` 退场。
7. `BP_ShowcaseModel_Level3` 与 `W_Level3_Introdection` 当前已经建立，可在 `Level3` 介绍页中生成或复用第三关展示模型。
8. `Level3` 当前已经从“仅除尘”扩展为同一页面内的双子阶段闭环：

   - `W_Level3_Introdection -> W_Level3`
   - `Dusting` 阶段使用 5 个工具
   - `Oiling` 阶段使用 4 个工具，并隐藏第 5 个工具槽
   - 左键点击修复区域一次结算一次工具效果
   - 左下角三维数值、进度条和圆点实时刷新
   - `Dusting` 完成后点击“下一步”不关闭 `W_Level3`，而是切到 `Oiling`
   - `Oiling` 完成后进入 `W_Level4_Introdection`，开始第四关拼图流程
9. `Level3` 当前已经从占位打印推进到真实材质参数表现：

   - `BP_ShowcaseModel_Level3` 从 `BP_Level3RepairRegion_Dusting` 获取真实 `SM_RepairMesh`
   - 灰尘与桐油表现只驱动真实修复网格的 Slot 0
   - `M_Dust.DustConcentration01` 负责除尘视觉过渡
   - `M_Oil.OilBlend01` 负责涂油视觉过渡
   - 不再把 `SM_DustOverlay / SM_OilOverlay` 作为主表现路径

10. `Level4` 当前已经完整实现拼图主玩法：

   - 普通/专家难度均支持每片固定生成 Transform
   - 普通模式流程为 `CloudFrame1 -> StarMap -> FinalAssembly`
   - 专家模式流程为 `CloudFrame1 -> CloudFrame2 -> StarMap -> FinalAssembly`
   - `FinalAssembly` 禁用旋转，生成方向由配置保证
   - 最后一阶段完成后关闭 `BP_ShowcaseModel_Level4` 并进入 `W_Level5` 占位页

11. 当前下一步重点是：

   - 全流程打磨 `MainMenu -> Level1 -> Level2 -> Level3 -> Level4 -> W_Level5`
   - 收尾正式文案、音效、按钮反馈、阶段提示和最终完成反馈
   - 将 `W_Level5` 占位页推进为正式结果页、证书页或收尾演出

## 4. 蓝图文档如何更新

蓝图导出和文档更新入口位于：

1. [../Tools/README.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Tools/README.md)
2. [generate_blueprint_docs.ps1](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/generate_blueprint_docs.ps1)

日常增量更新：

```powershell
powershell -ExecutionPolicy Bypass -File Docs\generate_blueprint_docs.ps1
```

全量重建：

```powershell
powershell -ExecutionPolicy Bypass -File Docs\generate_blueprint_docs.ps1 --sync-all-docs
```

## 5. 标准协作开发流

以后默认采用下面这套流程：

1. 用户提出本次开发目标
2. AI 先阅读项目文档和相关蓝图文档
3. AI 先输出计划
4. 用户确认计划后，AI 负责修改代码
5. AI 引导开发者在 UE 里手动完成蓝图接线、资源配置和摆放
6. 开发完成后，AI 负责更新相关文档
7. AI 引导开发者只导出这次新改过的蓝图 `.T3D`
8. AI 引导开发者执行增量更新脚本，只更新新的蓝图文档

如果开发中卡住，或者 AI 需要确认蓝图当前真实接线，流程改为：

1. AI 指定需要查看的蓝图
2. 开发者从 UE 手动导出这些蓝图 `.T3D`
3. 只更新这些新的蓝图文档
4. AI 基于新的 `md/json` 继续分析和调试

## 6. 当前建议的接手入口

如果后续开发或 AI 需要继续接手 `Level4`，建议从下面顺序开始：

1. 先读 [开发流程.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/开发流程.md)
2. 再读 [Blueprint_Architecture.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprint_Architecture.md)
3. 再读 [Level4_Puzzle.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Level4_Puzzle.md)
4. 再查看：
   - [BP_ShowcaseModel_Level4.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_ShowcaseModel_Level4.md)
   - [W_Level4_Introdection.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/W_Level4_Introdection.md)
   - [W_Level4.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/W_Level4.md)
   - [BP_L4_Layout_CloudFrame1.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_L4_Layout_CloudFrame1.md)
   - [BP_L4_Layout_CloudFrame2.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_L4_Layout_CloudFrame2.md)
   - [BP_L4_Layout_StarMap.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_L4_Layout_StarMap.md)
   - [BP_L4_Layout_FinalAssembly.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/BP_L4_Layout_FinalAssembly.md)
   - [W_Level5.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/Blueprints/W_Level5.md)
5. 如果需要确认详细节点，再回看对应 `json`

## 7. 文档维护约定

后续每次改完蓝图或蓝图导出脚本，建议至少同步更新：

1. `Docs/开发流程.md`
2. `Docs/Blueprint_Architecture.md`
3. 对应蓝图的 `json/md`
4. 如有入口变化，再更新 [../Tools/README.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Tools/README.md)
