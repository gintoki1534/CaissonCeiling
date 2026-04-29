# W_Level4

## 1. 蓝图定位

本文档由 Docs/BlueprintExports/W_Level4.json 自动整理生成，用于记录当前蓝图的真实导出结构。

## 2. 基础信息

1. 蓝图名：W_Level4
1. 资源路径：/Script/UMGEditor.WidgetBlueprint'/Game/UI/W_Level4.W_Level4'
1. 蓝图类型：WidgetBlueprint
1. 父类：UMG.UserWidget
1. 生成类：W_Level4_C

## 3. 当前状态

1. 本文档基于蓝图导出数据自动生成。
1. 事件链、控件树、变量和特殊设置以导出内容为准。
1. 如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。

## 4. 控件树

1. 根控件：CanvasPanel_30

```text
pices_5_selected [Image] | parent=picies_5 | children=无
Image_5 [Image] | parent=picies_5 | children=无
pices_4_selected [Image] | parent=picies_4 | children=无
Image_4 [Image] | parent=picies_4 | children=无
pices_3_selected [Image] | parent=picies_3 | children=无
Image_1 [Image] | parent=picies_3 | children=无
pices_2_selected [Image] | parent=picies_2 | children=无
Image_0 [Image] | parent=picies_2 | children=无
pices_1_selected [Image] | parent=picies_1 | children=无
Image_3 [Image] | parent=picies_1 | children=无
Image_104 [Image] | parent=Difficuty_selected_normal | children=无
Image_102 [Image] | parent=Difficuty_selected_normal | children=无
Image_6 [Image] | parent=Difficuty_selected_expert | children=无
Image_2 [Image] | parent=Difficuty_selected_expert | children=无
DifficultyBase [Image] | parent=CanvasPanel_30 | children=无
StepBase [Image] | parent=CanvasPanel_30 | children=无
difficuty_base_expert [Image] | parent=CanvasPanel_30 | children=无
difficuty_base_normal [Image] | parent=CanvasPanel_30 | children=无
PiciesBase [Image] | parent=CanvasPanel_30 | children=无
Difficuty_selected_normal [CanvasPanel] | parent=CanvasPanel_30 | children=Image_104, Image_102
Difficuty_selected_expert [CanvasPanel] | parent=CanvasPanel_30 | children=Image_6, Image_2
picies_5 [CanvasPanel] | parent=CanvasPanel_30 | children=Image_5, pices_5_selected, pices_5_button
picies_4 [CanvasPanel] | parent=CanvasPanel_30 | children=Image_4, pices_4_selected, pices_4_button
picies_3 [CanvasPanel] | parent=CanvasPanel_30 | children=Image_1, pices_3_selected, pices_3_button
picies_2 [CanvasPanel] | parent=CanvasPanel_30 | children=Image_0, pices_2_selected, pices_2_button
picies_1 [CanvasPanel] | parent=CanvasPanel_30 | children=Image_3, pices_1_selected, pices_1_button
CanvasPanel_30 [CanvasPanel] | parent=ROOT | children=Difficuty_selected_expert, Difficuty_selected_normal, difficuty_base_expert, difficuty_base_normal, picies_5, picies_4, picies_3, picies_2, picies_1, PiciesBase, DifficultyBase, StepBase, difficuty_button_normal, difficuty_button_expert
pices_5_button [Button] | parent=picies_5 | children=无
pices_4_button [Button] | parent=picies_4 | children=无
pices_3_button [Button] | parent=picies_3 | children=无
pices_2_button [Button] | parent=picies_2 | children=无
pices_1_button [Button] | parent=picies_1 | children=无
difficuty_button_normal [Button] | parent=CanvasPanel_30 | children=无
difficuty_button_expert [Button] | parent=CanvasPanel_30 | children=无
```

## 5. 事件图主链

### 5.1 组件事件

1. pices_5_button -> OnClicked
1. pices_4_button -> OnClicked
1. pices_3_button -> OnClicked
1. pices_2_button -> OnClicked
1. pices_1_button -> OnClicked
1. difficuty_button_expert -> OnClicked
1. difficuty_button_normal -> OnClicked

### 5.2 主执行链

