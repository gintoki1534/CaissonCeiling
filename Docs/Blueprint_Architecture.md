# 蓝图结构与接线说明

## 1. 文档目的

本文档用于给后续开发者和 AI 说明当前项目中蓝图资产的职责边界、项目结构，以及已经确认过的蓝图接线方式。

阅读原则：

1. 本文档只记录当前已经确认、已经跑通或已经在开发中形成约定的结构。
2. 没有明确验收过的蓝图，不把推测写成事实。
3. 如果蓝图接线发生变化，优先更新本文档，再继续功能开发。

配套文档：

1. [开发流程.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/开发流程.md)
   - 说明当前开发阶段、Level2 进度和后续开发顺序
2. [../README.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/README.md)
   - 项目根入口，约定 AI 与开发者的标准协作开发流
3. [CaissonCeiling_Migration_Guide.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/CaissonCeiling_Migration_Guide.md)
   - 说明当前 C++/蓝图职责边界和重构原则
4. [README.md](D:/xiaojia/CaissonCeiling/CaissonCeiling/Docs/README.md)
   - 文档总索引，方便定位当前蓝图状态、导出文档和维护入口
5. `Docs/BlueprintExports/`
   - 存放原始蓝图导出 `json`
6. `Docs/Blueprints/`
   - 存放按蓝图拆分后的可读文档

## 2. 当前项目结构总览

### 2.1 C++ 核心层

当前核心逻辑入口位于 `Source/CaissonCeiling/`：

1. `ACaissonPlayerController`
   - 输入绑定
   - 点击交互
   - 悬停检测
   - Level2 步骤推进
   - Level2 流程广播
   - Widget 打开/关闭
2. `ACaissonPawn`
   - 展示相机
   - 模型旋转
   - 模型枢轴 `ModelPivotComp`
3. `ACaissonGameMode`
   - 项目运行时 GameMode 入口
4. `UCaissonInteractComponent`
   - 可点击对象统一组件
   - 悬停/点击/高亮状态入口
5. `UCaissonUserWidget`
   - UI 蓝图基类扩展点
6. `ULevel3FlowComponent`
   - `Level3` 当前双子阶段独立状态机
   - 工具选择、子阶段切换、进度、评分和结果广播
7. `ULevel3RepairAreaComponent`
   - `Level3` 修复区域命中与覆盖率累计
8. `ALevel3RepairRegionActor`
   - `Level3` 修复区域蓝图壳层入口

### 2.2 当前主要蓝图资产

当前 `Content/UI/` 下可见的主要资产：

1. `BP_CaissonController`
2. `BP_CaissonGameMode`
3. `BP_ShowcaseModel`
4. `BP_ShowcaseModel_Level3`
5. `BP_Level3RepairRegion_Dusting`
6. `BP_ShowcaseModel_BC2`
7. `MainMenu`
8. `W_MainMenu`
9. `W_Level1`
10. `W_Level1_Introdection`
11. `W_Level2`
12. `W_Level3`
13. `W_Level3_Introdection`
14. `背景`
15. `M_UI_Model`
16. `RT_ModelViewer`
17. `LevelTargets/`

`Content/UI/LevelTargets/` 下当前目标蓝图：

1. `BP_LevelTarget_TiangongTower`
2. `BP_LevelTarget_CloudPattern`
3. `BP_LevelTarget_StarMap`

## 3. 当前推荐职责边界

### 3.1 C++ 应负责

1. 输入绑定与输入响应
2. 射线检测与点击判定
3. 步骤状态、顺序状态和流程状态
4. 可交互对象命中与广播
5. Widget 生命周期管理
6. 跨页面共享状态

### 3.2 蓝图应负责

1. Widget 布局
2. 图标、文案、动画、显隐
3. 模型资源挂载与关卡摆放
4. 悬停、点亮、灯光、音效等表现响应
5. 接收 C++ 委托后刷新 UI

### 3.3 不应重新写回蓝图的内容

