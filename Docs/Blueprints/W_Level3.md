# W_Level3

## 1. 蓝图定位

本文档由 Docs/BlueprintExports/W_Level3.json 自动整理生成，用于记录当前蓝图的真实导出结构。

## 2. 基础信息

1. 蓝图名：W_Level3
1. 资源路径：/Script/UMGEditor.WidgetBlueprint'/Game/UI/W_Level3.W_Level3'
1. 蓝图类型：WidgetBlueprint
1. 父类：UMG.UserWidget
1. 生成类：W_Level3_C

## 3. 当前状态

1. 本文档基于蓝图导出数据自动生成。
1. 事件链、控件树、变量和特殊设置以导出内容为准。
1. 如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。

## 4. 控件树

1. 根控件：CanvasPanel_30

```text
Text_Result [TextBlock] | parent=CanvasPanel_Result | children=无
barbase2 [Image] | parent=CanvasPanel_30 | children=无
barbase1 [Image] | parent=CanvasPanel_30 | children=无
barbase [Image] | parent=CanvasPanel_30 | children=无
uibase [Image] | parent=CanvasPanel_30 | children=无
TextBlock_2 [TextBlock] | parent=CanvasPanel_30 | children=无
Text_AestheticsValue [TextBlock] | parent=CanvasPanel_30 | children=无
Text_IntegrityValue [TextBlock] | parent=CanvasPanel_30 | children=无
Text_CleanValue [TextBlock] | parent=CanvasPanel_30 | children=无
Tools_1_Selected [Image] | parent=CanvasPanel_30 | children=无
Tools_2_Selected [Image] | parent=CanvasPanel_30 | children=无
Tools_3_Selected [Image] | parent=CanvasPanel_30 | children=无
Tools_4_Selected [Image] | parent=CanvasPanel_30 | children=无
Tools_5_Unselected [Image] | parent=CanvasPanel_30 | children=无
Tools_4_Unselected [Image] | parent=CanvasPanel_30 | children=无
Tools_3_Unselected [Image] | parent=CanvasPanel_30 | children=无
Tools_2_Unselected [Image] | parent=CanvasPanel_30 | children=无
Tools_1_Unselected [Image] | parent=CanvasPanel_30 | children=无
Tools_5_Selected [Image] | parent=CanvasPanel_30 | children=无
Image_11 [Image] | parent=CanvasPanel_0 | children=无
Image_10 [Image] | parent=CanvasPanel_0 | children=无
Image_9 [Image] | parent=CanvasPanel_0 | children=无
Image_8 [Image] | parent=CanvasPanel_0 | children=无
tools [Image] | parent=CanvasPanel_0 | children=无
tools_text_9 [TextBlock] | parent=CanvasPanel_0 | children=无
tools_text_8 [TextBlock] | parent=CanvasPanel_0 | children=无
tools_text_7 [TextBlock] | parent=CanvasPanel_0 | children=无
tools_text_6 [TextBlock] | parent=CanvasPanel_0 | children=无
tools_text_5 [TextBlock] | parent=CanvasPanel_0 | children=无
tools_text_4 [TextBlock] | parent=CanvasPanel_0 | children=无
tools_text_3 [TextBlock] | parent=CanvasPanel_0 | children=无
tools_text_1 [TextBlock] | parent=CanvasPanel_0 | children=无
tools_text [TextBlock] | parent=CanvasPanel_0 | children=无
clean_4 [Image] | parent=CanvasPanel_0 | children=无
clean_3 [Image] | parent=CanvasPanel_0 | children=无
clean_2 [Image] | parent=CanvasPanel_0 | children=无
clean_1 [Image] | parent=CanvasPanel_0 | children=无
clean [Image] | parent=CanvasPanel_0 | children=无
Perfect_2 [Image] | parent=CanvasPanel_0 | children=无
Perfect_1 [Image] | parent=CanvasPanel_0 | children=无
Perfect [Image] | parent=CanvasPanel_0 | children=无
beauty [Image] | parent=CanvasPanel_0 | children=无
beauty_1 [Image] | parent=CanvasPanel_0 | children=无
tools_text_2 [TextBlock] | parent=CanvasPanel_0 | children=无
CanvasPanel_0 [CanvasPanel] | parent=CanvasPanel_30 | children=Image_11, Image_10, Image_9, Image_8, tools, tools_text_9, tools_text_8, tools_text_7, tools_text_6, tools_text_5, tools_text_4, tools_text_3, tools_text_2, tools_text_1, tools_text, clean_4, clean_3, clean_2, clean_1, clean, Perfect_2, Perfect_1, Perfect, beauty, beauty_1
CanvasPanel_Result [CanvasPanel] | parent=CanvasPanel_30 | children=Text_Result, Button_Continue
CanvasPanel_30 [CanvasPanel] | parent=ROOT | children=TextBlock_2, CanvasPanel_Result, Text_AestheticsValue, Text_IntegrityValue, Text_CleanValue, barbase2, barbase1, barbase, Tools_1_Selected, Tools_2_Selected, Tools_3_Selected, Tools_4_Selected, Tools_5_Selected, Tools_5_Unselected, Tools_4_Unselected, Tools_3_Unselected, Tools_2_Unselected, Tools_1_Unselected, uibase, CanvasPanel_0, Button_Tool5, Button_Tool4, Button_Tool3, Button_Tool2, Button_Tool1, Button_0, Button
Button_Tool5 [Button] | parent=CanvasPanel_30 | children=无
Button_Tool4 [Button] | parent=CanvasPanel_30 | children=无
Button_Tool3 [Button] | parent=CanvasPanel_30 | children=无
Button_Tool2 [Button] | parent=CanvasPanel_30 | children=无
Button_Tool1 [Button] | parent=CanvasPanel_30 | children=无
Button_Continue [Button] | parent=CanvasPanel_Result | children=无
Button_0 [Button] | parent=CanvasPanel_30 | children=无
Button [Button] | parent=CanvasPanel_30 | children=无
```