1. HandleLevel4Completed -> KismetSystemLibrary.PrintString
1. HandleLevel4StageChanged -> SetVisibility -> Switch on ELevel4StageId
1. HandleLevel4PieceSelected -> SetVisibility -> K2Node_SwitchInteger_0 -> SetVisibility
1. pices_5_button.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.SelectLevel4Piece
1. pices_4_button.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.SelectLevel4Piece
1. pices_3_button.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.SelectLevel4Piece
1. pices_2_button.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.SelectLevel4Piece
1. pices_1_button.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.SelectLevel4Piece
1. difficuty_button_expert.OnClicked -> Cast To CaissonPlayerController -> K2Node_IfThenElse_0 -> CaissonPlayerController.SetLevel4Difficulty -> SetVisibility -> SetVisibility
1. difficuty_button_expert.OnClicked -> Cast To CaissonPlayerController -> K2Node_IfThenElse_0 -> CaissonPlayerController.StartLevel4Puzzle -> Set bLevel4Started -> SetVisibility -> SetVisibility
1. difficuty_button_normal.OnClicked -> Cast To CaissonPlayerController -> K2Node_IfThenElse_1 -> CaissonPlayerController.SetLevel4Difficulty -> SetVisibility -> SetVisibility
1. difficuty_button_normal.OnClicked -> Cast To CaissonPlayerController -> K2Node_IfThenElse_1 -> CaissonPlayerController.StartLevel4Puzzle -> Set bLevel4Started -> SetVisibility -> SetVisibility
1. Construct -> SetVisibility -> Cast To CaissonPlayerController -> CaissonPlayerController.StartLevel4Puzzle -> Set bLevel4Started -> K2Node_AddDelegate_0 -> K2Node_AddDelegate_1 -> K2Node_AddDelegate_2
1. PreConstruct

## 6. 关键变量

### 6.1 自定义变量

1. bLevel4Started
   类型：bool
   显示名：Level 4Started

### 6.2 自动生成的控件变量

1. pices_5_selected -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. pices_5_button -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. pices_4_selected -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. pices_4_button -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. pices_3_selected -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. pices_3_button -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. pices_2_selected -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. pices_2_button -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. pices_1_selected -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. pices_1_button -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Difficuty_selected_normal -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. Difficuty_selected_expert -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. difficuty_button_normal -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. difficuty_button_expert -> /Script/CoreUObject.Class'/Script/UMG.Button'

## 7. 特殊设置

1. pices_5_selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/点击特效_方形）.点击特效_方形）'")
1. Image_5.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/碎片6.碎片6'")
1. pices_4_selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/点击特效_方形）.点击特效_方形）'")
1. Image_4.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/碎片5.碎片5'")
1. pices_3_selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/点击特效_方形）.点击特效_方形）'")
1. Image_1.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/碎片3.碎片3'")
1. pices_2_selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/点击特效_方形）.点击特效_方形）'")
1. Image_0.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/碎片2_1.碎片2_1'")
1. pices_1_selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/点击特效_方形）.点击特效_方形）'")
1. Image_3.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/碎片1.碎片1'")
1. Image_104.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/当前选择.当前选择'")
1. Image_102.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/圆环选中png.圆环选中png'")
1. Image_6.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/当前选择.当前选择'")
1. Image_2.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/圆环选中png.圆环选中png'")
1. DifficultyBase.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/难度选择底框.难度选择底框'")
1. StepBase.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/左上角.左上角'")
1. difficuty_base_expert.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/圆环未选中.圆环未选中'")
1. difficuty_base_normal.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/圆环未选中.圆环未选中'")
1. PiciesBase.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/右侧碎片列表底框.右侧碎片列表底框'")
1. pices_5_button.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_5_button.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_4_button.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_4_button.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_3_button.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_3_button.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_2_button.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_2_button.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_1_button.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_1_button.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. difficuty_button_normal.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. difficuty_button_normal.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. difficuty_button_expert.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. difficuty_button_expert.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. Image_104.LayoutData = (Offsets=(Left=10.000000,Top=140.000000,Bottom=40.000000))
1. Image_102.LayoutData = (Offsets=(Top=-4.000000,Right=120.000000,Bottom=120.000000))
1. Image_6.LayoutData = (Offsets=(Left=10.000000,Top=144.000000,Bottom=40.000000))
1. Image_2.LayoutData = (Offsets=(Top=-4.000000,Right=120.000000,Bottom=120.000000))
1. Image_5.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_5_selected.LayoutData = (Offsets=(Left=-40.000000,Top=-44.000000,Right=200.000000,Bottom=200.000000))
1. pices_5_button.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. Image_4.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_4_selected.LayoutData = (Offsets=(Left=-40.000000,Top=-40.000000,Right=200.000000,Bottom=200.000000))
1. pices_4_button.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. Image_1.LayoutData = (Offsets=(Left=-16.000000,Top=-16.000000,Right=150.000000,Bottom=150.000000))
1. pices_3_selected.LayoutData = (Offsets=(Left=-40.000000,Top=-40.000000,Right=200.000000,Bottom=200.000000))
1. pices_3_button.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. Image_0.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_2_selected.LayoutData = (Offsets=(Left=-40.000000,Top=-40.000000,Right=200.000000,Bottom=200.000000))
1. pices_2_button.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. Image_3.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_1_selected.LayoutData = (Offsets=(Left=-40.000000,Top=-40.000000,Right=200.000000,Bottom=200.000000))
1. pices_1_button.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. Difficuty_selected_expert.LayoutData = (Offsets=(Left=280.000000,Top=644.000000,Right=120.000000,Bottom=200.000000))
1. Difficuty_selected_normal.LayoutData = (Offsets=(Left=116.000000,Top=648.000000,Right=120.000000,Bottom=200.000000))
1. difficuty_base_expert.LayoutData = (Offsets=(Left=280.000000,Top=640.000000,Right=120.000000,Bottom=120.000000))
1. difficuty_base_normal.LayoutData = (Offsets=(Left=116.000000,Top=644.000000,Right=120.000000,Bottom=120.000000))
1. picies_5.LayoutData = (Offsets=(Left=1540.000000,Top=632.000000,Right=120.000000,Bottom=120.000000))
1. picies_4.LayoutData = (Offsets=(Left=1708.000000,Top=468.000000,Right=120.000000,Bottom=120.000000))
1. picies_3.LayoutData = (Offsets=(Left=1540.000000,Top=472.000000,Right=120.000000,Bottom=120.000000))
1. picies_2.LayoutData = (Offsets=(Left=1708.000000,Top=304.000000,Right=120.000000,Bottom=120.000000))
1. picies_1.LayoutData = (Offsets=(Left=1540.000000,Top=304.000000,Right=120.000000,Bottom=120.000000))
1. PiciesBase.LayoutData = (Offsets=(Left=1448.000000,Top=132.000000,Right=452.841705,Bottom=790.261353))
1. DifficultyBase.LayoutData = (Offsets=(Left=64.000000,Top=532.000000,Right=391.491486,Bottom=330.779663))
1. StepBase.LayoutData = (Offsets=(Left=12.000000,Top=88.000000,Right=525.911621,Bottom=429.189178))
1. difficuty_button_normal.LayoutData = (Offsets=(Left=116.000000,Top=644.000000,Right=120.000000,Bottom=120.000000))
1. difficuty_button_expert.LayoutData = (Offsets=(Left=280.000000,Top=640.000000,Right=120.000000,Bottom=120.000000))

