# BP_ShowcaseModel_Level3

## 1. 蓝图定位

本文档由 Docs/BlueprintExports/BP_ShowcaseModel_Level3.json 自动整理生成，用于记录当前蓝图的真实导出结构。

## 2. 基础信息

1. 蓝图名：BP_ShowcaseModel_Level3
1. 资源路径：/Script/Engine.Blueprint'/Game/UI/BP_ShowcaseModel_Level3.BP_ShowcaseModel_Level3'
1. 蓝图类型：Blueprint
1. 父类：CaissonCeiling.CaissonPawn
1. 生成类：BP_ShowcaseModel_Level3_C

## 3. 当前状态

1. 本文档基于蓝图导出数据自动生成。
1. 事件链、控件树、变量和特殊设置以导出内容为准。
1. 如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。

## 4. 控件树


## 5. 事件图主链

### 5.2 主执行链

1. ReceiveTick
1. ReceiveActorBeginOverlap
1. ReceiveBeginPlay

## 6. 关键变量

### 6.1 自定义变量

1. DefaultArmLength
   类型：real/double
   显示名：Default Arm Length
1. DefaultArmRotation
   类型：struct -> /Script/CoreUObject.ScriptStruct'/Script/CoreUObject.Rotator'
   显示名：Default Arm Rotation
1. DefaultArmLocation
   类型：struct -> /Script/CoreUObject.ScriptStruct'/Script/CoreUObject.Vector'
   显示名：Default Arm Location
1. RepairArmLength
   类型：real/double
   显示名：Repair Arm Length
1. RepairArmRotation
   类型：struct -> /Script/CoreUObject.ScriptStruct'/Script/CoreUObject.Rotator'
   显示名：Repair Arm Rotation
1. RepairArmLocation
   类型：struct -> /Script/CoreUObject.ScriptStruct'/Script/CoreUObject.Vector'
   显示名：Repair Arm Location


## 7. 特殊设置

1. 当前导出中未发现额外特殊设置。

## 8. 其他图表

### 8.1 ActivateLevel3Presentation

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：4
1. 连接数：3
1. 执行链：ActivateLevel3Presentation -> SetActorHiddenInGame -> SetActorEnableCollision -> SetActorTickEnabled
1. 关键节点：
   - SetActorTickEnabled
     输入：execute <- K2Node_CallFunction_1:5859C35E463EBCE4B332CEB87A99D779
     输入：bEnabled = true
   - SetActorEnableCollision
     输入：execute <- K2Node_CallFunction_0:259AF0FA4A9697E4C94B37BABB617AC4
   - SetActorHiddenInGame
     输入：execute <- K2Node_FunctionEntry_0:4E8F13D24F25A96BA36C64A3F1D647E7
   - ActivateLevel3Presentation

### 8.2 ActivateRepairPresentation

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：7
1. 连接数：7
1. 执行链：ActivateRepairPresentation -> Set TargetArmLength -> SceneComponent.K2_SetWorldLocationAndRotation
1. 关键节点：
   - SceneComponent.K2_SetWorldLocationAndRotation
     输入：execute <- K2Node_VariableSet_1:3BC707704DA3E43B612C9692B4884296
     输入：self <- K2Node_VariableGet_1:E8DBDB134F09998A978E7B807FD4EE31
     输入：NewLocation <- K2Node_VariableGet_4:EAF5292A438967282229F09271D5F746
     输入：NewRotation <- K2Node_VariableGet_3:EDA2F53940AA0E01DA9E0C8DD1E805B5
   - Set TargetArmLength
     输入：execute <- K2Node_FunctionEntry_0:7A83F4FD43C18B2C7F2F3B8FC92048A5
     输入：TargetArmLength <- K2Node_VariableGet_2:D940AE7A41F9D6C85F4BE7BDC19FAFFB
     输入：self <- K2Node_VariableGet_1:E8DBDB134F09998A978E7B807FD4EE31
   - ActivateRepairPresentation

### 8.3 ReturnToLevel3DefaultView

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：7
1. 连接数：7
1. 执行链：ReturnToLevel3DefaultView -> Set TargetArmLength -> SceneComponent.K2_SetWorldLocationAndRotation
1. 关键节点：
   - SceneComponent.K2_SetWorldLocationAndRotation
     输入：execute <- K2Node_VariableSet_1:3BC707704DA3E43B612C9692B4884296
     输入：self <- K2Node_VariableGet_1:E8DBDB134F09998A978E7B807FD4EE31
     输入：NewLocation <- K2Node_VariableGet_6:F5714FBF47059AF964EB179C21B824CC
     输入：NewRotation <- K2Node_VariableGet_5:C6B95B62449F30B9F01A55BFDD5ADFF4
   - Set TargetArmLength
     输入：execute <- K2Node_FunctionEntry_1:220A429E4D58B89C21CC988D993F7D50
     输入：TargetArmLength <- K2Node_VariableGet_0:F606E57846AC2B2EF0ACD8B2A49EE9E7
     输入：self <- K2Node_VariableGet_1:E8DBDB134F09998A978E7B807FD4EE31
   - ReturnToLevel3DefaultView

### 8.4 UpdateDustVisual

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：6
1. 连接数：4
1. 执行链：UpdateDustVisual -> KismetSystemLibrary.PrintString
1. 关键节点：
   - WidgetBlueprintLibrary.DrawTextFormatted
     输入：self = /Script/UMG.Default__WidgetBlueprintLibrary
     输入：FontSize = 16.000000
     输入：FontTypeFace = Regular
     输入：Tint = (R=1.000000,G=1.000000,B=1.000000,A=1.000000)
   - UpdateDustVisual
   - KismetSystemLibrary.PrintString
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：InString = UpdateDustVisual Called
     输入：bPrintToScreen = true
     输入：bPrintToLog = true
   - KismetTextLibrary.Conv_TextToString
     输入：self = /Script/Engine.Default__KismetTextLibrary
     输入：InText <- K2Node_FormatText_0:A522094D408D6DE1A3032BA372EBDCBB
   - KismetSystemLibrary.PrintString
     输入：execute <- K2Node_FunctionEntry_0:E94A690044B052E0297E889A6CF34635
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：InString <- K2Node_CallFunction_4:BB6F1E7343252D7A7131E3A0F897D263
     输入：bPrintToScreen = true

### 8.5 UserConstructionScript

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：1
1. 连接数：0
1. 执行链：UserConstructionScript
1. 关键节点：
   - UserConstructionScript

## 9. 当前可直接确认的结论

1. 本蓝图至少存在上文列出的执行链，可作为后续人工确认和接线补全文档的基础。
1. Widget 树已经从导出中整理出来，后续可以继续补充哪些控件参与了实际逻辑。
1. 变量区已经整理完成，后续重点是确认哪些变量仍然在真实运行链中使用。

## 10. 后续补充建议

1. 如果该蓝图已经过运行验证，建议补充一节“已运行验证链路”。
1. 如果导出中存在历史残留变量、残留事件或旧链路，建议后续在本文档中手动标记。
1. 如果本蓝图和别的蓝图存在强关联，建议在文档中补“对外依赖”。
