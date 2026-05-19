# W_Level4

## 1. 蓝图定位

本文档由 Docs/BlueprintExports/W_Level4.json 自动整理生成，用于记录当前蓝图的真实导出结构。

## 2. 基础信息

1. 蓝图名：W_Level4
1. 资源路径：/Script/UMGEditor.WidgetBlueprint'/Game/UI/W_Level4.W_Level4'
1. 蓝图类型：WidgetBlueprint
1. 父类：UMG.UserWidget
1. 生成类：W_Level4_C
1. Tick 预测：OnDemand
1. Tick 原因：Anim

## 3. 当前状态

1. 本文档基于蓝图导出数据自动生成。
1. 事件链、控件树、变量和特殊设置以导出内容为准。
1. 如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。

## 4. 控件树

1. 根控件：CanvasPanel_30

```text
BackgroundBlur_145 [BackgroundBlur] | parent=NewbieGuideOverlay | children=无
Button_Close_Guide [Button] | parent=NewbieGuideOverlay | children=无
Continue_Next [Button] | parent=StageSolved | children=无
difficuty_button_expert [Button] | parent=CanvasPanel_30 | children=无
difficuty_button_normal [Button] | parent=CanvasPanel_30 | children=无
pices_1_button [Button] | parent=picies_1 | children=无
pices_2_button [Button] | parent=picies_2 | children=无
pices_3_button [Button] | parent=picies_3 | children=无
pices_4_button [Button] | parent=picies_4 | children=无
pices_5_button [Button] | parent=picies_5 | children=无
CanvasPanel_30 [CanvasPanel] | parent=ROOT | children=StageSolved, Highlight_1, StepNum_1, PiciesBase, stepcirclesmall_2, picies_3, picies_4, picies_5, picies_2, stepcirclesmall_1, difficuty_base_normal, difficuty_base_expert, Finish_1, line, Tools_5_Unselected, Tools_4_Unselected, Difficuty_selected_normal, Tools_3_Unselected, Difficuty_selected_expert, difficuty_button_normal, difficuty_button_expert, Tools_2_Unselected, Tools_1_Unselected, CurrentStepBackGround, cloudtittle, stepcirclesmall, stepcirclesmallcurrent_1, stepcirclesmallcurrent_2, stepcirclesmallcurrent_3, CurrentStepBackGround_1, Step_content, CurrentStepBackGround_2, CurrentStepBackGround_3, Step3_content, Step2_content, picies_1, Step1_content_1, Step2_content_1, Step3_content_1, Step_content_1, Step1_content, NewbieGuideOverlay, FocusCirclePanel, StepBase, DifficultyBase
Difficuty_selected_expert [CanvasPanel] | parent=CanvasPanel_30 | children=Image_2, Image_6, difficuty_base_expert_1
Difficuty_selected_normal [CanvasPanel] | parent=CanvasPanel_30 | children=Image_102, Image_104, difficuty_base_normal_1
FocusCirclePanel [CanvasPanel] | parent=CanvasPanel_30 | children=light, circle
NewbieGuideOverlay [CanvasPanel] | parent=CanvasPanel_30 | children=Image_274, TextBlock_77, TextBlock_8, Button_Close_Guide, GuideDimBackground, BackgroundBlur_145
picies_1 [CanvasPanel] | parent=CanvasPanel_30 | children=Image_3, pices_1_selected, pices_1_button
picies_2 [CanvasPanel] | parent=CanvasPanel_30 | children=Image_0, pices_2_button, pices_2_selected
picies_3 [CanvasPanel] | parent=CanvasPanel_30 | children=Image_1, pices_3_button, pices_3_selected
picies_4 [CanvasPanel] | parent=CanvasPanel_30 | children=Image_4, pices_4_button, pices_4_selected
picies_5 [CanvasPanel] | parent=CanvasPanel_30 | children=Image_5, pices_5_button, pices_5_selected
StageSolved [CanvasPanel] | parent=CanvasPanel_30 | children=Continue_Next, TextBlock_86, Image_101
Highlight_1 [Image] | parent=CanvasPanel_30 | children=无
StepNum_1 [EditableTextBox] | parent=CanvasPanel_30 | children=无
PiciesBase [Image] | parent=CanvasPanel_30 | children=无
stepcirclesmall_2 [Image] | parent=CanvasPanel_30 | children=无
stepcirclesmall_1 [Image] | parent=CanvasPanel_30 | children=无
difficuty_base_normal [Image] | parent=CanvasPanel_30 | children=无
difficuty_base_expert [Image] | parent=CanvasPanel_30 | children=无
Finish_1 [Image] | parent=CanvasPanel_30 | children=无
line [Image] | parent=CanvasPanel_30 | children=无
Tools_5_Unselected [Image] | parent=CanvasPanel_30 | children=无
Tools_4_Unselected [Image] | parent=CanvasPanel_30 | children=无
Tools_3_Unselected [Image] | parent=CanvasPanel_30 | children=无
Tools_2_Unselected [Image] | parent=CanvasPanel_30 | children=无
Tools_1_Unselected [Image] | parent=CanvasPanel_30 | children=无
CurrentStepBackGround [Image] | parent=CanvasPanel_30 | children=无
cloudtittle [TextBlock] | parent=CanvasPanel_30 | children=无
stepcirclesmall [Image] | parent=CanvasPanel_30 | children=无
stepcirclesmallcurrent_1 [Image] | parent=CanvasPanel_30 | children=无
stepcirclesmallcurrent_2 [Image] | parent=CanvasPanel_30 | children=无
stepcirclesmallcurrent_3 [Image] | parent=CanvasPanel_30 | children=无
CurrentStepBackGround_1 [Image] | parent=CanvasPanel_30 | children=无
Step_content [EditableTextBox] | parent=CanvasPanel_30 | children=无
CurrentStepBackGround_2 [Image] | parent=CanvasPanel_30 | children=无
CurrentStepBackGround_3 [Image] | parent=CanvasPanel_30 | children=无
Step3_content [EditableTextBox] | parent=CanvasPanel_30 | children=无
Step2_content [EditableTextBox] | parent=CanvasPanel_30 | children=无
Step1_content_1 [EditableTextBox] | parent=CanvasPanel_30 | children=无
Step2_content_1 [EditableTextBox] | parent=CanvasPanel_30 | children=无
Step3_content_1 [EditableTextBox] | parent=CanvasPanel_30 | children=无
Step_content_1 [EditableTextBox] | parent=CanvasPanel_30 | children=无
Step1_content [EditableTextBox] | parent=CanvasPanel_30 | children=无
StepBase [Image] | parent=CanvasPanel_30 | children=无
DifficultyBase [Image] | parent=CanvasPanel_30 | children=无
Image_2 [Image] | parent=Difficuty_selected_expert | children=无
Image_6 [Image] | parent=Difficuty_selected_expert | children=无
difficuty_base_expert_1 [Image] | parent=Difficuty_selected_expert | children=无
Image_102 [Image] | parent=Difficuty_selected_normal | children=无
Image_104 [Image] | parent=Difficuty_selected_normal | children=无
difficuty_base_normal_1 [Image] | parent=Difficuty_selected_normal | children=无
light [Image] | parent=FocusCirclePanel | children=无
circle [Image] | parent=FocusCirclePanel | children=无
Image_274 [Image] | parent=NewbieGuideOverlay | children=无
TextBlock_77 [TextBlock] | parent=NewbieGuideOverlay | children=无
TextBlock_8 [TextBlock] | parent=NewbieGuideOverlay | children=无
GuideDimBackground [Image] | parent=NewbieGuideOverlay | children=无
Image_3 [Image] | parent=picies_1 | children=无
pices_1_selected [Image] | parent=picies_1 | children=无
Image_0 [Image] | parent=picies_2 | children=无
pices_2_selected [Image] | parent=picies_2 | children=无
Image_1 [Image] | parent=picies_3 | children=无
pices_3_selected [Image] | parent=picies_3 | children=无
Image_4 [Image] | parent=picies_4 | children=无
pices_4_selected [Image] | parent=picies_4 | children=无
Image_5 [Image] | parent=picies_5 | children=无
pices_5_selected [Image] | parent=picies_5 | children=无
TextBlock_86 [TextBlock] | parent=StageSolved | children=无
Image_101 [Image] | parent=StageSolved | children=无
```

