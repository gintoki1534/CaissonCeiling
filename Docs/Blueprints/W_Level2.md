# W_Level2

## 1. 蓝图定位

本文档由 Docs/BlueprintExports/W_Level2.json 自动整理生成，用于记录当前蓝图的真实导出结构。

## 2. 基础信息

1. 蓝图名：W_Level2
1. 资源路径：/Script/UMGEditor.WidgetBlueprint'/Game/UI/W_Level2.W_Level2'
1. 蓝图类型：WidgetBlueprint
1. 父类：UMG.UserWidget
1. 生成类：W_Level2_C
1. Tick 预测：OnDemand
1. Tick 原因：Anim

## 3. 当前状态

1. 本文档基于蓝图导出数据自动生成。
1. 事件链、控件树、变量和特殊设置以导出内容为准。
1. 如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。

## 4. 控件树

1. 根控件：CanvasPanel_38

```text
TextBlock_1 [TextBlock] | parent=Introducing_3 | children=无
Image [Image] | parent=Introducing_3 | children=无
TextBlock [TextBlock] | parent=Introducing_2 | children=无
Image_2 [Image] | parent=Introducing_2 | children=无
TextBlock_71 [TextBlock] | parent=Introducing_1 | children=无
Image_1 [Image] | parent=Introducing_1 | children=无
TextBlock_2 [TextBlock] | parent=continue | children=无
Highlight_1 [Image] | parent=CanvasPanel_38 | children=无
EditableTextBox_290 [EditableTextBox] | parent=CanvasPanel_38 | children=无
云纹框架 [Image] | parent=CanvasPanel_38 | children=无
云纹框架-标题 [EditableTextBox] | parent=CanvasPanel_38 | children=无
StepNum_3 [EditableTextBox] | parent=CanvasPanel_38 | children=无
CurrentStepBackGround_1 [Image] | parent=CanvasPanel_38 | children=无
CurrentStepBackGround_2 [Image] | parent=CanvasPanel_38 | children=无
StepNum_2 [EditableTextBox] | parent=CanvasPanel_38 | children=无
Finish_3 [Image] | parent=CanvasPanel_38 | children=无
StepNum_1 [EditableTextBox] | parent=CanvasPanel_38 | children=无
Step2_content [EditableTextBox] | parent=CanvasPanel_38 | children=无
Step3_content [EditableTextBox] | parent=CanvasPanel_38 | children=无
Finish_2 [Image] | parent=CanvasPanel_38 | children=无
Step1_content [EditableTextBox] | parent=CanvasPanel_38 | children=无
CurrentStepBackGround [Image] | parent=CanvasPanel_38 | children=无
Finish_1 [Image] | parent=CanvasPanel_38 | children=无
Highlight_3 [Image] | parent=CanvasPanel_38 | children=无
StepFinish_2 [Image] | parent=CanvasPanel_38 | children=无
StepFinish_1 [Image] | parent=CanvasPanel_38 | children=无
StepFinish [Image] | parent=CanvasPanel_38 | children=无
Circle_2 [Image] | parent=CanvasPanel_38 | children=无
Highlight_2 [Image] | parent=CanvasPanel_38 | children=无
Circle_3 [Image] | parent=CanvasPanel_38 | children=无
Introducing_3 [CanvasPanel] | parent=CanvasPanel_38 | children=TextBlock_1, Image, Indtro_Button3
Introducing_2 [CanvasPanel] | parent=CanvasPanel_38 | children=TextBlock, Image_2, Indtro_Button2
Introducing_1 [CanvasPanel] | parent=CanvasPanel_38 | children=TextBlock_71, Image_1, Indtro_Button1
continue [CanvasPanel] | parent=CanvasPanel_38 | children=TextBlock_2
CanvasPanel_38 [CanvasPanel] | parent=ROOT | children=continue, Introducing_3, Introducing_2, Introducing_1, StepNum_3, StepNum_2, StepNum_1, Step3_content, Step2_content, Step1_content, CurrentStepBackGround_2, CurrentStepBackGround_1, CurrentStepBackGround, StepFinish_2, StepFinish_1, StepFinish, Finish_3, Finish_2, Finish_1, Circle_3, Circle_2, Highlight_3, Highlight_2, Highlight_1, EditableTextBox_290, 云纹框架-标题, 云纹框架
Indtro_Button3 [Button] | parent=Introducing_3 | children=无
Indtro_Button2 [Button] | parent=Introducing_2 | children=无
Indtro_Button1 [Button] | parent=Introducing_1 | children=无
```