1. 在 Widget 中维护 `CurrentStep`
2. 在 Widget 中重新做射线检测
3. 在蓝图中重写输入主逻辑
4. 在 UMG 中直接决定真实通关条件
5. 用蓝图保存跨页面真状态

## 4. 当前运行时架构结论

### 4.1 当前展示架构

当前项目已经从旧的 `SceneCapture + RT_ModelViewer + UMG 中央模型图` 架构，切换到：

1. 真实世界中的藻井模型
2. 真实世界中的背景板
3. UMG 只保留说明、进度、对话框和按钮

这意味着：

1. `Level1 / Level2` 的主模型显示不应再依赖 `RT_ModelViewer`
2. `W_Level1` 中央区域不应再被背景图或模型图层覆盖
3. 真实交互对象必须在世界中可见、可命中、可高亮

### 4.2 当前不建议继续使用的旧方案

以下方案已经在调试中被证明会引入重复模型、幽灵模型或点击错位，后续不要回退：

1. 用 `W_Level1` 或 `W_Level2` 的中央 `Image` 直接显示 `RT_ModelViewer`
2. 用 `SceneCaptureComponent2D` 作为主模型显示来源
3. 在模型区域上方叠加可命中的全屏背景图

`M_UI_Model`、`RT_ModelViewer`、以及旧的 `SceneCapture` 相关结构，只能视为历史残留资产，不应再作为 Level1/Level2 的主展示链路。

## 5. 已确认的关键蓝图接线

本节只记录已经通过本轮开发、排查和运行验证确认过的链路。

### 5.1 `BP_CaissonGameMode`

定位：

1. `ACaissonGameMode` 的蓝图壳层
2. 当前工程默认 `GameMode`

维护原则：

1. 只保留资源壳层配置
2. 不承载业务流程逻辑

### 5.2 `BP_CaissonController`

定位：

1. `ACaissonPlayerController` 的蓝图壳层
2. 当前输入与流程控制的运行时入口资产

当前应配置的核心资源：

1. `DefaultMappingContext -> IMC_Default`
2. `LookAction -> IA_Look`
3. `ClickAction -> IA_Click`
4. `RightClickAction -> IA_RightClick`

当前应配置的关键运行数据：

1. `RequiredLevel2TargetIds`
   - 顺序必须为：
   - `TiangongTower`
   - `CloudPattern`
   - `StarMap`
2. `bRightClickSkipEnabled`
   - 当前仅为兼容旧蓝图保留
   - 已不再参与输入绑定或跳步逻辑

维护原则：

1. 这里只配输入资源和少量调试参数
2. 不把步骤推进和点击判定搬回蓝图
3. 旋转模型的输入门控在 `ACaissonPlayerController` 中完成：
   - 未按住右键时，鼠标移动不应驱动模型旋转
   - 只有按住右键时，`LookAction` 才会继续传给 `ACaissonPawn`

### 5.3 `W_Level1`

定位：

1. `Level1` 的主 UI
2. 当前用于承载介绍阶段界面、说明 UI 和模型展示环境入口

已确认的结构性结论：

1. 中央模型区域不能再使用覆盖式背景图挡住真实模型
2. 若需要背景，应使用世界中的背景板，而不是 UMG 中央全屏背景
3. `Level1` 中央区域应尽量保持可交互区域干净，避免挡住模型点击

当前推荐接线约定：

1. 进入 `Level1` 时，如果需要创建展示用藻井 Pawn，应只创建一份
2. `Level1` 阶段应关闭目标点击推进能力：
   - `SetLevelTargetInteractionEnabled(false, false)`
3. `W_Level1` 中只保留：
   - 引导
   - 说明
   - 装饰 UI
   - 不影响模型交互的布局

不建议继续保留的旧接线：

1. `W_Level1 -> M_UI_Model -> RT_ModelViewer`
2. 模型区域上层的可命中 `BackGround`

### 5.4 `W_Level2`

定位：

1. Level2 的 UI 表现蓝图
2. 只监听 C++ 状态并刷新表现

当前已确认接线：

#### `Construct` 链路

当前已确认存在以下结构：

