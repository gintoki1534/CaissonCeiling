# CaissonCeiling 重构交接说明

## 1. 文档目的

本文用于向后续开发者说明本分支自克隆基线以来完成了哪些重构工作、当前项目的职责边界，以及后续功能应如何继续开发。

如果需要查看“当前蓝图资产有哪些、哪些已经验收、具体怎么接线”，请配合阅读：

- `Docs/Blueprint_Architecture.md`
- `Docs/README.md`

结论先行：

- 项目已经从“纯蓝图承载核心逻辑”迁移到“C++ 负责核心驱动，蓝图负责表现层”
- 后续开发不应再把输入、流程、交互判定和 UI 生命周期重新堆回蓝图
- 新功能默认先设计 C++ 真状态，再暴露给蓝图使用

## 2. 当前重构目标与边界

本次重构的目标不是完全消灭蓝图，而是建立稳定的职责分层：

- C++ 负责：
  - 输入绑定与输入响应
  - 视角/模型控制
  - 状态推进与流程控制
  - 射线检测与交互触发
  - UI 打开、关闭、焦点与 InputMode 管理
  - 跨页面共享状态

- 蓝图负责：
  - Widget 布局
  - 动画、图标、材质和资源引用
  - 世界模型、背景板和关卡资源挂载
  - 页面表现刷新
  - 关卡摆放和资源组织

如果某个功能同时涉及“输入、流程、交互、跨页面状态”中的任意一项，就不应先在蓝图里实现。

## 3. 相对当前仓库基线已完成的结构性变更

### 3.1 新增 Runtime C++ 模块

项目现在包含运行时模块 `CaissonCeiling`，见：

- `CaissonCeiling.uproject`
- `Source/CaissonCeiling/CaissonCeiling.Build.cs`

当前模块已经引入以下关键依赖：

- `UMG`
- `EnhancedInput`
- `Slate`
- `SlateCore`

### 3.2 新增核心 C++ 类

当前已新增并接入以下核心类：

- `ACaissonPlayerController`
  - 统一输入绑定入口
  - 步骤推进与广播
  - 鼠标点击交互
  - UI 生命周期管理

- `ACaissonPawn`
  - 展示相机与模型旋转控制

- `ACaissonGameMode`
  - 接入新的 PlayerController

- `UCaissonInteractComponent`
  - 提供统一的可点击交互入口
  - 提供悬停与点击表现广播

- `UCaissonUserWidget`
  - 作为 UI 蓝图的 C++ 基类扩展点
- `ULevel3FlowComponent`
  - `Level3` 第一阶段独立流程与评分状态机
- `ULevel3RepairAreaComponent`
  - `Level3` 修复区域覆盖率累计组件
- `ALevel3RepairRegionActor`
  - 供蓝图挂网格和修复区域组件的 `Level3` 区域壳层

### 3.3 输入体系切换为 Enhanced Input

当前工程已新增输入资源：

- `Content/Input/IMC_Default`
- `Content/Input/IA_Look`
- `Content/Input/IA_Click`
- `Content/Input/IA_RightClick`

并且 `Config/DefaultInput.ini` 已切换到：

- `EnhancedPlayerInput`
- `EnhancedInputComponent`

### 3.4 默认入口蓝图已切换到 C++ 壳层

当前默认 GameMode 已配置为：

- `BP_CaissonGameMode`

该蓝图应被视为 `ACaissonGameMode` 的资产壳层，而不是重新承载业务逻辑。

## 4. 当前关键类职责

### 4.1 `ACaissonPlayerController`

这是当前项目的主控制入口，后续绝大多数核心功能应优先扩展这里。

当前职责包括：

