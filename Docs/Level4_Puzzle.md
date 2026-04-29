# Level4 拼图实现说明

本文档记录当前 `Level4` 拼图关卡的已确认实现、C++/蓝图职责边界、运行时流程和后续维护入口。蓝图自动导出的真实接线仍以 `Docs/Blueprints/W_Level4.md`、`Docs/Blueprints/BP_ShowcaseModel_Level4.md` 及 `Docs/BlueprintExports/` 下对应 `json` 为准。

## 1. 当前状态

当前 `Level4` 已进入可完整体验的拼图关卡状态：

1. `W_Level4_Introdection` 负责生成或复用 `BP_ShowcaseModel_Level4`，并切换玩家控制到该展示 Pawn。
2. `BP_ShowcaseModel_Level4` 是第四关展示 Pawn，当前挂载 `Level4PuzzleComponent`，作为拼图运行时组件宿主。
3. `W_Level4` 是拼图主 UI，负责难度按钮、碎片按钮、阶段完成提示和事件绑定。
4. `Level4PuzzleComponent` 负责真实拼图状态、碎片生成、拖拽、旋转、吸附、相对位置判定、阶段流转和完成广播。
5. 当前已验收：难度选择、碎片生成、拖拽、右键选择/旋转、吸附、阶段完成提示、普通/专家流程差异。

## 2. C++ 职责

核心类型位于 `Source/CaissonCeiling/`：

1. `ELevel4Difficulty`
   - `Normal`
   - `Expert`
2. `ELevel4StageId`
   - `CloudFrame1`
   - `CloudFrame2`
   - `StarMap`
   - `FinalAssembly`
3. `ELevel4PuzzlePhase`
   - `Inactive`
   - `Playing`
   - `StageCompleted`
   - `Completed`
4. `ULevel4PuzzleComponent`
   - 运行时状态机
   - 关卡阶段配置读取
   - 碎片生成与选择
   - 鼠标命中检测
   - 拖拽与右键旋转
   - 松手吸附
   - 拼图完成判定
   - 阶段完成与最终完成事件
5. `ALevel4PuzzlePieceActor`
   - 碎片 Actor 基类
   - 选中表现事件
   - 相对正确表现事件
6. `ALevel4PuzzleTargetLayoutActor`
   - 阶段目标布局配置壳层
   - 在蓝图中配置各碎片正确 `TargetTransform`

## 3. 当前运行规则

### 3.1 难度

1. 难度按钮可随时切换。
2. 如果进入关卡后还没有手动选择难度，第一次拖动、旋转或点击碎片按钮时会自动选择 `Normal`。
3. `Normal` 模式：
   - 新碎片生成在正确位置
   - 只随机 90 度旋转
   - 完成 `CloudFrame1` 后跳过 `CloudFrame2`
4. `Expert` 模式：
   - 新碎片生成到拼图宿主坐标原点
   - 仍随机 90 度旋转
   - 需要完成全部四个阶段

### 3.2 阶段

当前阶段顺序为：

1. `CloudFrame1`
2. `CloudFrame2`
3. `StarMap`
4. `FinalAssembly`

普通模式在进入下一阶段时通过 C++ 判断跳过 `CloudFrame2`。专家模式不跳过任何阶段。

### 3.3 交互

1. 左键必须命中真实已生成碎片，才能开始拖拽。
2. 左键点中非当前碎片时，会先选中该碎片并立刻开始拖拽。
3. 右键必须命中真实已生成碎片。
4. 右键点中非当前右键预选碎片时，只负责选中/预选。
5. 再次右键点中同一碎片时，围绕鼠标命中点旋转 90 度。
6. 按 `R` 会把当前选中碎片移动到正确位置，但保持当前旋转不变。
7. 阶段完成提示显示期间，拖拽、旋转、添加碎片和 `R` 归位都被禁止。

### 3.4 吸附和完成

1. 松开拖拽时，如果当前碎片与任意已生成碎片的相对位置和相对旋转在容差内，会直接吸附到 0 容差位置。
2. 任意两个或多个碎片相对正确时，对应碎片会进入“相对正确”表现。
3. 阶段完成要求当前阶段全部碎片都已生成，并且能相对同一个锚点全部对齐。
4. 阶段完成后不会立即切换下一阶段，而是进入 `StageCompleted`，广播 `OnLevel4StageSolved`。
5. 玩家点击 UI 中的继续按钮后，蓝图调用 `ContinueLevel4AfterStageSolved`，C++ 再进入下一阶段或广播最终完成。

## 4. 蓝图接线约定

### 4.1 `BP_ShowcaseModel_Level4`

当前职责：

1. 作为 `Level4` 的展示 Pawn。
2. 挂载 `Level4PuzzleComponent`。
3. 配置 `StageLayoutClasses`，顺序必须与阶段顺序一致：
   - `BP_L4_Layout_CloudFrame1`
   - `BP_L4_Layout_CloudFrame2`
   - `BP_L4_Layout_StarMap`
   - `BP_L4_Layout_FinalAssembly`
4. 配置固定相机视角。

维护要求：

1. 如果拼图阶段资源变化，优先更新对应 `BP_L4_Layout_*`。
2. 不要在 `BP_ShowcaseModel_Level4` 里重写拼图状态判定。
3. 交互判定保持在 `Level4PuzzleComponent` 中。

### 4.2 `W_Level4_Introdection`