## 5. 事件图主链

### 5.1 组件事件

1. Indtro_Button3 -> OnClicked
1. Indtro_Button2 -> OnClicked
1. Indtro_Button1 -> OnClicked

### 5.2 主执行链

1. Indtro_Button3.OnClicked -> SetVisibility -> BP_ShowcaseModel_C.ReturnToDefaultView -> KismetSystemLibrary.PrintString
1. Indtro_Button2.OnClicked -> SetVisibility -> BP_ShowcaseModel_C.ReturnToDefaultView -> KismetSystemLibrary.PrintString
1. Indtro_Button1.OnClicked -> SetVisibility -> BP_ShowcaseModel_C.ReturnToDefaultView -> KismetSystemLibrary.PrintString
1. OnFocusLost
1. Construct -> SetVisibility -> SetVisibility -> Cast To CaissonPlayerController -> Set CaissonControllerRef -> KismetSystemLibrary.ControlScreensaver -> K2Node_AddDelegate_1 -> Cast To BP_ShowcaseModel_C -> Set ShowcaseRef -> K2Node_AddDelegate_0 -> K2Node_AddDelegate_2 -> K2Node_AddDelegate_3 -> K2Node_AddDelegate_4

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
1. CurrentStep
   类型：int
   显示名：Current Step
1. CheckmarkIcon
   类型：object -> /Script/CoreUObject.Class'/Script/Engine.Texture2D'
   显示名：Checkmark Icon
1. ChoosenIcon
   类型：object -> /Script/CoreUObject.Class'/Script/Engine.Texture2D'
   显示名：Choosen Icon
1. UnselectIcon
   类型：object -> /Script/CoreUObject.Class'/Script/Engine.Texture2D'
   显示名：Unselect Icon
1. NewVar
   类型：struct -> /Script/CoreUObject.ScriptStruct'/Script/UMG.EventReply'
   显示名：New Var
1. Hit Component
   类型：object -> /Script/CoreUObject.Class'/Script/Engine.PrimitiveComponent'
   显示名：Hit Component
1. MaxPitch
   类型：int
   显示名：Max Pitch
1. MinPitch
   类型：int
   显示名：Min Pitch
1. CaissonControllerRef
   类型：object -> /Script/CoreUObject.Class'/Script/CaissonCeiling.CaissonPlayerController'
   显示名：Caisson Controller Ref
1. ShowcaseRef
   类型：object -> /Script/Engine.BlueprintGeneratedClass'/Game/UI/BP_ShowcaseModel.BP_ShowcaseModel_C'
   显示名：Showcase Ref

### 6.2 自动生成的控件变量

1. StepNum_3 -> /Script/CoreUObject.Class'/Script/UMG.EditableTextBox'
1. StepNum_2 -> /Script/CoreUObject.Class'/Script/UMG.EditableTextBox'
1. StepNum_1 -> /Script/CoreUObject.Class'/Script/UMG.EditableTextBox'
1. StepFinish_2 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. StepFinish_1 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. StepFinish -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Introducing_3 -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. Introducing_2 -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. Introducing_1 -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. Indtro_Button3 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Indtro_Button2 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Indtro_Button1 -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Highlight_3 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Highlight_2 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Highlight_1 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Finish_3 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Finish_2 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Finish_1 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. CurrentStepBackGround_2 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. CurrentStepBackGround_1 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. CurrentStepBackGround -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. continue -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. Circle_3 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Circle_2 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Anim_Glow_1 -> /Script/CoreUObject.Class'/Script/UMG.WidgetAnimation'
1. Anim_Glow_2 -> /Script/CoreUObject.Class'/Script/UMG.WidgetAnimation'
1. Anim_Glow_3 -> /Script/CoreUObject.Class'/Script/UMG.WidgetAnimation'

## 7. 特殊设置

1. W_Level2.TickPrediction = OnDemand
   原因：Anim