## 8. 其他图表

### 8.1 BndEvt__W_Level4_difficuty_button_expert_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_difficuty_button_expert_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_difficuty_button_expert_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature:then
   - BndEvt__W_Level4_difficuty_button_expert_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature

### 8.2 BndEvt__W_Level4_difficuty_button_normal_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_difficuty_button_normal_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_difficuty_button_normal_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature:then
   - BndEvt__W_Level4_difficuty_button_normal_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature

### 8.3 BndEvt__W_Level4_pices_1_button_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_pices_1_button_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_pices_1_button_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature:then
   - BndEvt__W_Level4_pices_1_button_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature

### 8.4 BndEvt__W_Level4_pices_2_button_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_pices_2_button_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_pices_2_button_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature:then
   - BndEvt__W_Level4_pices_2_button_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature

### 8.5 BndEvt__W_Level4_pices_3_button_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_pices_3_button_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_pices_3_button_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature:then
   - BndEvt__W_Level4_pices_3_button_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature

### 8.6 BndEvt__W_Level4_pices_4_button_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_pices_4_button_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_pices_4_button_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature:then
   - BndEvt__W_Level4_pices_4_button_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature

### 8.7 BndEvt__W_Level4_pices_5_button_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_pices_5_button_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_pices_5_button_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature:then
   - BndEvt__W_Level4_pices_5_button_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature

### 8.8 Construct

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：Construct -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- Construct:then
   - Construct

### 8.9 HandleLevel4Completed

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：HandleLevel4Completed -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- HandleLevel4Completed:then
   - HandleLevel4Completed

### 8.10 HandleLevel4PieceSelected

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：HandleLevel4PieceSelected -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- SetVariableOnPersistentFrame:then
   - HandleLevel4PieceSelected

### 8.11 HandleLevel4StageChanged

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：4
1. 执行链：HandleLevel4StageChanged -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- SetVariableOnPersistentFrame:then
   - HandleLevel4StageChanged

### 8.12 PreConstruct

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：PreConstruct -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - ExecuteUbergraph_W_Level4
     输入：execute <- SetVariableOnPersistentFrame:then
   - PreConstruct

## 9. 当前可直接确认的结论

1. 本蓝图至少存在上文列出的执行链，可作为后续人工确认和接线补全文档的基础。
1. Widget 树已经从导出中整理出来，后续可以继续补充哪些控件参与了实际逻辑。
1. 变量区已经整理完成，后续重点是确认哪些变量仍然在真实运行链中使用。

## 10. 后续补充建议

1. 如果该蓝图已经过运行验证，建议补充一节“已运行验证链路”。
1. 如果导出中存在历史残留变量、残留事件或旧链路，建议后续在本文档中手动标记。
1. 如果本蓝图和别的蓝图存在强关联，建议在文档中补“对外依赖”。