- `BeginPlay` 时添加默认输入映射
- `SetupInputComponent` 中绑定 Look / Click / RightClick
- `RightClickAction` 当前只负责维护“右键是否按住”的旋转门控状态
- `Look` 当前只在按住右键时才继续驱动模型旋转
- `AdvanceStep` / `ResetSteps` 维护步骤状态
- `OnStepChanged` 作为 UI 统一监听委托
- `OnLevel2TargetProgressChanged` / `OnLevel2TargetActivated` 负责 Level2 目标广播
- `OpenCaissonWidget` / `CloseCaissonWidget` 统一管理 UI 生命周期
- `OnInteract` 通过射线检测触发 `UCaissonInteractComponent`
- `UpdateHoveredInteractable` 负责悬停检测

### 4.2 `ACaissonPawn`

当前负责展示相机与模型旋转，不应再在蓝图中重写同类逻辑。

当前关键结构包括：

- `SceneRootComp`
- `ModelPivotComp`
- `SpringArmComp`
- `CameraComp`

模型应挂在 `ModelPivotComp` 下，由 C++ 统一驱动旋转。

后续如果需要新增：

- 缩放
- 阻尼
- 角度限制
- 触摸输入

应继续扩展 `ACaissonPawn`，而不是另起一套蓝图旋转逻辑。

### 4.3 `UCaissonInteractComponent`

这是后续所有可点击部件的统一挂载点。

凡是需要：

- 点击高亮
- 局部构件说明
- 构件点击反馈

都建议优先挂这个组件，而不是在每个蓝图里各写一套点击事件。

当前该组件还承担：

- `OnHoverBegin`
- `OnHoverEnd`
- `OnInteractClicked`

这三个蓝图事件入口。

## 5. 当前蓝图层的收口情况

### 5.1 已收口为“表现层壳”的蓝图

根据当前工作区与已完成验收，以下蓝图已经回到合理职责：

- `BP_CaissonController`
  - 应只保留输入资源映射、参数配置、资产引用

- `BP_CaissonGameMode`
  - 应只保留 GameMode 资源壳层配置

- `W_Level2`
  - 步骤推进由 C++ 接管
  - Widget 只监听 `OnStepChanged` 并刷新表现
  - 右键当前只承担按住拖拽旋转模型，不再承担开发跳步
  - 当前已增加介绍 UI、镜头回位后的回调承接以及 `Level2` 流程广播绑定

- `BP_ShowcaseModel`
  - 作为真实世界中的藻井展示 Pawn 蓝图
  - 承载模型、相机和背景板
  - 不再承载旧的 SceneCapture 主展示链路
  - 当前已增加 `FocusTargetById` / `ReturnToDefaultView` / `OnReturnToDefaultFinished`

### 5.2 仍需注意的事项

- `W_Level3` 已完成第一阶段除尘玩法的基础接线
  - 当前状态是“占位数值 + 占位视觉调试”
  - 真实灰尘材质和最终视觉效果仍需后续接入

- 蓝图资产的最新职责与接线，请以 `Docs/Blueprint_Architecture.md` 为准
  - 该文档面向后续开发者与 AI
  - 负责记录当前项目结构与已验证接线

- 历史蓝图中如果仍残留：
  - 禁用节点
  - 孤儿节点
  - 纯调试节点
  - 已无引用变量
  应继续清理，但不要把表现初始化误删掉

## 6. 后续开发规范

### 6.1 新功能开发顺序

建议按以下顺序开发：

1. 先确定“真状态”由哪个 C++ 类维护
2. 在 C++ 中增加最小必要接口
3. 再让蓝图只做资源和表现接线
4. 最后删除旧蓝图残留，避免双轨逻辑并存

### 6.2 新页面开发规范

新增页面时，应遵循：

1. 页面只监听 C++ 广播出来的状态
2. 页面不自行维护流程状态
3. 页面打开/关闭优先走 Controller 提供的统一接口
4. 页面内部只负责图标、动画、文案、显隐等表现层逻辑

### 6.3 新交互对象开发规范

新增交互对象时，应遵循：

1. 对象挂 `UCaissonInteractComponent`
2. 由 Controller 统一做点击检测
3. 命中后通过组件广播通知蓝图做表现
4. 不在 UMG 里重新做交互判定