1. TextBlock_1.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "9E60DED647D1CDD732F04484EAA7D1EE", "介绍3")
1. TextBlock_1.ColorAndOpacity = (SpecifiedColor=(R=0.000000,G=0.000000,B=0.000000,A=1.000000))
1. Image.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/对话框底部.对话框底部'")
1. TextBlock.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "226173854CE71E150C4664B242F564D9", "介绍2")
1. TextBlock.ColorAndOpacity = (SpecifiedColor=(R=0.000000,G=0.000000,B=0.000000,A=1.000000))
1. Image_2.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/对话框底部.对话框底部'")
1. TextBlock_71.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "42557600434EDB78BA1E72936E75B267", "介绍1")
1. TextBlock_71.ColorAndOpacity = (SpecifiedColor=(R=0.000000,G=0.000000,B=0.000000,A=1.000000))
1. Image_1.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/对话框底部.对话框底部'")
1. TextBlock_2.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "562B9E9B4B9734D98D8C93A56FDD4931", "点击屏幕以继续")
1. Highlight_1.Brush = (ImageType=FullColor,ImageSize=(X=77.000000,Y=77.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/Ellipse_27.Ellipse_27'")
1. EditableTextBox_290.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "1EEF58504002B4647869D0ADF90D534B", "修复步骤")
1. 云纹框架.Brush = (ImageType=FullColor,ImageSize=(X=1520.000000,Y=1278.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/新手引导2.新手引导2'")
1. 云纹框架-标题.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "048C76F54EF8E8D50E91C0B6956301D0", "云纹框架")
1. StepNum_3.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "B3E45590466898AFAA3F7690F96D7322", "3")
1. CurrentStepBackGround_1.Brush = (ImageType=FullColor,ImageSize=(X=905.000000,Y=92.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/高亮显示背景.高亮显示背景'")
1. CurrentStepBackGround_2.Brush = (ImageType=FullColor,ImageSize=(X=905.000000,Y=92.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/高亮显示背景.高亮显示背景'")
1. StepNum_2.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "0D25CA1D4BAFFBE3D14EA388B70B947B", "2")
1. Finish_3.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/对勾.对勾'")
1. StepNum_1.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "57BEF76E48FA6F4A3A374A84DE5AB292", "1")
1. Step2_content.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "D4EE6B6B48163347E55A648812F847EF", "寻找[云纹雕饰]")
1. Step3_content.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "4D805242469EAB002BFB32A5C8E2594F", "寻找[唐代星空图]")
1. Finish_2.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/对勾.对勾'")
1. Step1_content.Text = NSLOCTEXT("[36D278295D84F0200FDE34B5820D24FA]", "CCC45BDA47F3DBD432AD998CF34DFDB8", "寻找[天宫楼阁]")
1. CurrentStepBackGround.Brush = (ImageType=FullColor,ImageSize=(X=905.000000,Y=92.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/高亮显示背景.高亮显示背景'")
1. Finish_1.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/对勾.对勾'")
1. Highlight_3.Brush = (ImageType=FullColor,ImageSize=(X=77.000000,Y=77.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/Ellipse_27.Ellipse_27'")
1. StepFinish_2.Brush = (ImageType=FullColor,ImageSize=(X=53.000000,Y=48.000000))
1. StepFinish_1.Brush = (ImageType=FullColor,ImageSize=(X=53.000000,Y=48.000000))
1. StepFinish.Brush = (ImageType=FullColor,ImageSize=(X=53.000000,Y=48.000000))
1. Circle_2.Brush = (ImageType=FullColor,ImageSize=(X=37.000000,Y=36.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/Ellipse_28.Ellipse_28'")
1. Highlight_2.Brush = (ImageType=FullColor,ImageSize=(X=77.000000,Y=77.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/Ellipse_27.Ellipse_27'")
1. Circle_3.Brush = (ImageType=FullColor,ImageSize=(X=37.000000,Y=36.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/Ellipse_28.Ellipse_28'")
1. Introducing_3.Visibility = Collapsed
1. Introducing_2.Visibility = Collapsed
1. Introducing_1.Visibility = Collapsed
1. continue.Visibility = Collapsed
1. Indtro_Button3.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. Indtro_Button3.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. Indtro_Button2.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. Indtro_Button2.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. Indtro_Button1.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. Indtro_Button1.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. TextBlock_1.LayoutData = (Offsets=(Left=399.067078,Top=207.623611))
1. Image.LayoutData = (Offsets=(Right=1000.000000,Bottom=400.000000))
1. Indtro_Button3.LayoutData = (Offsets=(Top=4.000000,Right=1000.000000,Bottom=400.000000))
1. TextBlock.LayoutData = (Offsets=(Left=324.166382,Top=175.063156))
1. Image_2.LayoutData = (Offsets=(Top=4.000000,Right=1000.000000,Bottom=400.000000))
1. Indtro_Button2.LayoutData = (Offsets=(Right=1000.000000,Bottom=400.000000))
1. TextBlock_71.LayoutData = (Offsets=(Left=304.024353,Top=162.480881))
1. Image_1.LayoutData = (Offsets=(Top=4.000000,Right=1000.000000,Bottom=400.000000))
1. Indtro_Button1.LayoutData = (Offsets=(Right=1000.000000,Bottom=400.000000))
1. TextBlock_2.LayoutData = (Offsets=(Left=529.087097,Top=53.893749,Right=813.856689,Bottom=127.812096))
1. continue.LayoutData = (Offsets=(Right=1922.393799,Bottom=1079.896973))
1. Introducing_3.LayoutData = (Offsets=(Left=428.000000,Top=636.000000,Right=1028.356934,Bottom=422.668365))
1. Introducing_2.LayoutData = (Offsets=(Left=432.000000,Top=636.000000,Right=1028.356934,Bottom=422.668365))
1. Introducing_1.LayoutData = (Offsets=(Left=432.000000,Top=636.000000,Right=1028.356934,Bottom=422.668365))
1. StepNum_3.LayoutData = (Offsets=(Left=104.000000,Top=348.000000,Right=27.241528))
1. StepNum_2.LayoutData = (Offsets=(Left=104.000000,Top=296.000000,Right=26.097527))
1. StepNum_1.LayoutData = (Offsets=(Left=104.000000,Top=248.000000,Right=27.241528))
1. Step3_content.LayoutData = (Offsets=(Left=152.000000,Top=348.000000,Right=156.971252))
1. Step2_content.LayoutData = (Offsets=(Left=152.000000,Top=296.000000,Right=128.142426))
1. Step1_content.LayoutData = (Offsets=(Left=152.000000,Top=248.000000,Right=128.142426))
1. CurrentStepBackGround_2.LayoutData = (Offsets=(Left=63.000000,Top=340.000000,Right=424.000000,Bottom=43.000000))
1. CurrentStepBackGround_1.LayoutData = (Offsets=(Left=63.000000,Top=288.000000,Right=424.000000,Bottom=43.000000))
1. CurrentStepBackGround.LayoutData = (Offsets=(Left=63.000000,Top=240.000000,Right=424.000000,Bottom=43.000000))
1. StepFinish_2.LayoutData = (Offsets=(Left=97.000000,Top=343.000000,Right=36.000000,Bottom=36.000000))
1. StepFinish_1.LayoutData = (Offsets=(Left=97.000000,Top=292.000000,Right=36.000000,Bottom=36.000000))
1. StepFinish.LayoutData = (Offsets=(Left=97.000000,Top=244.000000,Right=36.000000,Bottom=36.000000))
1. Finish_3.LayoutData = (Offsets=(Left=91.000000,Top=337.675690,Right=49.069069,Bottom=51.531532))
1. Finish_2.LayoutData = (Offsets=(Left=91.000000,Top=285.875641,Right=49.069069,Bottom=51.531532))
1. Finish_1.LayoutData = (Offsets=(Left=91.000000,Top=237.273270,Right=49.069069,Bottom=51.531532))
1. Circle_3.LayoutData = (Offsets=(Left=97.000000,Top=343.000000,Right=36.000000,Bottom=36.000000))
1. Circle_2.LayoutData = (Offsets=(Left=97.000000,Top=292.000000,Right=36.000000,Bottom=36.000000))
1. Highlight_3.LayoutData = (Offsets=(Left=97.000000,Top=343.000000,Right=36.000000,Bottom=36.000000))
1. Highlight_2.LayoutData = (Offsets=(Left=97.000000,Top=292.000000,Right=36.000000,Bottom=36.000000))
1. Highlight_1.LayoutData = (Offsets=(Left=97.000000,Top=244.000000,Right=36.000000,Bottom=36.000000))
1. EditableTextBox_290.LayoutData = (Offsets=(Left=96.000000,Top=182.000000,Right=388.000000,Bottom=40.000000))
1. 云纹框架-标题.LayoutData = (Offsets=(Left=96.000000,Top=133.000000,Right=388.000000,Bottom=40.000000))
1. 云纹框架.LayoutData = (Offsets=(Left=46.000000,Top=87.000000,Right=484.000000,Bottom=344.000000))

## 8. 其他图表

### 8.1 BndEvt__W_Level2_Indtro_Button1_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level2_Indtro_Button1_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level2
1. 关键节点：
   - ExecuteUbergraph_W_Level2
     输入：execute <- K2Node_FunctionEntry_0:7C179A3849BD5214222A8DAB55D476D6
   - BndEvt__W_Level2_Indtro_Button1_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature

### 8.2 BndEvt__W_Level2_Indtro_Button2_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level2_Indtro_Button2_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level2
1. 关键节点：
   - ExecuteUbergraph_W_Level2
     输入：execute <- K2Node_FunctionEntry_0:1A365790451F16EBC7D92D86719308C6
   - BndEvt__W_Level2_Indtro_Button2_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature

### 8.3 BndEvt__W_Level2_Indtro_Button3_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level2_Indtro_Button3_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level2
1. 关键节点：
   - ExecuteUbergraph_W_Level2
     输入：execute <- K2Node_FunctionEntry_0:6E9A2FAC4D33D8B17733889B3C2065E1
   - BndEvt__W_Level2_Indtro_Button3_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature

### 8.4 Construct

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：Construct -> ExecuteUbergraph_W_Level2
1. 关键节点：
   - ExecuteUbergraph_W_Level2
     输入：execute <- K2Node_FunctionEntry_0:ED293FEA4E1F8ACAA87F8A8AA2E827C0
   - Construct

### 8.5 HandleFinalContinuePromptRequested

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：HandleFinalContinuePromptRequested -> ExecuteUbergraph_W_Level2
1. 关键节点：
   - ExecuteUbergraph_W_Level2
     输入：execute <- K2Node_FunctionEntry_0:9D9F6A4C464678C86BEFC88011285C7F
   - HandleFinalContinuePromptRequested

### 8.6 HandleLevel2InspectStarted

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：4
1. 执行链：HandleLevel2InspectStarted -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level2
1. 关键节点：
   - ExecuteUbergraph_W_Level2
     输入：execute <- K2Node_SetVariableOnPersistentFrame_0:DB15BEF149206BDAD46F6483579F760F
   - HandleLevel2InspectStarted

### 8.7 HandleNextLevelRequested

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：HandleNextLevelRequested -> ExecuteUbergraph_W_Level2
1. 关键节点：
   - ExecuteUbergraph_W_Level2
     输入：execute <- K2Node_FunctionEntry_0:E41CD6DD41E01DC480E16592B17FF84E
   - HandleNextLevelRequested

### 8.8 HandleReturnToDefaultFinished

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：HandleReturnToDefaultFinished -> ExecuteUbergraph_W_Level2
1. 关键节点：
   - ExecuteUbergraph_W_Level2
     输入：execute <- K2Node_FunctionEntry_0:F0A17BCE41AA8F61A76E6CBFBE7C6D79
   - HandleReturnToDefaultFinished

### 8.9 OnFocusLost

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：OnFocusLost -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level2
1. 关键节点：
   - ExecuteUbergraph_W_Level2
     输入：execute <- K2Node_SetVariableOnPersistentFrame_0:88DA0CDF4F4A5B586590CC8A88D67F4A
   - OnFocusLost

### 8.10 新函数

1. 图类型：UMGEditor.WidgetGraphSchema
1. 模块数：1
1. 连接数：0
1. 执行链：新函数
1. 关键节点：
   - 新函数

### 8.11 自定义事件_0

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：自定义事件_0 -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level2
1. 关键节点：
   - ExecuteUbergraph_W_Level2
     输入：execute <- K2Node_SetVariableOnPersistentFrame_0:48212D094B72BE94F7A2A88F63766131
   - 自定义事件_0

## 9. 当前可直接确认的结论

1. 本蓝图至少存在上文列出的执行链，可作为后续人工确认和接线补全文档的基础。
1. Widget 树已经从导出中整理出来，后续可以继续补充哪些控件参与了实际逻辑。
1. 变量区已经整理完成，后续重点是确认哪些变量仍然在真实运行链中使用。

## 10. 后续补充建议

1. 如果该蓝图已经过运行验证，建议补充一节“已运行验证链路”。
1. 如果导出中存在历史残留变量、残留事件或旧链路，建议后续在本文档中手动标记。
1. 如果本蓝图和别的蓝图存在强关联，建议在文档中补“对外依赖”。