## 5. 事件图主链

### 5.1 组件事件

1. Button_Tool5 -> OnClicked
1. Button_Tool4 -> OnClicked
1. Button_Tool3 -> OnClicked
1. Button_Tool2 -> OnClicked
1. Button_Tool1 -> OnClicked
1. Button -> OnClicked
1. Button_0 -> OnClicked
1. Button_Continue -> OnClicked

### 5.2 主执行链

1. Button_Tool5.OnClicked -> RequestSelectTool
1. Button_Tool4.OnClicked -> RequestSelectTool
1. Button_Tool3.OnClicked -> RequestSelectTool
1. Button_Tool2.OnClicked -> RequestSelectTool
1. Button_Tool1.OnClicked -> RequestSelectTool
1. Button.OnClicked
1. Button_0.OnClicked
1. Tick
1. Construct -> Cast To BP_ShowcaseModel_Level3_C -> Set Level3ShowcaseRef -> Cast To CaissonPlayerController -> Set CaissonControllerRef -> Set Level3FlowRef -> K2Node_MacroInstance_0 -> K2Node_AddDelegate_0 -> K2Node_AddDelegate_1 -> K2Node_AddDelegate_2 -> K2Node_AddDelegate_3 -> HandleLevel3PhaseChanged -> HandleLevel3ProgressChanged
1. PreConstruct
1. Button_Continue.OnClicked -> K2Node_MacroInstance_3 -> BP_ShowcaseModel_Level3_C.ReturnToLevel3DefaultView -> Set bRepairViewActivated -> K2Node_MacroInstance_4 -> CaissonPlayerController.CompleteLevel3ResultPresentation -> Set Visibility

## 6. 关键变量

### 6.1 自定义变量

1. Level3FlowRef
   类型：object -> /Script/CoreUObject.Class'/Script/CaissonCeiling.Level3FlowComponent'
   显示名：Level 3Flow Ref
1. CaissonControllerRef
   类型：object -> /Script/CoreUObject.Class'/Script/CaissonCeiling.CaissonPlayerController'
   显示名：Caisson Controller Ref
1. Level3ShowcaseRef
   类型：object -> /Script/Engine.BlueprintGeneratedClass'/Game/UI/BP_ShowcaseModel_Level3.BP_ShowcaseModel_Level3_C'
   显示名：Level 3Showcase Ref
1. bRepairViewActivated
   类型：bool
   显示名：Repair View Activated