1. 初始隐藏完成图标 `StepFinish / StepFinish_1 / StepFinish_2`
2. 设置默认高亮显示
3. `GetPlayerController`
4. `Cast To CaissonPlayerController`
5. 保存 `CaissonControllerRef`
6. `Bind Event to OnStepChanged`
7. `Get Owning Player Pawn`
8. `Cast To BP_ShowcaseModel`
9. 保存 `ShowcaseRef`
10. `Bind Event to OnReturnToDefaultFinished`
11. `Bind Event to OnLevel2InspectStarted`
12. `Bind Event to OnLevel2FinalContinuePromptRequested`
13. `Bind Event to OnLevel2NextLevelRequested`

#### `OnStepChanged` 链路

当前已确认并修复的结构：

1. 从 `Bind Event to OnStepChanged` 的 `Event` 引脚重新创建匹配事件
2. 事件参数为 `NewStep`
3. `NewStep -> Switch on Int`
4. `Switch on Int` 的分支负责：
   - 调试打印
   - 切换 `StepFinish` 显隐
   - 切换 `Highlight` 显隐

#### 交互阶段开关

进入 `Level2` 时，应显式打开目标交互能力：

1. `GetPlayerController`
2. `Cast To CaissonPlayerController`
3. 调用：
   - `SetLevelTargetInteractionEnabled(true, true)`

这一步的意义：

1. `Level1` 不高亮、不推进
2. `Level2` 才启用：
   - 悬停高亮
   - 点击推进

#### 运行原则

1. `W_Level2` 不维护真步骤状态
2. 真状态由 `ACaissonPlayerController` 维护
3. UI 只消费：
   - `OnStepChanged`
   - `OnLevel2TargetProgressChanged`
   - 后续流程广播
4. `W_Level2` 当前负责介绍面板显隐、按钮点击回位，以及接收 Controller 与展示 Pawn 的回调
5. `Level2 -> Level3` 的真正跳转仍应放在蓝图层执行，但前置状态应由 Controller 广播决定

#### 当前状态判定

1. `W_Level2` 的步骤监听与 UI 刷新已可用
2. 文本框已改为不可编辑展示用途
3. 当前已新增 `Introducing_1 / Introducing_2 / Introducing_3`
4. 当前已新增 `Indtro_Button1 / Indtro_Button2 / Indtro_Button3`
5. 当前已新增变量：
   - `CaissonControllerRef`
   - `ShowcaseRef`
6. 当前已接入以下事件：
   - `OnLevel2InspectStarted`
   - `OnLevel2FinalContinuePromptRequested`
   - `OnLevel2NextLevelRequested`
   - `OnReturnToDefaultFinished`
7. 当前按钮点击后会调用 `BP_ShowcaseModel.ReturnToDefaultView`
8. 当前回原视角完成后会通过 `CompleteLevel2InspectPresentation` 继续放开关卡流程
9. 当前已新增 `continue` 容器，默认隐藏，用于显示“点击屏幕以继续”
10. 当前 `HandleFinalContinuePromptRequested` 已用于显示继续提示
11. 当前 `HandleNextLevelRequested` 已接入：
   - `BP_ShowcaseModel.DeactivateLevel2Presentation`
   - `CloseCaissonWidget`
   - `OpenCaissonWidget(W_Level3_Introdection)`
12. `Level2 -> Level3` 的入口链路已经建立，但仍需要继续联调整体验证

### 5.5 `BP_ShowcaseModel`

定位：

1. 当前藻井展示 Pawn 蓝图
2. 用于承载模型、相机和背景板

当前推荐结构应满足以下层级关系：

1. `SceneRootComp`
2. `ModelPivotComp`
3. `SpringArmComp`
4. `CameraComp`
5. `BackgroundPlane`
6. 3 个目标子对象挂在 `ModelPivotComp` 下

也就是说，蓝图内部逻辑上应满足：

1. 藻井模型跟随 `ModelPivotComp` 旋转
2. 相机不跟着模型一起转
3. 背景板位于模型后方
4. 背景板不参与交互，不拦截点击

当前结构性结论：