## 5. 事件图主链

### 5.1 组件事件

1. difficuty_button_normal -> OnClicked
1. difficuty_button_expert -> OnClicked
1. pices_1_button -> OnClicked
1. pices_2_button -> OnClicked
1. pices_3_button -> OnClicked
1. pices_4_button -> OnClicked
1. pices_5_button -> OnClicked
1. Continue_Next -> OnClicked
1. Button_Close_Guide -> OnClicked

### 5.2 主执行链

1. PreConstruct
1. Construct -> PlayAnimation -> SetVisibility -> Cast To CaissonPlayerController -> Set Level4PuzzleComponentRef -> K2Node_AddDelegate_3 -> K2Node_AddDelegate_4 -> CaissonPlayerController.StartLevel4Puzzle -> Set bLevel4Started -> K2Node_AddDelegate_0 -> K2Node_AddDelegate_1 -> K2Node_AddDelegate_2
1. difficuty_button_normal.OnClicked -> IsValid -> Level4PuzzleComponent.SetLevel4Difficulty
1. difficuty_button_normal.OnClicked -> IsValid -> Level4PuzzleComponent.StartLevel4Puzzle -> Set bLevel4Started
1. difficuty_button_expert.OnClicked -> IsValid -> Level4PuzzleComponent.StartLevel4Puzzle -> Set bLevel4Started
1. difficuty_button_expert.OnClicked -> IsValid -> Level4PuzzleComponent.SetLevel4Difficulty
1. pices_1_button.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.SelectLevel4Piece
1. pices_2_button.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.SelectLevel4Piece
1. pices_3_button.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.SelectLevel4Piece
1. pices_4_button.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.SelectLevel4Piece
1. pices_5_button.OnClicked -> Cast To CaissonPlayerController -> CaissonPlayerController.SelectLevel4Piece
1. HandleLevel4PieceSelected -> SetVisibility -> K2Node_SwitchInteger_0 -> SetVisibility
1. HandleLevel4StageChanged -> SetVisibility -> Switch on ELevel4StageId
1. HandleLevel4Completed -> KismetSystemLibrary.PrintString -> Cast To BP_ShowcaseModel_Level4_C -> Actor.SetActorHiddenInGame -> Actor.SetActorEnableCollision -> Actor.SetActorTickEnabled -> K2Node_CreateWidget_0 -> AddToViewport -> RemoveFromParent
1. EVT_OnLevel4StageSolved -> SetVisibility -> Switch on ELevel4StageId -> SetVisibility -> SetVisibility
1. EVT_OnLevel4StageSolved -> SetVisibility -> Switch on ELevel4StageId -> Set viewChangeFlag -> SetVisibility -> SetVisibility
1. Continue_Next.OnClicked -> SetVisibility -> Cast To CaissonPlayerController -> CaissonPlayerController.ContinueLevel4AfterStageSolved -> K2Node_IfThenElse_1 -> Cast To BP_ShowcaseModel_Level4_C -> BP_ShowcaseModel_Level4_C.FinalViewChanged -> Set viewChangeFlag -> Switch on ELevel4StageId -> SetVisibility -> SetVisibility -> Cast To BP_ShowcaseModel_Level4_C -> BP_ShowcaseModel_Level4_C.StarMapViewChanged
1. Continue_Next.OnClicked -> SetVisibility -> Cast To CaissonPlayerController -> CaissonPlayerController.ContinueLevel4AfterStageSolved -> K2Node_IfThenElse_1 -> Cast To BP_ShowcaseModel_Level4_C -> BP_ShowcaseModel_Level4_C.FinalViewChanged -> Set viewChangeFlag -> Switch on ELevel4StageId -> SetVisibility
1. Continue_Next.OnClicked -> SetVisibility -> Cast To CaissonPlayerController -> CaissonPlayerController.ContinueLevel4AfterStageSolved -> K2Node_IfThenElse_1 -> Cast To BP_ShowcaseModel_Level4_C -> BP_ShowcaseModel_Level4_C.FinalViewChanged -> Set viewChangeFlag -> Switch on ELevel4StageId -> Switch on ELevel4Difficulty -> SetVisibility -> SetVisibility -> Cast To BP_ShowcaseModel_Level4_C -> BP_ShowcaseModel_Level4_C.StarMapViewChanged
1. Continue_Next.OnClicked -> SetVisibility -> Cast To CaissonPlayerController -> CaissonPlayerController.ContinueLevel4AfterStageSolved -> K2Node_IfThenElse_1 -> Switch on ELevel4StageId -> SetVisibility -> SetVisibility -> Cast To BP_ShowcaseModel_Level4_C -> BP_ShowcaseModel_Level4_C.StarMapViewChanged
1. Continue_Next.OnClicked -> SetVisibility -> Cast To CaissonPlayerController -> CaissonPlayerController.ContinueLevel4AfterStageSolved -> K2Node_IfThenElse_1 -> Switch on ELevel4StageId -> SetVisibility
1. Continue_Next.OnClicked -> SetVisibility -> Cast To CaissonPlayerController -> CaissonPlayerController.ContinueLevel4AfterStageSolved -> K2Node_IfThenElse_1 -> Switch on ELevel4StageId -> Switch on ELevel4Difficulty -> SetVisibility -> SetVisibility -> Cast To BP_ShowcaseModel_Level4_C -> BP_ShowcaseModel_Level4_C.StarMapViewChanged
1. EVT_OnLevel4DifficultyChanged -> Switch on ELevel4Difficulty -> SetVisibility -> SetVisibility
1. Button_Close_Guide.OnClicked -> SetVisibility