## 7. 蓝图中允许与不允许的模式

### 7.1 可以保留在蓝图中的典型节点

以下节点大体属于合理的表现层逻辑：

- `PlayAnimation`
- `SetBrushFromTexture`
- `SetVisibility`
- 资源切换
- 按钮点击后调用 C++ 暴露的方法
- 目标对象的悬停与点亮表现
- 世界背景板和模型资源接线

### 7.2 应高度警惕的模式

如果蓝图中再次出现以下模式，通常意味着架构在倒退：

- `LineTraceByChannel`
- `GetHitResultUnderCursor`
- 在 Widget 中保存并修改 `CurrentStep`
- 大量 `CreateWidget -> AddToViewport -> RemoveFromParent`
- 在 UMG 中绑定输入并处理输入
- 在蓝图里维护跨页面流程状态
- 同一状态在 C++ 和蓝图中各维护一份

## 8. 推荐的后续开发思路

### 8.1 以后优先怎么写

今后增加功能时，默认思路应是：

- 先写 C++ 类或扩展现有 C++ 类
- 用 `BlueprintCallable` / `BlueprintAssignable` 暴露最小接口
- 蓝图只接资源和表现

### 8.2 不要再做的事

以下做法不应再成为默认开发方式：

- “先在蓝图里写通，之后再迁移”
- “Widget 自己推进流程状态”
- “页面直接打开/关闭其他页面”
- “临时在 UMG 里做输入或交互”

这类做法会再次把工程带回难以维护、难以让 AI 稳定接管的状态。

## 9. 提交前自检清单

提交前建议至少确认以下几点：

- 是否把核心流程写回了蓝图
- 是否新增了重复状态变量
- 是否让 Widget 直接管理其他 Widget 生命周期
- 是否把交互检测重新写回 UMG
- 是否保留了无用的禁用节点、调试打印和孤儿节点
- 是否误提交了本地工程产物

## 10. 当前结论

当前分支已经完成了主线重构：项目的核心驱动已经转移到 C++。

同时，展示架构也已经从“UI 中央显示 RenderTarget 模型”切换为“真实世界中的藻井模型 + 世界背景板 + UI 叠加说明”的方式。

在此基础上，`Level2` 当前也已经形成了：

- C++ 维护真实点击、顺序和流程状态
- `W_Level2` 承接介绍 UI、步骤 UI 和回位完成回调
- `BP_ShowcaseModel` 承接镜头拉近和回原位演出

同时，`Level3` 第一阶段当前也已经建立了新的独立代码入口，并且已经接通到真实蓝图：

- `ACaissonPlayerController` 只负责输入宿主与转发
- `ULevel3FlowComponent` 维护 `Level3` 真状态
- `ULevel3RepairAreaComponent` 负责区域命中与覆盖率
- `ALevel3RepairRegionActor` 作为 UE 蓝图摆放入口

当前这条链路已经进一步落地为：

- `W_Level3_Introdection -> W_Level3` 页面入口
- `W_Level3` 中 5 个工具按钮互斥选择
- 左键点击修复区域一次，结算一次工具效果
- 三维数值当前支持 `-2 ~ 2`
- 左下角数值、进度条和圆点已实时刷新
- `BP_ShowcaseModel_Level3.UpdateDustVisual` 当前先使用占位 `PrintString` 调试
- 真实灰尘材质待后续接入

这意味着后续 `Level3` 不需要复用 `Level2` 的 `CurrentStep / RequiredLevel2TargetIds / ELevel2FlowState` 逻辑。

后续工作的重点不是“继续机械地把更多蓝图搬到 C++”，而是：

- 保持边界不退化
- 在需要时继续扩展 C++ 公共接口
- 让蓝图保持轻量、稳定、可读、可替换

只要后续开发继续遵循这份文档，项目就能稳定地沿着“C++ 核心驱动 + 蓝图表现层”的方向演进。