1. 当前主模型展示已经不再依赖 `SceneCapture`
2. 背景图已改为世界中的 `BackgroundPlane`
3. 模型已能围绕自身中心稳定旋转
4. 当前已增加用于镜头插值的变量：
   - `StartLoc`
   - `StartRot`
   - `TargetLoc`
   - `TargetRot`
   - `bReturningToDefault`
5. 当前已增加以下关键逻辑：
   - `FocusTargetById`
   - `ReturnToDefaultView`
   - `OnReturnToDefaultFinished`
   - `TL_CameraMove__UpdateFunc`
   - `TL_CameraMove__FinishedFunc`
6. 当前已新增：
   - `DeactivateLevel2Presentation`
7. `DeactivateLevel2Presentation` 当前用于 `Level2` 退场，执行：
   - `SetActorHiddenInGame(true)`
   - `SetActorEnableCollision(false)`
   - `SetActorTickEnabled(false)`
8. 当前 `BP_ShowcaseModel` 已承担 `Level2` 镜头拉近、回原位演出以及退场隐藏，不再只是静态展示 Pawn
9. 模型旋转输入门控不在该蓝图内实现，而由 `ACaissonPlayerController::Look()` 根据右键按住状态决定是否继续转发给 Pawn

### 5.5.1 `BP_ShowcaseModel_Level3`

定位：

1. `Level3` 当前的新展示 Pawn 蓝图
2. 复制自 `BP_ShowcaseModel` 后用于承载第三关介绍阶段与后续展示模型

当前已确认的结构结论：

1. 其父类当前仍为 `CaissonCeiling.CaissonPawn`
2. 当前已新增并接通：
   - `ActivateLevel3Presentation`
   - `ActivateRepairPresentation`
   - `ReturnToLevel3DefaultView`
   - `InitializeLevel3VisualMaterials`
   - `ApplyDustMaterialToRepairMesh`
   - `ApplyOilMaterialToRepairMesh`
   - `UpdateDustVisual`
   - `UpdateOilVisual`
3. `ActivateLevel3Presentation` 当前用于 `Level3` 展示模型激活，执行：
   - `SetActorHiddenInGame(false)`
   - `SetActorEnableCollision(...)`
   - `SetActorTickEnabled(true)`
4. `ActivateRepairPresentation / ReturnToLevel3DefaultView` 当前用于 `Level3` 修复子阶段的近景视角切换
5. `InitializeLevel3VisualMaterials` 当前负责从 `BP_Level3RepairRegion_Dusting.GetRepairMeshComponent` 获取真实 `SM_RepairMesh`，并为材质 Slot 0 创建 `DustMID_Slot0 / OilMID_Slot0`
6. `UpdateDustVisual / UpdateOilVisual` 当前已接入真实材质参数过渡：
   - Dusting 驱动 `M_Dust.DustConcentration01`
   - Oiling 驱动 `M_Oil.OilBlend01`
   - 只改真实修复网格 Slot 0，不改 Slot 1 Cloud 材质
7. 当前不把它视为“Level3 玩法逻辑蓝图”，而视为 `Level3` 的展示载体入口与视觉材质表现蓝图
8. 后续如果 `Level3` 建立新的 C++ 流程类，再决定是否需要调整父类或进一步拆职责

`BP_ShowcaseModel_BC2`：

1. 当前仓库中存在，但本轮未作为主链路继续维护
2. 视为历史或备用资产
3. 修改前先确认用途，不要让 AI 默认把它当成主展示蓝图

### 5.6 `LevelTargets/` 下目标蓝图

当前 3 个目标蓝图：

1. `BP_LevelTarget_TiangongTower`
2. `BP_LevelTarget_CloudPattern`
3. `BP_LevelTarget_StarMap`

这些蓝图当前的职责非常明确：

1. 挂真实模型
2. 配置唯一 `InteractionId`
3. 响应悬停开始、悬停结束、点击成功后的表现

当前推荐统一结构：