### 6.2 自动生成的控件变量

1. Tools_5_Selected -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Tools_4_Selected -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Tools_3_Selected -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Tools_2_Selected -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Tools_1_Selected -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. TextBlock_2 -> /Script/CoreUObject.Class'/Script/UMG.TextBlock'
1. Text_IntegrityValue -> /Script/CoreUObject.Class'/Script/UMG.TextBlock'
1. Text_CleanValue -> /Script/CoreUObject.Class'/Script/UMG.TextBlock'
1. Text_AestheticsValue -> /Script/CoreUObject.Class'/Script/UMG.TextBlock'
1. CanvasPanel_Result -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. CanvasPanel_0 -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. Button_Tool5 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Button_Tool4 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Button_Tool3 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Button_Tool2 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Button_Tool1 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Button_Continue -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Button_0 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Button -> /Script/CoreUObject.Class'/Script/UMG.Button'

## 7. 特殊设置

1. Text_Result.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "A1F6B9AC420CC8E3B898EBA59F6D6625", "除尘完成")
1. barbase2.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/修复程度横线.修复程度横线'")
1. barbase1.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/修复程度横线.修复程度横线'")
1. barbase.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/修复程度横线.修复程度横线'")
1. uibase.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/图层_3.图层_3'")
1. TextBlock_2.Text = NSLOCTEXT("UMG", "TextBlockDefaultValue", "Text Block")
1. Text_AestheticsValue.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "1B438C92419C68D7494A319E9E0C0B82", "0")
1. Text_IntegrityValue.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "1B438C92419C68D7494A319E9E0C0B82", "0")
1. Text_CleanValue.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "1B438C92419C68D7494A319E9E0C0B82", "0")
1. Tools_1_Selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子选择状态背景.刷子选择状态背景'")
1. Tools_1_Selected.Visibility = Collapsed
1. Tools_2_Selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子选择状态背景.刷子选择状态背景'")
1. Tools_2_Selected.Visibility = Collapsed
1. Tools_3_Selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子选择状态背景.刷子选择状态背景'")
1. Tools_3_Selected.Visibility = Collapsed
1. Tools_4_Selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子选择状态背景.刷子选择状态背景'")
1. Tools_4_Selected.Visibility = Collapsed
1. Tools_5_Unselected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子未选择状态背景.刷子未选择状态背景'")
1. Tools_4_Unselected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子未选择状态背景.刷子未选择状态背景'")
1. Tools_3_Unselected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子未选择状态背景.刷子未选择状态背景'")
1. Tools_2_Unselected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子未选择状态背景.刷子未选择状态背景'")
1. Tools_1_Unselected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子未选择状态背景.刷子未选择状态背景'")
1. Tools_5_Selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子选择状态背景.刷子选择状态背景'")
1. Tools_5_Selected.Visibility = Collapsed
1. Image_11.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/墩头打磨油刷.墩头打磨油刷'")
1. Image_10.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/弯曲柄.弯曲柄'")
1. Image_9.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/极细勾线描刷.极细勾线描刷'")
1. Image_8.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/圆头除尘软刷.圆头除尘软刷'")
1. tools.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/宽口羊毛排刷.宽口羊毛排刷'")
1. tools_text_9.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "6BE8B7C043D441F0A4B2C488648D68B0", "1")
1. tools_text_8.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "6BE8B7C043D441F0A4B2C488648D68B0", "1")
1. tools_text_7.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "6BE8B7C043D441F0A4B2C488648D68B0", "1")
1. tools_text_6.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "6BE8B7C043D441F0A4B2C488648D68B0", "1")
1. tools_text_5.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "6BE8B7C043D441F0A4B2C488648D68B0", "1")
1. tools_text_4.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "6BE8B7C043D441F0A4B2C488648D68B0", "1")
1. tools_text_3.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "6BE8B7C043D441F0A4B2C488648D68B0", "1")
1. tools_text_1.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "6BE8B7C043D441F0A4B2C488648D68B0", "1")
1. tools_text.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "6BE8B7C043D441F0A4B2C488648D68B0", "1")
1. clean_4.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/清洁度ui.清洁度ui'")
1. clean_3.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/清洁度ui.清洁度ui'")
1. clean_2.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/清洁度ui.清洁度ui'")
1. clean_1.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/清洁度ui.清洁度ui'")
1. clean.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/清洁度ui.清洁度ui'")
1. Perfect_2.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/完好度ui_3.完好度ui_3'")
1. Perfect_1.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/完好度ui_3.完好度ui_3'")
1. Perfect.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/完好度ui_3.完好度ui_3'")
1. beauty.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/美观度ui_1.美观度ui_1'")
1. beauty_1.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/美观度ui_1.美观度ui_1'")
1. tools_text_2.Text = NSLOCTEXT("[BB8C6375CC95ABC679EF6811644C7252]", "6BE8B7C043D441F0A4B2C488648D68B0", "1")
1. CanvasPanel_0.Visibility = HitTestInvisible
1. CanvasPanel_Result.Visibility = Collapsed
1. Button_0.Visibility = Collapsed
1. Button.Visibility = Collapsed
1. Image_11.LayoutData = (Offsets=(Left=40.000000,Top=584.000000,Right=80.000000,Bottom=80.000000))
1. Image_10.LayoutData = (Offsets=(Left=40.000000,Top=472.000000,Right=80.000000,Bottom=80.000000))
1. Image_9.LayoutData = (Offsets=(Left=44.000000,Top=352.000000,Right=80.000000,Bottom=80.000000))
1. Image_8.LayoutData = (Offsets=(Left=40.000000,Top=240.000000,Right=80.000000,Bottom=80.000000))
1. tools.LayoutData = (Offsets=(Left=36.000000,Top=136.000000,Right=80.000000,Bottom=80.000000))
1. tools_text_9.LayoutData = (Offsets=(Left=272.000000,Top=620.000000,Right=18.000000,Bottom=18.000000))
1. tools_text_8.LayoutData = (Offsets=(Left=196.000000,Top=620.000000,Right=18.000000,Bottom=18.000000))
1. tools_text_7.LayoutData = (Offsets=(Left=276.000000,Top=508.000000,Right=18.000000,Bottom=18.000000))
1. tools_text_6.LayoutData = (Offsets=(Left=196.000000,Top=508.000000,Right=18.000000,Bottom=18.000000))
1. tools_text_5.LayoutData = (Offsets=(Left=272.000000,Top=396.000000,Right=18.000000,Bottom=18.000000))
1. tools_text_4.LayoutData = (Offsets=(Left=192.000000,Top=396.000000,Right=18.000000,Bottom=18.000000))
1. tools_text_3.LayoutData = (Offsets=(Left=272.000000,Top=284.000000,Right=18.000000,Bottom=18.000000))
1. tools_text_2.LayoutData = (Offsets=(Left=192.000000,Top=284.000000,Right=18.000000,Bottom=18.000000))
1. tools_text_1.LayoutData = (Offsets=(Left=272.000000,Top=172.000000,Right=18.000000,Bottom=18.000000))
1. tools_text.LayoutData = (Offsets=(Left=196.000000,Top=172.000000,Right=18.000000,Bottom=18.000000))
1. clean_4.LayoutData = (Offsets=(Left=244.000000,Top=284.000000,Right=20.000000,Bottom=20.000000))
1. clean_3.LayoutData = (Offsets=(Left=244.000000,Top=620.000000,Right=20.000000,Bottom=20.000000))
1. clean_2.LayoutData = (Offsets=(Left=244.000000,Top=508.000000,Right=20.000000,Bottom=20.000000))
1. clean_1.LayoutData = (Offsets=(Left=244.000000,Top=396.000000,Right=20.000000,Bottom=20.000000))
1. clean.LayoutData = (Offsets=(Left=244.000000,Top=172.000000,Right=20.000000,Bottom=20.000000))
1. Perfect_2.LayoutData = (Offsets=(Left=168.000000,Top=620.000000,Right=20.000000,Bottom=20.000000))
1. Perfect_1.LayoutData = (Offsets=(Left=168.000000,Top=508.000000,Right=20.000000,Bottom=20.000000))
1. Perfect.LayoutData = (Offsets=(Left=168.000000,Top=284.000000,Right=20.000000,Bottom=20.000000))
1. beauty.LayoutData = (Offsets=(Left=168.000000,Top=172.000000,Right=20.000000,Bottom=20.000000))
1. beauty_1.LayoutData = (Offsets=(Left=168.000000,Top=396.000000,Right=20.000000,Bottom=20.000000))
1. Text_Result.LayoutData = (Offsets=(Left=280.000000,Top=132.000000,Right=279.836975,Bottom=40.000000))
1. Button_Continue.LayoutData = (Offsets=(Left=288.000000,Top=344.000000,Bottom=40.000000))
1. TextBlock_2.LayoutData = (Offsets=(Left=748.000000,Top=136.000000,Bottom=40.000000))
1. CanvasPanel_Result.LayoutData = (Offsets=(Left=592.000000,Top=244.000000,Right=746.589417,Bottom=590.493286))
1. Text_AestheticsValue.LayoutData = (Offsets=(Left=168.000000,Top=808.000000,Bottom=40.000000))
1. Text_IntegrityValue.LayoutData = (Offsets=(Left=168.000000,Top=744.000000,Bottom=40.000000))
1. Text_CleanValue.LayoutData = (Offsets=(Left=168.000000,Top=676.000000,Bottom=40.000000))
1. barbase2.LayoutData = (Offsets=(Left=212.000000,Top=828.000000,Right=200.000000,Bottom=3.000000))
1. barbase1.LayoutData = (Offsets=(Left=212.000000,Top=760.000000,Right=200.000000,Bottom=3.000000))
1. barbase.LayoutData = (Offsets=(Left=212.000000,Top=696.000000,Right=200.000000,Bottom=3.000000))
1. Tools_1_Selected.LayoutData = (Offsets=(Left=1512.000000,Top=284.000000,Right=325.000000,Bottom=100.000000))
1. Tools_2_Selected.LayoutData = (Offsets=(Left=1512.000000,Top=396.000000,Right=325.000000,Bottom=100.000000))
1. Tools_3_Selected.LayoutData = (Offsets=(Left=1512.000000,Top=508.000000,Right=325.000000,Bottom=100.000000))
1. Tools_4_Selected.LayoutData = (Offsets=(Left=1512.000000,Top=620.000000,Right=325.000000,Bottom=100.000000))
1. Tools_5_Selected.LayoutData = (Offsets=(Left=1512.000000,Top=732.000000,Right=325.000000,Bottom=100.000000))
1. Tools_5_Unselected.LayoutData = (Offsets=(Left=1512.000000,Top=732.000000,Right=325.000000,Bottom=100.000000))
1. Tools_4_Unselected.LayoutData = (Offsets=(Left=1512.000000,Top=620.000000,Right=325.000000,Bottom=100.000000))
1. Tools_3_Unselected.LayoutData = (Offsets=(Left=1512.000000,Top=508.000000,Right=325.000000,Bottom=100.000000))
1. Tools_2_Unselected.LayoutData = (Offsets=(Left=1512.000000,Top=396.000000,Right=325.000000,Bottom=100.000000))
1. Tools_1_Unselected.LayoutData = (Offsets=(Left=1512.000000,Top=284.000000,Right=325.000000,Bottom=100.000000))
1. uibase.LayoutData = (Offsets=(Top=101.655945,Right=1900.000000,Bottom=872.112183))
1. CanvasPanel_0.LayoutData = (Offsets=(Left=1480.816406,Top=164.104416,Right=400.643524,Bottom=786.117554))
1. Button_Tool5.LayoutData = (Offsets=(Left=1512.000000,Top=732.000000,Right=325.000000,Bottom=100.000000))
1. Button_Tool4.LayoutData = (Offsets=(Left=1512.000000,Top=620.000000,Right=325.000000,Bottom=100.000000))
1. Button_Tool3.LayoutData = (Offsets=(Left=1512.000000,Top=508.000000,Right=325.000000,Bottom=100.000000))
1. Button_Tool2.LayoutData = (Offsets=(Left=1512.000000,Top=396.000000,Right=325.000000,Bottom=100.000000))
1. Button_Tool1.LayoutData = (Offsets=(Left=1512.000000,Top=284.000000,Right=325.000000,Bottom=100.000000))
1. Button_0.LayoutData = (Offsets=(Left=544.000000,Top=248.000000,Bottom=40.000000))
1. Button.LayoutData = (Offsets=(Left=540.000000,Top=332.000000,Bottom=40.000000))

