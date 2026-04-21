# W_Level1

## 1. 蓝图定位

本文档由 Docs/BlueprintExports/W_Level1.json 自动整理生成，用于记录当前蓝图的真实导出结构。

## 2. 基础信息

1. 蓝图名：W_Level1
1. 资源路径：/Script/UMGEditor.WidgetBlueprint'/Game/UI/W_Level1.W_Level1'
1. 蓝图类型：WidgetBlueprint
1. 父类：UMG.UserWidget
1. 生成类：W_Level1_C
1. Tick 预测：WillTick
1. Tick 原因：Script

## 3. 当前状态

1. 本文档基于蓝图导出数据自动生成。
1. 事件链、控件树、变量和特殊设置以导出内容为准。
1. 如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。

## 4. 控件树

1. 根控件：CanvasPanel_38

```text
TextBlock_0 [TextBlock] | parent=RollBox | children=无
TalkBox [Image] | parent=CanvasPanel_38 | children=无
RollBox [ScrollBox] | parent=CanvasPanel_38 | children=TextBlock_0
CanvasPanel_38 [CanvasPanel] | parent=ROOT | children=TalkBox, RollBox, Btn_Dialogue
Btn_Dialogue [Button] | parent=CanvasPanel_38 | children=无
```

## 5. 事件图主链

### 5.1 组件事件

1. Btn_Dialogue -> OnClicked

### 5.2 主执行链

1. Btn_Dialogue.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.CloseCaissonWidget -> CaissonPlayerController.OpenCaissonWidget
1. OnFocusLost
1. Tick
1. Construct -> SpawnActor BP_ShowcaseModel_C -> Set ShowcaseActor
1. PreConstruct

## 6. 关键变量

### 6.1 自定义变量

1. ShowcaseActor
   类型：object -> /Script/Engine.BlueprintGeneratedClass'/Game/UI/BP_ShowcaseModel_BC2.BP_ShowcaseModel_BC2_C'
   显示名：Showcase Actor
1. bIsDragging
   类型：bool
   显示名：Is Dragging
1. location
   类型：int
   显示名：Location
1. CurrentYaw
   类型：real/double
   显示名：Current Yaw
1. CurrentPitch
   类型：real/double
   显示名：Current Pitch
1. VelocityX
   类型：real/double
   显示名：Velocity X
1. VelocityY
   类型：real/double
   显示名：Velocity Y

### 6.2 自动生成的控件变量

1. TalkBox -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. RollBox -> /Script/CoreUObject.Class'/Script/UMG.ScrollBox'
1. Btn_Dialogue -> /Script/CoreUObject.Class'/Script/UMG.Button'

## 7. 特殊设置

1. W_Level1.TickPrediction = WillTick
   原因：Script
1. TextBlock_0.Text = NSLOCTEXT("[54EE08DFC892F2E9E1C6A089C6A3073A]", "A16BE1AA46D033C3B70D5AA3BB947A9A", "欢迎来到王者荣耀!!欢迎来到王者荣耀!!欢迎来到王者荣耀!!欢迎来到王者荣耀!!欢迎来到王者荣耀欢迎来到王者荣耀!!欢迎来到王者荣耀欢迎来到王者荣耀!!欢迎来到王者荣耀")
1. TextBlock_0.ColorAndOpacity = (SpecifiedColor=(R=0.162029,G=0.114435,B=0.034340,A=1.000000))
1. TextBlock_0.AutoWrapText = True
1. TalkBox.Brush = (ImageType=FullColor,ImageSize=(X=1844.000000,Y=581.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/对话框底部.对话框底部'")
1. Btn_Dialogue.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. TalkBox.LayoutData = (Offsets=(Top=-329.081055,Right=992.320862,Bottom=299.688263),Anchors=(Minimum=(X=0.500000,Y=1.000000),Maximum=(X=0.500000,Y=1.000000)),Alignment=(X=0.500000,Y=0.000000))
1. RollBox.LayoutData = (Offsets=(Left=768.000000,Top=848.000000,Right=625.096497,Bottom=137.240097))
1. Btn_Dialogue.LayoutData = (Offsets=(Left=465.537537,Top=786.630615,Right=1002.388062,Bottom=263.080200))

## 8. 其他图表

### 8.1 BndEvt__W_Level1_Btn_Dialogue_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level1_Btn_Dialogue_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level1
1. 关键节点：
   - ExecuteUbergraph_W_Level1
     输入：execute <- BndEvt__W_Level1_Btn_Dialogue_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature:then
   - BndEvt__W_Level1_Btn_Dialogue_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature

### 8.2 Construct

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：Construct -> ExecuteUbergraph_W_Level1
1. 关键节点：
   - ExecuteUbergraph_W_Level1
     输入：execute <- Construct:then
   - Construct

### 8.3 OnFocusLost

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：OnFocusLost -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level1
1. 关键节点：
   - ExecuteUbergraph_W_Level1
     输入：execute <- SetVariableOnPersistentFrame:then
   - OnFocusLost

### 8.4 PreConstruct

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：PreConstruct -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level1
1. 关键节点：
   - ExecuteUbergraph_W_Level1
     输入：execute <- SetVariableOnPersistentFrame:then
   - PreConstruct

### 8.5 Tick

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：4
1. 执行链：Tick -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level1
1. 关键节点：
   - ExecuteUbergraph_W_Level1
     输入：execute <- SetVariableOnPersistentFrame:then
   - Tick

### 8.6 新函数

1. 图类型：UMGEditor.WidgetGraphSchema
1. 模块数：1
1. 连接数：0
1. 执行链：新函数
1. 关键节点：
   - 新函数

## 9. 当前可直接确认的结论

1. 本蓝图至少存在上文列出的执行链，可作为后续人工确认和接线补全文档的基础。
1. Widget 树已经从导出中整理出来，后续可以继续补充哪些控件参与了实际逻辑。
1. 变量区已经整理完成，后续重点是确认哪些变量仍然在真实运行链中使用。

## 10. 后续补充建议

1. 如果该蓝图已经过运行验证，建议补充一节“已运行验证链路”。
1. 如果导出中存在历史残留变量、残留事件或旧链路，建议后续在本文档中手动标记。
1. 如果本蓝图和别的蓝图存在强关联，建议在文档中补“对外依赖”。