1. `StaticMeshComponent`
2. `CaissonInteractComponent`
3. 悬停光或临时高亮表现组件
4. 激活后常亮表现组件
5. 一个本地状态变量，例如 `bIsActivated`

当前推荐统一接线：

1. `OnHoverBegin`
   - 只做悬停表现
   - 不推进步骤
2. `OnHoverEnd`
   - 取消悬停表现
3. `OnInteractClicked`
   - 只在 C++ 判定“当前点击合法”后才会触发
   - 用于做正式点亮表现

当前已确认的关键规则：

1. 点亮表现不能在“射线命中时”就提前触发
2. 点亮表现应只绑定到 `OnInteractClicked`
3. 由于目标模型包含多种材质和多个材质槽，不建议采用“统一替换所有材质”的方案
4. 当前更合适的方向是：
   - 灯光
   - 局部发光
   - 后续可补 `CustomDepth` 描边

## 6. C++ 与蓝图的实际连接关系

### 6.1 输入与交互主链路

```text
BP_CaissonGameMode
-> BP_CaissonController
-> ACaissonPlayerController
-> RightClickAction.Started / Completed / Canceled 维护右键按住状态
-> LookAction 只有在右键按住时才继续驱动模型旋转
-> ClickAction 负责点击交互
-> Controller 执行射线检测 / 步骤推进 / 广播
-> 目标蓝图与 Widget 蓝图只接表现
```

补充说明：

1. `RightClickAction` 当前只承担“按住右键拖拽旋转模型”的输入门控，不再用于调试跳步。
2. 如果后续需要恢复调试跳步，应单独增加新的 Debug 输入资源，而不是复用右键。

### 6.2 当前模型展示链路

```text
展示 Pawn 蓝图（BP_ShowcaseModel）
-> 世界中的藻井模型
-> 世界中的背景板
-> CameraComp 观察模型
-> W_Level1 / W_Level2 只叠 UI，不再叠模型贴图
```

补充：

```text
W_Level3_Introdection
-> 构造时 GetAllActorsOfClass(BP_ShowcaseModel_Level3)
-> 若已有则复用
-> 若无则 SpawnActor(BP_ShowcaseModel_Level3)
-> Set Level3ShowcaseRef
-> ActivateLevel3Presentation
-> CaissonPlayerController.Possess
```

这条链路说明：

1. `Level3` 介绍页当前已能在半透明 UI 背后显示第三关模型
2. `W_Level3_Introdection` 当前承担 `Level3` 展示 Pawn 的创建或复用入口

### 6.2.1 Level3 当前双子阶段主链路

```text
W_Level3_Introdection.按钮-关闭
-> CaissonPlayerController.StartLevel3Dusting()
-> OpenCaissonWidget(W_Level3)

W_Level3.Construct
-> 获取 CaissonControllerRef / Level3FlowRef / Level3ShowcaseRef
-> 绑定 OnLevel3PhaseChanged / OnLevel3ToolSelected / OnLevel3ProgressChanged / OnLevel3ResultReady
-> 手动刷新当前 Phase 与 Progress

W_Level3.Dusting 阶段
-> 显示 Dusting 工具面板
-> Button_Tool1~5
-> RequestSelectTool
-> CaissonPlayerController.SelectLevel3Tool(ToolId)

鼠标左键点击修复区域
-> ACaissonPlayerController::OnPrimaryInteractPressed()
-> ULevel3FlowComponent::ApplySelectedToolToHit()
-> 结算当前工具的三维数值
-> 广播 OnLevel3ProgressChanged
-> W_Level3 刷新左下角数值 / 进度条 / 圆点
-> BP_ShowcaseModel_Level3.UpdateDustVisual()
-> ApplyDustMaterialToRepairMesh()
-> 真实 SM_RepairMesh Slot 0 切换到 DustMID_Slot0
-> 5 秒 Timeline 驱动 DustConcentration01 过渡

Dusting 达成结果
-> W_Level3.Button_Continue
-> BP_ShowcaseModel_Level3.ReturnToLevel3DefaultView()
-> CaissonPlayerController.AdvanceLevel3ToOiling()
-> W_Level3 不关闭
-> 右侧切换为 Oiling 工具面板
-> 重置三维数值 / 进度条 / 当前选中工具

W_Level3.Oiling 阶段
-> 显示 Oiling 工具面板
-> Button_Tool1~4
-> 隐藏 Button_Tool5 / Tools_5_Selected / Tools_5_Unselected
-> RequestSelectTool
-> CaissonPlayerController.SelectLevel3Tool(ToolId)

鼠标左键点击修复区域
-> ACaissonPlayerController::OnPrimaryInteractPressed()
-> ULevel3FlowComponent::ApplySelectedToolToHit()
-> 结算当前工具的三维数值
-> 广播 OnLevel3ProgressChanged
-> W_Level3 刷新左下角数值 / 进度条 / 圆点
-> BP_ShowcaseModel_Level3.UpdateOilVisual()
-> ApplyOilMaterialToRepairMesh()
-> 真实 SM_RepairMesh Slot 0 切换到 OilMID_Slot0
-> 5 秒 Timeline 驱动 OilBlend01 过渡

Oiling 达成结果
-> W_Level3.Button_Continue
-> BP_ShowcaseModel_Level3.ReturnToLevel3DefaultView()
-> CaissonPlayerController.CompleteLevel3ResultPresentation()
-> 进入 Completed 占位完成态
```