## 8. 其他图表

### 8.1 BndEvt__W_Level3_Button_0_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level3_Button_0_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_FunctionEntry_0:CEE4621946F2D578731CE6B6F1D4CCFD
   - BndEvt__W_Level3_Button_0_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature

### 8.2 BndEvt__W_Level3_Button_Continue_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level3_Button_Continue_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_FunctionEntry_0:1DF2155E4DC160722A56158E8B30F6C2
   - BndEvt__W_Level3_Button_Continue_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature

### 8.3 BndEvt__W_Level3_Button_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level3_Button_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_FunctionEntry_0:82673351431D96D15DE5DD8EB29B43CE
   - BndEvt__W_Level3_Button_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature

### 8.4 BndEvt__W_Level3_Button_Tool1_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level3_Button_Tool1_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_FunctionEntry_0:AD890DDA4A015A0AA622B1AD93CF0490
   - BndEvt__W_Level3_Button_Tool1_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature

### 8.5 BndEvt__W_Level3_Button_Tool2_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level3_Button_Tool2_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_FunctionEntry_0:F7C9AD2540A5EB68B620FA91DB3DE68C
   - BndEvt__W_Level3_Button_Tool2_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature

### 8.6 BndEvt__W_Level3_Button_Tool3_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level3_Button_Tool3_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_FunctionEntry_0:8C6D56754F4F44844D742484F47F2073
   - BndEvt__W_Level3_Button_Tool3_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature

### 8.7 BndEvt__W_Level3_Button_Tool4_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level3_Button_Tool4_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_FunctionEntry_0:9471930E4236E21EB2694CB7AC995107
   - BndEvt__W_Level3_Button_Tool4_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature

### 8.8 BndEvt__W_Level3_Button_Tool5_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level3_Button_Tool5_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_FunctionEntry_0:C37781B64B45E5EB9E7EEF847194D038
   - BndEvt__W_Level3_Button_Tool5_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature

### 8.9 Construct

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：Construct -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_FunctionEntry_0:89DBCF0D4A091A257B1F77B137CA26A6
   - Construct

### 8.10 HandleLevel3PhaseChanged

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：HandleLevel3PhaseChanged -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_SetVariableOnPersistentFrame_0:42279E264FC69C1CFAE72BAE77E1BB95
   - HandleLevel3PhaseChanged

### 8.11 HandleLevel3ProgressChanged

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：HandleLevel3ProgressChanged -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_SetVariableOnPersistentFrame_0:C04CBFDD4C2DA3DA6AAECD8D402F6AC1
   - HandleLevel3ProgressChanged

### 8.12 HandleLevel3ResultReady

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：HandleLevel3ResultReady -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_SetVariableOnPersistentFrame_0:03FAB3E5432E0A1E54AA7D88ED04A00E
   - HandleLevel3ResultReady