## 6. 关键变量

### 6.1 自定义变量

1. bLevel4Started
   类型：bool
   显示名：Level 4Started
1. Level4PuzzleComponentRef
   类型：object -> /Script/CoreUObject.Class'/Script/CaissonCeiling.Level4PuzzleComponent'
   显示名：Level 4Puzzle Component Ref
1. viewChangeFlag
   类型：bool
   显示名：View Change Flag

### 6.2 自动生成的控件变量

1. Tools_5_Unselected -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. StepNum_1 -> /Script/CoreUObject.Class'/Script/UMG.EditableTextBox'
1. stepcirclesmallcurrent_3 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. stepcirclesmallcurrent_2 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. stepcirclesmallcurrent_1 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Step_content_1 -> /Script/CoreUObject.Class'/Script/UMG.EditableTextBox'
1. Step3_content_1 -> /Script/CoreUObject.Class'/Script/UMG.EditableTextBox'
1. Step2_content_1 -> /Script/CoreUObject.Class'/Script/UMG.EditableTextBox'
1. Step1_content_1 -> /Script/CoreUObject.Class'/Script/UMG.EditableTextBox'
1. StageSolved -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
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
1. NewbieGuideOverlay -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. Highlight_1 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. FocusCirclePanel -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. Finish_1 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Difficuty_selected_normal -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. Difficuty_selected_expert -> /Script/CoreUObject.Class'/Script/UMG.CanvasPanel'
1. difficuty_button_normal -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. difficuty_button_expert -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. CurrentStepBackGround_3 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. CurrentStepBackGround_2 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. CurrentStepBackGround_1 -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. CurrentStepBackGround -> /Script/CoreUObject.Class'/Script/UMG.Image'
1. Continue_Next -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. Button_Close_Guide -> /Script/CoreUObject.Class'/Script/UMG.Button'
1. FocusNewAnimation -> /Script/CoreUObject.Class'/Script/UMG.WidgetAnimation'