这条链路说明：

1. `Level3` 当前已经不再使用旧的“按住刷 + Tick 持续除尘”方案
2. 当前真实玩法是“选工具后单击模型一次，结算一次工具效果”
3. `Level3` 当前通过 `ELevel3SubStage` 在同一套 `W_Level3` 中切换 `Dusting / Oiling`
4. `BP_Level3RepairRegion_Dusting` 当前仍复用为单区域命中壳层
5. `W_Level3` 当前已经进入正式玩法开发状态，不再是纯占位页
6. 真实灰尘与桐油材质已经接入 `BP_ShowcaseModel_Level3`，当前采用真实修复网格 Slot 0 材质直驱，不再使用 Dust/Oil Overlay Mesh 作为主表现路径

### 6.3 Level2 当前主链路

```text
鼠标左键
-> ACaissonPlayerController::OnInteract()
-> GetInteractComponentUnderCursor()
-> HandleLevel2Interaction()
-> 顺序校验通过
-> UCaissonInteractComponent::TryInteract()
-> OnInteractClicked.Broadcast()
-> 目标蓝图播放点亮表现
-> AdvanceStep()
-> OnStepChanged.Broadcast(NewStep)
-> W_Level2 刷新 UI
```

补充：当前 `Level2` 已接通点击后镜头与介绍 UI 闭环：

```text
正确点击目标
-> ACaissonPlayerController 广播 OnLevel2InspectStarted(TargetId, bIsFinalTarget)
-> W_Level2 根据 TargetId 显示对应 Introducing 面板
-> W_Level2 调用 BP_ShowcaseModel.FocusTargetById(TargetId)
-> 镜头拉近到目标
-> 玩家点击介绍按钮
-> W_Level2 调用 BP_ShowcaseModel.ReturnToDefaultView()
-> TL_CameraMove 回位完成
-> BP_ShowcaseModel 广播 OnReturnToDefaultFinished
-> W_Level2 调用 CompleteLevel2InspectPresentation
```

### 6.4 悬停高亮链路

```text
PlayerTick
-> UpdateHoveredInteractable()
-> GetInteractComponentUnderCursor()
-> 交替触发 SetHoverHighlight(true/false)
-> UCaissonInteractComponent 广播 OnHoverBegin / OnHoverEnd
-> 目标蓝图播放悬停表现
```

### 6.5 Level2 顺序规则

当前 Level2 Step 1 已经明确要求按顺序点击：

1. `TiangongTower`
2. `CloudPattern`
3. `StarMap`

当前真实判定规则位于 `ACaissonPlayerController` 中：

1. 当前应点击目标 = `RequiredLevel2TargetIds[CurrentStep]`
2. 点击顺序不正确时：
   - 不推进
   - 不触发 `OnInteractClicked`