### 8.13 HandleLevel3ToolSelected

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：HandleLevel3ToolSelected -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_SetVariableOnPersistentFrame_0:DCBEFC9C4A913EA4617CB9B96970C85A
   - HandleLevel3ToolSelected

### 8.14 PreConstruct

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：PreConstruct -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level3
1. 关键节点：
   - ExecuteUbergraph_W_Level3
     输入：execute <- K2Node_SetVariableOnPersistentFrame_0:CDEE38BA411AE3E5E9B661B57BC43BED
   - PreConstruct

### 8.15 RefreshToolSelectionVisuals

1. 图类型：UMGEditor.WidgetGraphSchema
1. 模块数：31
1. 连接数：39
1. 执行链：RefreshToolSelectionVisuals -> K2Node_IfThenElse_1 -> SetVisibility -> K2Node_IfThenElse_4 -> SetVisibility -> K2Node_IfThenElse_5 -> SetVisibility -> K2Node_IfThenElse_6 -> SetVisibility -> K2Node_IfThenElse_0 -> SetVisibility
1. 关键节点：
   - SetVisibility
     输入：execute <- K2Node_IfThenElse_0:D4AA9DF94BC2CC038C0729A15ED2E3B2
     输入：self <- K2Node_VariableGet_12:356B1986491258011D37E391CEAF6327
     输入：InVisibility = Collapsed
   - SetVisibility
     输入：execute <- K2Node_IfThenElse_0:D6BAD89943E44D87198C268C51E7F77C
     输入：self <- K2Node_VariableGet_12:356B1986491258011D37E391CEAF6327
     输入：InVisibility = SelfHitTestInvisible
   - SetVisibility
     输入：execute <- K2Node_IfThenElse_6:D4AA9DF94BC2CC038C0729A15ED2E3B2
     输入：self <- K2Node_VariableGet_11:FF7CBD854554C978336E239B1BBC5139
     输入：InVisibility = Collapsed
   - SetVisibility
     输入：execute <- K2Node_IfThenElse_6:D6BAD89943E44D87198C268C51E7F77C
     输入：self <- K2Node_VariableGet_11:FF7CBD854554C978336E239B1BBC5139
     输入：InVisibility = SelfHitTestInvisible
   - SetVisibility
     输入：execute <- K2Node_IfThenElse_5:D4AA9DF94BC2CC038C0729A15ED2E3B2
     输入：self <- K2Node_VariableGet_10:F4EC7DF7454BEE7C81A2E3BEA2FA5924
     输入：InVisibility = Collapsed
   - SetVisibility
     输入：execute <- K2Node_IfThenElse_5:D6BAD89943E44D87198C268C51E7F77C
     输入：self <- K2Node_VariableGet_10:F4EC7DF7454BEE7C81A2E3BEA2FA5924
     输入：InVisibility = SelfHitTestInvisible
   - SetVisibility
     输入：execute <- K2Node_IfThenElse_4:D4AA9DF94BC2CC038C0729A15ED2E3B2
     输入：self <- K2Node_VariableGet_4:2ABA88FE45D32B2E6C1C168388C20D1B
     输入：InVisibility = Collapsed
   - SetVisibility
     输入：execute <- K2Node_IfThenElse_4:D6BAD89943E44D87198C268C51E7F77C
     输入：self <- K2Node_VariableGet_4:2ABA88FE45D32B2E6C1C168388C20D1B
     输入：InVisibility = SelfHitTestInvisible
   - SetVisibility
     输入：execute <- K2Node_IfThenElse_1:D4AA9DF94BC2CC038C0729A15ED2E3B2
     输入：self <- K2Node_VariableGet_3:E8749257411AF7EE054E4E90614BC1AF
     输入：InVisibility = Collapsed
   - SetVisibility
     输入：execute <- K2Node_IfThenElse_1:D6BAD89943E44D87198C268C51E7F77C
     输入：self <- K2Node_VariableGet_3:E8749257411AF7EE054E4E90614BC1AF
     输入：InVisibility = SelfHitTestInvisible
   - RefreshToolSelectionVisuals