## 7. 特殊设置

1. W_Level4.TickPrediction = OnDemand
   原因：Anim
1. BackgroundBlur_145.Visibility = Collapsed
1. Button_Close_Guide.BackgroundColor = (R=0.000000,G=0.000000,B=0.000000,A=0.200000)
1. Button_Close_Guide.ColorAndOpacity = (R=0.000000,G=0.000000,B=0.000000,A=0.000000)
1. Continue_Next.BackgroundColor = (R=0.000000,G=0.000000,B=0.000000,A=0.529739)
1. Continue_Next.ColorAndOpacity = (R=0.000000,G=0.000000,B=0.000000,A=0.000000)
1. difficuty_button_expert.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. difficuty_button_expert.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. difficuty_button_normal.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. difficuty_button_normal.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_1_button.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_1_button.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_2_button.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_2_button.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_3_button.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_3_button.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_4_button.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_4_button.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_5_button.BackgroundColor = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. pices_5_button.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.000000)
1. Difficuty_selected_expert.Visibility = Collapsed
1. Difficuty_selected_normal.Visibility = Collapsed
1. FocusCirclePanel.Visibility = HitTestInvisible
1. NewbieGuideOverlay.Visibility = Visible
1. StageSolved.Visibility = Collapsed
1. Highlight_1.Brush = (ImageType=FullColor,ImageSize=(X=77.000000,Y=77.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/Ellipse_27.Ellipse_27'")
1. StepNum_1.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "E48140814500FA629EEF53BC4627B2BE", "1")
1. PiciesBase.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/右侧碎片列表底框.右侧碎片列表底框'")
1. stepcirclesmall_2.ColorAndOpacity = (R=0.287441,G=0.287441,B=0.270498,A=1.000000)
1. stepcirclesmall_2.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/点击圆圈内部.点击圆圈内部'")
1. stepcirclesmall_1.ColorAndOpacity = (R=0.287441,G=0.287441,B=0.270498,A=1.000000)
1. stepcirclesmall_1.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/点击圆圈内部.点击圆圈内部'")
1. difficuty_base_normal.Brush = (ImageType=FullColor,ImageSize=(X=377.000000,Y=377.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/学徒难度未选中.学徒难度未选中'")
1. difficuty_base_expert.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/专家难度未选中.专家难度未选中'")
1. Finish_1.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/对勾.对勾'")
1. Finish_1.Visibility = Collapsed
1. line.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/修复程度横线.修复程度横线'")
1. Tools_5_Unselected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子未选择状态背景.刷子未选择状态背景'")
1. Tools_4_Unselected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子未选择状态背景.刷子未选择状态背景'")
1. Tools_3_Unselected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子未选择状态背景.刷子未选择状态背景'")
1. Tools_2_Unselected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子未选择状态背景.刷子未选择状态背景'")
1. Tools_1_Unselected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/刷子未选择状态背景.刷子未选择状态背景'")
1. CurrentStepBackGround.Brush = (ImageType=FullColor,ImageSize=(X=905.000000,Y=92.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/高亮显示背景.高亮显示背景'")
1. cloudtittle.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "E4627B7C49AF8AD447AFB886EC4A654B", "天宫藻井")
1. cloudtittle.ColorAndOpacity = (SpecifiedColor=(R=0.630757,G=0.439657,B=0.230740,A=1.000000))
1. stepcirclesmall.ColorAndOpacity = (R=0.287441,G=0.287441,B=0.270498,A=1.000000)
1. stepcirclesmall.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/点击圆圈内部.点击圆圈内部'")
1. stepcirclesmallcurrent_1.ColorAndOpacity = (R=0.930111,G=0.863157,B=0.610496,A=1.000000)
1. stepcirclesmallcurrent_1.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/点击圆圈内部.点击圆圈内部'")
1. stepcirclesmallcurrent_2.ColorAndOpacity = (R=0.930111,G=0.863157,B=0.610496,A=1.000000)
1. stepcirclesmallcurrent_2.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/点击圆圈内部.点击圆圈内部'")
1. stepcirclesmallcurrent_2.Visibility = Collapsed
1. stepcirclesmallcurrent_3.ColorAndOpacity = (R=0.930111,G=0.863157,B=0.610496,A=1.000000)
1. stepcirclesmallcurrent_3.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/点击圆圈内部.点击圆圈内部'")
1. stepcirclesmallcurrent_3.Visibility = Collapsed
1. CurrentStepBackGround_1.Brush = (ImageType=FullColor,ImageSize=(X=905.000000,Y=92.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/高亮显示背景.高亮显示背景'")
1. Step_content.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "5DCBD9C24D141A78BA5999844C4D9369", "天宫藻井的拼接")
1. CurrentStepBackGround_2.Brush = (ImageType=FullColor,ImageSize=(X=905.000000,Y=92.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/高亮显示背景.高亮显示背景'")
1. CurrentStepBackGround_2.Visibility = Collapsed
1. CurrentStepBackGround_3.Brush = (ImageType=FullColor,ImageSize=(X=905.000000,Y=92.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/任务栏/高亮显示背景.高亮显示背景'")
1. CurrentStepBackGround_3.Visibility = Collapsed
1. Step3_content.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "301DD12E417E70DA05F29EA7DDE6979C", "整体拼接")
1. Step2_content.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "43BAB8C64A7A55B9174BAE8B0CCF8407", "星空图拼接")
1. Step1_content_1.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "1CEFF7244F7AB6F000C8129307587D38", "云纹框架拼接")
1. Step1_content_1.Visibility = Collapsed
1. Step2_content_1.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "43BAB8C64A7A55B9174BAE8B0CCF8407", "星空图拼接")
1. Step2_content_1.Visibility = Collapsed
1. Step3_content_1.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "301DD12E417E70DA05F29EA7DDE6979C", "整体拼接")
1. Step3_content_1.Visibility = Collapsed
1. Step_content_1.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "5DCBD9C24D141A78BA5999844C4D9369", "天宫藻井的拼接")
1. Step_content_1.Visibility = Collapsed
1. Step1_content.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "1CEFF7244F7AB6F000C8129307587D38", "云纹框架拼接")
1. StepBase.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/左上角.左上角'")
1. DifficultyBase.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/难度选择底框.难度选择底框'")
1. Image_2.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/圆环选中png.圆环选中png'")
1. Image_6.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/当前选择.当前选择'")
1. difficuty_base_expert_1.Brush = (ImageType=FullColor,ImageSize=(X=1192.000000,Y=1192.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/专家难度选中.专家难度选中'")
1. Image_102.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/圆环选中png.圆环选中png'")
1. Image_104.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/当前选择.当前选择'")
1. difficuty_base_normal_1.Brush = (ImageType=FullColor,ImageSize=(X=948.000000,Y=948.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/学徒难度选中.学徒难度选中'")
1. light.Brush = (ImageType=FullColor,ImageSize=(X=903.000000,Y=903.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/方形内部.方形内部'")
1. circle.ColorAndOpacity = (R=1.000000,G=1.000000,B=1.000000,A=0.700000)
1. circle.Brush = (ImageType=FullColor,ImageSize=(X=977.000000,Y=977.000000),ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/方形外部.方形外部'")
1. Image_274.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Level1/对话框底部.对话框底部'")
1. TextBlock_77.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "E5F108544888F7EB4AE2F094775D4174", "在这里，可以选择拼接的难度。如果拼接时遇到困难，可以随\r\n时调整哦！")
1. TextBlock_77.ColorAndOpacity = (SpecifiedColor=(R=0.011612,G=0.011612,B=0.011612,A=1.000000))
1. TextBlock_77.AutoWrapText = True
1. TextBlock_8.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "7EB2651D4350B9B8804AF8ADC471FA2E", "现在，藻井的碎片已经被修复，我们要做的就是把它拼接起来。")
1. TextBlock_8.ColorAndOpacity = (SpecifiedColor=(R=0.011612,G=0.011612,B=0.011612,A=1.000000))
1. GuideDimBackground.ColorAndOpacity = (R=0.000000,G=0.000000,B=0.000000,A=0.550000)
1. GuideDimBackground.Visibility = Collapsed
1. Image_3.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/碎片1.碎片1'")
1. pices_1_selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/点击特效_方形）.点击特效_方形）'")
1. Image_0.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/碎片2_1.碎片2_1'")
1. pices_2_selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/点击特效_方形）.点击特效_方形）'")
1. Image_1.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/碎片3.碎片3'")
1. pices_3_selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/点击特效_方形）.点击特效_方形）'")
1. Image_4.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/碎片5.碎片5'")
1. pices_4_selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/点击特效_方形）.点击特效_方形）'")
1. Image_5.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/碎片6.碎片6'")
1. pices_5_selected.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Pinjie/点击特效_方形）.点击特效_方形）'")
1. TextBlock_86.Text = NSLOCTEXT("[8EDE3FA6D1D1A1F9630203C981A361E7]", "663D39574C0964C4DDA53F8E07A0E71C", "本关完成！")
1. Image_101.Brush = (ResourceObject="/Script/Engine.Texture2D'/Game/assets/Chuchen/本部分已修复.本部分已修复'")
1. Image_101.Visibility = SelfHitTestInvisible
1. StageSolved.LayoutData = (Offsets=(Right=0.000000,Bottom=0.000000),Anchors=(Maximum=(X=1.000000,Y=1.000000)))
1. Highlight_1.LayoutData = (Offsets=(Left=140.000000,Top=208.000000,Right=33.000000,Bottom=33.000000))
1. StepNum_1.LayoutData = (Offsets=(Left=145.854294,Top=209.669998,Right=27.241528))
1. PiciesBase.LayoutData = (Offsets=(Left=1448.000000,Top=132.000000,Right=452.841705,Bottom=790.261353))
1. stepcirclesmall_2.LayoutData = (Offsets=(Left=152.000000,Top=280.000000,Right=9.000000,Bottom=9.000000))
1. picies_3.LayoutData = (Offsets=(Left=1540.000000,Top=472.000000,Right=120.000000,Bottom=120.000000))
1. picies_4.LayoutData = (Offsets=(Left=1708.000000,Top=468.000000,Right=120.000000,Bottom=120.000000))
1. picies_5.LayoutData = (Offsets=(Left=1540.000000,Top=632.000000,Right=120.000000,Bottom=120.000000))
1. picies_2.LayoutData = (Offsets=(Left=1708.000000,Top=304.000000,Right=120.000000,Bottom=120.000000))
1. stepcirclesmall_1.LayoutData = (Offsets=(Left=152.000000,Top=252.000000,Right=9.000000,Bottom=9.000000))
1. difficuty_base_normal.LayoutData = (Offsets=(Left=116.000000,Top=644.000000,Right=120.000000,Bottom=120.000000))
1. difficuty_base_expert.LayoutData = (Offsets=(Left=280.000000,Top=640.000000,Right=120.000000,Bottom=120.000000))
1. Finish_1.LayoutData = (Offsets=(Left=132.000000,Top=200.000000,Right=49.069069,Bottom=51.531532))
1. line.LayoutData = (Offsets=(Left=123.100266,Top=282.159760,Right=66.481583,Bottom=1.268372))
1. Tools_5_Unselected.LayoutData = (Offsets=(Left=1560.000000,Top=668.000000,Right=325.000000,Bottom=100.000000))
1. Tools_4_Unselected.LayoutData = (Offsets=(Left=1528.000000,Top=608.000000,Right=325.000000,Bottom=100.000000))
1. Difficuty_selected_normal.LayoutData = (Offsets=(Left=116.000000,Top=648.000000,Right=120.000000,Bottom=200.000000))
1. Tools_3_Unselected.LayoutData = (Offsets=(Left=1528.000000,Top=496.000000,Right=325.000000,Bottom=100.000000))
1. Difficuty_selected_expert.LayoutData = (Offsets=(Left=280.000000,Top=644.000000,Right=120.000000,Bottom=200.000000))
1. difficuty_button_normal.LayoutData = (Offsets=(Left=116.000000,Top=644.000000,Right=120.000000,Bottom=120.000000))
1. difficuty_button_expert.LayoutData = (Offsets=(Left=280.000000,Top=640.000000,Right=120.000000,Bottom=120.000000))
1. Tools_2_Unselected.LayoutData = (Offsets=(Left=1528.000000,Top=384.000000,Right=325.000000,Bottom=100.000000))
1. Tools_1_Unselected.LayoutData = (Offsets=(Left=1528.000000,Top=272.000000,Right=325.000000,Bottom=100.000000))
1. CurrentStepBackGround.LayoutData = (Offsets=(Left=140.000000,Top=208.000000,Right=331.651642,Bottom=33.390388))
1. cloudtittle.LayoutData = (Offsets=(Left=264.000000,Top=123.604210,Right=87.644791,Bottom=22.449593))
1. stepcirclesmall.LayoutData = (Offsets=(Left=152.000000,Top=304.000000,Right=9.000000,Bottom=9.000000))
1. stepcirclesmallcurrent_1.LayoutData = (Offsets=(Left=152.000000,Top=252.000000,Right=9.000000,Bottom=9.000000))
1. stepcirclesmallcurrent_2.LayoutData = (Offsets=(Left=152.000000,Top=280.000000,Right=9.000000,Bottom=9.000000))
1. stepcirclesmallcurrent_3.LayoutData = (Offsets=(Left=152.000000,Top=304.000000,Right=9.000000,Bottom=9.000000))
1. CurrentStepBackGround_1.LayoutData = (Offsets=(Left=144.000000,Top=244.000000,Right=331.651642,Bottom=18.055592))
1. Step_content.LayoutData = (Offsets=(Left=180.000000,Top=208.000000,Right=208.211060))
1. CurrentStepBackGround_2.LayoutData = (Offsets=(Left=144.000000,Top=272.000000,Right=331.651642,Bottom=18.055592))
1. CurrentStepBackGround_3.LayoutData = (Offsets=(Left=148.000000,Top=300.000000,Right=331.651642,Bottom=18.055592))
1. Step3_content.LayoutData = (Offsets=(Left=180.000000,Top=296.000000,Right=162.013596))
1. Step2_content.LayoutData = (Offsets=(Left=180.000000,Top=268.000000,Right=162.013596))
1. picies_1.LayoutData = (Offsets=(Left=1540.000000,Top=304.000000,Right=120.000000,Bottom=120.000000))
1. Step1_content_1.LayoutData = (Offsets=(Left=180.000000,Top=240.000000,Right=162.013596))
1. Step2_content_1.LayoutData = (Offsets=(Left=180.000000,Top=268.000000,Right=162.013596))
1. Step3_content_1.LayoutData = (Offsets=(Left=180.000000,Top=296.000000,Right=162.013596))
1. Step_content_1.LayoutData = (Offsets=(Left=180.000000,Top=208.000000,Right=208.211060))
1. Step1_content.LayoutData = (Offsets=(Left=180.000000,Top=240.000000,Right=162.013596))
1. NewbieGuideOverlay.LayoutData = (Offsets=(Right=0.000000,Bottom=0.000000),Anchors=(Maximum=(X=1.000000,Y=1.000000)))
1. FocusCirclePanel.LayoutData = (Offsets=(Left=1300.361328,Top=95.727264,Right=590.490479,Bottom=572.542542))
1. StepBase.LayoutData = (Offsets=(Left=52.000000,Top=47.806816,Right=525.911621,Bottom=375.802460))
1. DifficultyBase.LayoutData = (Offsets=(Left=64.000000,Top=532.000000,Right=391.491486,Bottom=330.779663))
1. Image_2.LayoutData = (Offsets=(Top=-4.000000,Right=120.000000,Bottom=120.000000))
1. Image_6.LayoutData = (Offsets=(Left=10.000000,Top=144.000000,Bottom=40.000000))
1. difficuty_base_expert_1.LayoutData = (Offsets=(Top=-4.000000,Right=120.000000,Bottom=120.000000))
1. Image_102.LayoutData = (Offsets=(Top=-4.000000,Right=120.000000,Bottom=120.000000))
1. Image_104.LayoutData = (Offsets=(Left=10.000000,Top=140.000000,Bottom=40.000000))
1. difficuty_base_normal_1.LayoutData = (Offsets=(Left=0.080000,Top=-4.000000,Right=120.000000,Bottom=120.000000))
1. light.LayoutData = (Offsets=(Left=204.750122,Top=172.809341,Right=190.000000,Bottom=190.000000))
1. circle.LayoutData = (Offsets=(Left=176.296967,Top=141.366623,Right=250.000000,Bottom=250.000000))
1. Image_274.LayoutData = (Offsets=(Left=-392.960938,Top=-381.081055,Right=1000.000000,Bottom=305.418762),Anchors=(Minimum=(X=0.500000,Y=1.000000),Maximum=(X=0.500000,Y=1.000000)))
1. TextBlock_77.LayoutData = (Offsets=(Left=864.000000,Top=853.723145,Right=628.528503,Bottom=63.633633))
1. TextBlock_8.LayoutData = (Offsets=(Left=864.000000,Top=816.000000))
1. Button_Close_Guide.LayoutData = (Offsets=(Right=0.000000,Bottom=0.000000),Anchors=(Maximum=(X=1.000000,Y=1.000000)))
1. GuideDimBackground.LayoutData = (Offsets=(Right=1947.447388,Bottom=1089.459473))
1. BackgroundBlur_145.LayoutData = (Offsets=(Right=1940.240112,Bottom=1099.068970))
1. Image_3.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_1_selected.LayoutData = (Offsets=(Left=-40.000000,Top=-40.000000,Right=200.000000,Bottom=200.000000))
1. pices_1_button.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. Image_0.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_2_button.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_2_selected.LayoutData = (Offsets=(Left=-40.000000,Top=-40.000000,Right=200.000000,Bottom=200.000000))
1. Image_1.LayoutData = (Offsets=(Left=-16.000000,Top=-16.000000,Right=150.000000,Bottom=150.000000))
1. pices_3_button.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_3_selected.LayoutData = (Offsets=(Left=-40.000000,Top=-40.000000,Right=200.000000,Bottom=200.000000))
1. Image_4.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_4_button.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_4_selected.LayoutData = (Offsets=(Left=-40.000000,Top=-40.000000,Right=200.000000,Bottom=200.000000))
1. Image_5.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_5_button.LayoutData = (Offsets=(Right=120.000000,Bottom=120.000000))
1. pices_5_selected.LayoutData = (Offsets=(Left=-40.000000,Top=-44.000000,Right=200.000000,Bottom=200.000000))
1. Continue_Next.LayoutData = (Offsets=(Right=0.000000,Bottom=0.000000),Anchors=(Maximum=(X=1.000000,Y=1.000000)))
1. TextBlock_86.LayoutData = (Offsets=(Left=16.000000,Top=56.000000))
1. Image_101.LayoutData = (Offsets=(Left=684.000000,Top=764.000000,Right=580.480469,Bottom=119.289291))

## 8. 其他图表

### 8.1 BndEvt__W_Level3_Button_Close_Guide_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level3_Button_Close_Guide_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - BndEvt__W_Level3_Button_Close_Guide_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level3_Button_Close_Guide_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature:then

### 8.2 BndEvt__W_Level4_Continue_Next_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_Continue_Next_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - BndEvt__W_Level4_Continue_Next_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_Continue_Next_K2Node_ComponentBoundEvent_7_OnButtonClickedEvent__DelegateSignature:then

### 8.3 BndEvt__W_Level4_difficuty_button_expert_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_difficuty_button_expert_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - BndEvt__W_Level4_difficuty_button_expert_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_difficuty_button_expert_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature:then

### 8.4 BndEvt__W_Level4_difficuty_button_normal_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_difficuty_button_normal_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - BndEvt__W_Level4_difficuty_button_normal_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_difficuty_button_normal_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature:then

### 8.5 BndEvt__W_Level4_pices_1_button_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_pices_1_button_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - BndEvt__W_Level4_pices_1_button_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_pices_1_button_K2Node_ComponentBoundEvent_2_OnButtonClickedEvent__DelegateSignature:then

### 8.6 BndEvt__W_Level4_pices_2_button_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_pices_2_button_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - BndEvt__W_Level4_pices_2_button_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_pices_2_button_K2Node_ComponentBoundEvent_3_OnButtonClickedEvent__DelegateSignature:then

### 8.7 BndEvt__W_Level4_pices_3_button_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_pices_3_button_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - BndEvt__W_Level4_pices_3_button_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_pices_3_button_K2Node_ComponentBoundEvent_4_OnButtonClickedEvent__DelegateSignature:then

### 8.8 BndEvt__W_Level4_pices_4_button_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_pices_4_button_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - BndEvt__W_Level4_pices_4_button_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_pices_4_button_K2Node_ComponentBoundEvent_5_OnButtonClickedEvent__DelegateSignature:then

### 8.9 BndEvt__W_Level4_pices_5_button_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：BndEvt__W_Level4_pices_5_button_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - BndEvt__W_Level4_pices_5_button_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature
   - ExecuteUbergraph_W_Level4
     输入：execute <- BndEvt__W_Level4_pices_5_button_K2Node_ComponentBoundEvent_6_OnButtonClickedEvent__DelegateSignature:then

### 8.10 Construct

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：Construct -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - Construct
   - ExecuteUbergraph_W_Level4
     输入：execute <- Construct:then

### 8.11 EVT_OnLevel4DifficultyChanged

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：EVT_OnLevel4DifficultyChanged -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - EVT_OnLevel4DifficultyChanged
   - ExecuteUbergraph_W_Level4
     输入：execute <- SetVariableOnPersistentFrame:then

### 8.12 EVT_OnLevel4StageSolved

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：4
1. 执行链：EVT_OnLevel4StageSolved -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - EVT_OnLevel4StageSolved
   - ExecuteUbergraph_W_Level4
     输入：execute <- SetVariableOnPersistentFrame:then

### 8.13 HandleLevel4Completed

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：HandleLevel4Completed -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - HandleLevel4Completed
   - ExecuteUbergraph_W_Level4
     输入：execute <- HandleLevel4Completed:then

### 8.14 HandleLevel4PieceSelected

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：HandleLevel4PieceSelected -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - HandleLevel4PieceSelected
   - ExecuteUbergraph_W_Level4
     输入：execute <- SetVariableOnPersistentFrame:then

### 8.15 HandleLevel4StageChanged

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：4
1. 执行链：HandleLevel4StageChanged -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - HandleLevel4StageChanged
   - ExecuteUbergraph_W_Level4
     输入：execute <- SetVariableOnPersistentFrame:then

### 8.16 PreConstruct

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：PreConstruct -> SetVariableOnPersistentFrame -> ExecuteUbergraph_W_Level4
1. 关键节点：
   - PreConstruct
   - ExecuteUbergraph_W_Level4
     输入：execute <- SetVariableOnPersistentFrame:then

## 9. 当前可直接确认的结论

1. 本蓝图至少存在上文列出的执行链，可作为后续人工确认和接线补全文档的基础。
1. Widget 树已经从导出中整理出来，后续可以继续补充哪些控件参与了实际逻辑。
1. 变量区已经整理完成，后续重点是确认哪些变量仍然在真实运行链中使用。

## 10. 后续补充建议

1. 如果该蓝图已经过运行验证，建议补充一节“已运行验证链路”。
1. 如果导出中存在历史残留变量、残留事件或旧链路，建议后续在本文档中手动标记。
1. 如果本蓝图和别的蓝图存在强关联，建议在文档中补“对外依赖”。