3. 只有点击顺序正确时，目标才会：
   - 触发表现
   - 被记录为已完成
   - 推进一步

## 7. 当前蓝图资产状态表

### 7.1 已形成稳定参考价值的资产

1. `BP_CaissonGameMode`
2. `BP_CaissonController`
3. `W_Level2`
4. `BP_ShowcaseModel`
5. `BP_ShowcaseModel_Level3`
6. `W_Level3_Introdection`
7. `W_Level3`
8. `BP_Level3RepairRegion_Dusting`
9. `LevelTargets/` 下 3 个目标蓝图

### 7.2 已存在，但仍应谨慎修改的资产

以下资产虽然存在，但本文档不把它们的内部接线全部视为“已经完全定稿”：

1. `W_MainMenu`
2. `MainMenu`
3. `W_Level1_Introdection`
4. `BP_ShowcaseModel_BC2`
5. `M_UI_Model`
6. `RT_ModelViewer`

对这些资产的要求：

1. 修改前先在编辑器确认当前接线
2. 如果接线经过验收，请把结果补写回本文档
3. 在未确认前，不要让 AI 把它们当成“已稳定定稿”的结构

补充说明：

1. `W_Level3` 当前已完成 `Level3` 的 `Dusting + Oiling` 双子阶段 UI 接线，真实灰尘与桐油材质已通过 `BP_ShowcaseModel_Level3` 接入
2. 当前项目里已实际建立的介绍页资产名为 `W_Level3_Introdection`
3. `W_Level3_Introdection` 当前已经完成基础接线，可作为 `Level3` 入口继续维护
4. 不建议继续直接复用 `W_Level1_Introdection` 原资产，因为它原本关闭后会打开 `W_Level2`

## 8. 后续新增蓝图时的标准写法

### 8.1 Widget 蓝图

应该优先采用下面的结构：

1. `Construct`
2. 获取 `CaissonPlayerController`
3. 绑定一个或多个 C++ 广播
4. 收到广播后只更新：
   - 文案
   - 图标
   - 显隐
   - 动画

不要默认在 Widget 中：

1. 保存真流程状态
2. 直接管理别的页面生命周期
3. 重新做点击判定

### 8.2 交互对象蓝图

应该优先采用下面的结构：

1. 目标 Actor 挂 `UCaissonInteractComponent`
2. 蓝图只绑定：
   - `OnInteractClicked`
   - `OnHoverBegin`
   - `OnHoverEnd`
3. 蓝图只做：
   - 悬停高亮
   - 点亮
   - 灯光
   - 音效
   - 材质或发光表现

### 8.3 页面流转蓝图

当前推荐思路：

1. 流转条件由 C++ 状态决定
2. 蓝图只在收到“可以跳转”的广播后执行表现
3. 页面打开/关闭尽量走 `OpenCaissonWidget` / `CloseCaissonWidget`

## 9. 文档维护规则

以后每次做完蓝图相关工作，至少补充下面三项：

1. 改的是哪个蓝图
2. 它现在承担什么职责
3. 关键接线链路是什么

如果没有经过运行验证，就不要写“已确认可用”。

如果当前蓝图现状不明确，先不要凭经验补文档，而应先：

1. 导出当前需要确认的蓝图 `.T3D`
2. 更新对应 `json/md`
3. 再回写本文档

## 10. 当前结论

当前项目已经形成了比较清晰的结构：

1. C++ 负责核心驱动
2. 蓝图负责表现和资源接线
3. `Level1 / Level2` 主模型已经统一回到真实世界模型
4. `Level2 Step 1` 的点击顺序、推进与点亮链已经收口
5. `Level2` 当前已经形成“点击目标 -> 镜头拉近 -> 介绍 UI -> 回原视角”的蓝图表现闭环

后续 AI 或开发者继续接手时，应优先沿着当前结构扩展，而不是回到：

1. `SceneCapture + UMG 模型显示`
2. Widget 自己维护步骤状态
3. 蓝图里同时写输入、状态、流程、页面管理