### 8.16 RequestSelectTool

1. 图类型：UMGEditor.WidgetGraphSchema
1. 模块数：5
1. 连接数：5
1. 执行链：RequestSelectTool -> K2Node_MacroInstance_0 -> CaissonPlayerController.SelectLevel3Tool
1. 关键节点：
   - CaissonPlayerController.SelectLevel3Tool
     输入：execute <- K2Node_MacroInstance_0:D085256647DD7FAFD1B55AA30E43747D
     输入：self <- K2Node_VariableGet_4:8D172F024F81794E9FAF488198B32060
     输入：ToolId <- K2Node_VariableGet_5:E49AF71C4EE3B892347AB1B48ED64EBE
   - RequestSelectTool

## 9. 当前可直接确认的结论

1. 本蓝图至少存在上文列出的执行链，可作为后续人工确认和接线补全文档的基础。
1. Widget 树已经从导出中整理出来，后续可以继续补充哪些控件参与了实际逻辑。
1. 变量区已经整理完成，后续重点是确认哪些变量仍然在真实运行链中使用。

## 10. 后续补充建议

1. 如果该蓝图已经过运行验证，建议补充一节“已运行验证链路”。
1. 如果导出中存在历史残留变量、残留事件或旧链路，建议后续在本文档中手动标记。
1. 如果本蓝图和别的蓝图存在强关联，建议在文档中补“对外依赖”。