当前职责：

1. 教学页阶段生成或复用 `BP_ShowcaseModel_Level4`。
2. 调用展示 Pawn 的激活逻辑。
3. 让 `CaissonPlayerController` possess 到 `BP_ShowcaseModel_Level4`。
4. 关闭教学页后打开 `W_Level4`。

### 4.3 `W_Level4`

当前职责：

1. 初始化时获取 `BP_ShowcaseModel_Level4` 上的 `Level4PuzzleComponent`，并保存为 `Level4PuzzleComponentRef`。
2. 直接通过 `Level4PuzzleComponentRef` 调用：
   - `StartLevel4Puzzle`
   - `SetLevel4Difficulty`
   - `SelectPiece`
   - `ContinueLevel4AfterStageSolved`
3. 不再把这些操作绕回 `CaissonPlayerController`，避免当前控制对象切换后找不到组件。
4. 绑定 `Level4PuzzleComponent` 事件：
   - `OnLevel4DifficultyChanged`
   - `OnLevel4StageChanged`
   - `OnLevel4PieceSelected`
   - `OnLevel4StageSolved`
   - `OnLevel4Completed`
5. `OnLevel4DifficultyChanged` 只负责刷新普通/专家难度高亮。
6. `OnLevel4PieceSelected` 只负责刷新碎片按钮选中高亮。
7. `OnLevel4StageSolved` 显示 `StageSolved` 完成提示。
8. `Continue_Next.OnClicked` 隐藏 `StageSolved`，然后调用 `ContinueLevel4AfterStageSolved`。

## 5. 配置和资产入口

当前关键蓝图文档：

1. `Docs/Blueprints/W_Level4_Introdection.md`
2. `Docs/Blueprints/W_Level4.md`
3. `Docs/Blueprints/BP_ShowcaseModel_Level4.md`
4. `Docs/Blueprints/BP_L4_Layout_CloudFrame1.md`
5. `Docs/Blueprints/BP_L4_Layout_CloudFrame2.md`
6. `Docs/Blueprints/BP_L4_Layout_StarMap.md`
7. `Docs/Blueprints/BP_L4_Layout_FinalAssembly.md`
8. `Docs/Blueprints/BP_L4_CloudFrame1_Piece1.md` 至 `BP_L4_CloudFrame1_Piece5.md`
9. `Docs/Blueprints/BP_L4_CloudFrame2_Piece1.md` 至 `BP_L4_CloudFrame2_Piece5.md`
10. `Docs/Blueprints/BP_L4_StarMap_Piece1.md` 至 `BP_L4_StarMap_Piece5.md`

当前关键 C++ 文件：

1. `Source/CaissonCeiling/Level4Types.h`
2. `Source/CaissonCeiling/Level4PuzzleComponent.h`
3. `Source/CaissonCeiling/Level4PuzzleComponent.cpp`
4. `Source/CaissonCeiling/Level4PuzzlePieceActor.h`
5. `Source/CaissonCeiling/Level4PuzzlePieceActor.cpp`
6. `Source/CaissonCeiling/Level4PuzzleTargetLayoutActor.h`
7. `Source/CaissonCeiling/Level4PuzzleTargetLayoutActor.cpp`
8. `Source/CaissonCeiling/CaissonPlayerController.h`
9. `Source/CaissonCeiling/CaissonPlayerController.cpp`

## 6. 验收清单

每次改动 `Level4` 后至少验证：

1. 进入教学页后能生成或复用 `BP_ShowcaseModel_Level4`。
2. 进入 `W_Level4` 后默认生成碎片 1。
3. 不手动选难度，点击碎片按钮或操作碎片会自动选中 `Normal`。
4. 普通按钮点击后只显示普通高亮。
5. 专家按钮点击后只显示专家高亮。
6. 普通模式新增碎片只随机旋转，位置在目标位置。
7. 专家模式新增碎片生成到拼图宿主原点。
8. 左键只能拖动命中的已生成碎片。
9. 右键第一次点中碎片只选中，第二次点同一碎片才旋转。
10. 右键旋转围绕鼠标命中点。
11. 松手后满足容差会吸附。
12. 阶段完成后显示 `StageSolved`，不会立即切换下一阶段。
13. 点击 `Continue_Next` 后才进入下一阶段。
14. 普通模式完成 `CloudFrame1` 后跳过 `CloudFrame2`。
15. 专家模式需要完成全部四个阶段。
16. 最后一阶段完成后触发 `OnLevel4Completed`。

## 7. 后续维护注意事项

1. `Docs/Blueprints/*.md` 和 `Docs/BlueprintExports/*.json` 是蓝图导出结果，不建议手写修改后长期依赖；蓝图变更后应重新导出再运行生成脚本。
2. 本文档是人工维护的 `Level4` 设计与接线说明，蓝图导出更新后应同步检查本文档是否需要调整。
3. 如果 `W_Level4` 再次出现难度 UI 不刷新，优先检查 `Level4PuzzleComponentRef` 是否有效，以及是否直接调用组件而不是绕回 Controller。
4. 如果碎片点击错选，优先检查碎片蓝图的碰撞、`GetDragPrimitive` 命中组件和 C++ piece-only trace 日志。
5. 如果阶段无法完成，优先查看 `Level4PuzzleComponent` 的 `Evaluate`、`Snap` 日志，确认全部碎片是否生成且是否相对同一锚点对齐。
