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

1. ReceiveBeginPlay
1. ReceiveActorBeginOverlap
1. ReceiveTick

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
   - ActivateLevel3Presentation
   - SetActorHiddenInGame
     输入：execute <- ActivateLevel3Presentation:then
   - SetActorEnableCollision
     输入：execute <- SetActorHiddenInGame:then
   - SetActorTickEnabled
     输入：execute <- SetActorEnableCollision:then
     输入：bEnabled = true

### 8.2 ActivateRepairPresentation

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：7
1. 连接数：7
1. 执行链：ActivateRepairPresentation -> Set TargetArmLength -> SceneComponent.K2_SetWorldLocationAndRotation
1. 关键节点：
   - ActivateRepairPresentation
   - Set TargetArmLength
     输入：execute <- ActivateRepairPresentation:then
     输入：TargetArmLength <- Get RepairArmLength:RepairArmLength
     输入：self <- Get SpringArmComp:SpringArmComp
   - SceneComponent.K2_SetWorldLocationAndRotation
     输入：execute <- Set TargetArmLength:then
     输入：self <- Get SpringArmComp:SpringArmComp
     输入：NewLocation <- Get RepairArmLocation:RepairArmLocation
     输入：NewRotation <- Get RepairArmRotation:RepairArmRotation

### 8.3 ReturnToLevel3DefaultView

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：7
1. 连接数：7
1. 执行链：ReturnToLevel3DefaultView -> Set TargetArmLength -> SceneComponent.K2_SetWorldLocationAndRotation
1. 关键节点：
   - ReturnToLevel3DefaultView
   - Set TargetArmLength
     输入：execute <- ReturnToLevel3DefaultView:then
     输入：TargetArmLength <- Get DefaultArmLength:DefaultArmLength
     输入：self <- Get SpringArmComp:SpringArmComp
   - SceneComponent.K2_SetWorldLocationAndRotation
     输入：execute <- Set TargetArmLength:then
     输入：self <- Get SpringArmComp:SpringArmComp
     输入：NewLocation <- Get DefaultArmLocation:DefaultArmLocation
     输入：NewRotation <- Get DefaultArmRotation:DefaultArmRotation

### 8.4 UpdateDustVisual

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：4
1. 连接数：4
1. 执行链：UpdateDustVisual -> KismetSystemLibrary.PrintString
1. 关键节点：
   - UpdateDustVisual
   - KismetSystemLibrary.PrintString
     输入：execute <- UpdateDustVisual:then
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：InString <- KismetTextLibrary.Conv_TextToString:ReturnValue
     输入：bPrintToScreen = true
   - KismetTextLibrary.Conv_TextToString
     输入：self = /Script/Engine.Default__KismetTextLibrary
     输入：InText <- K2Node_FormatText_0:Result

### 8.5 UpdateOilVisual

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：4
1. 连接数：4
1. 执行链：UpdateOilVisual -> KismetSystemLibrary.PrintString
1. 关键节点：
   - UpdateOilVisual
   - KismetSystemLibrary.PrintString
     输入：execute <- UpdateOilVisual:then
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：InString <- KismetTextLibrary.Conv_TextToString:ReturnValue
     输入：bPrintToScreen = true
   - KismetTextLibrary.Conv_TextToString
     输入：self = /Script/Engine.Default__KismetTextLibrary
     输入：InText <- K2Node_FormatText_0:Result

### 8.6 UserConstructionScript

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
