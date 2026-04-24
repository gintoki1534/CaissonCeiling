# BP_ShowcaseModel

## 1. 蓝图定位

本文档由 Docs/BlueprintExports/BP_ShowcaseModel.json 自动整理生成，用于记录当前蓝图的真实导出结构。

## 2. 基础信息

1. 蓝图名：BP_ShowcaseModel
1. 资源路径：/Script/Engine.Blueprint'/Game/UI/BP_ShowcaseModel.BP_ShowcaseModel'
1. 蓝图类型：Blueprint
1. 父类：CaissonCeiling.CaissonPawn
1. 生成类：BP_ShowcaseModel_C

## 3. 当前状态

1. 本文档基于蓝图导出数据自动生成。
1. 事件链、控件树、变量和特殊设置以导出内容为准。
1. 如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。

## 4. 控件树


## 5. 事件图主链

### 5.2 主执行链

1. FocusTargetById -> Set bReturningToDefault -> Set StartRot -> Set StartLoc -> Set StartField -> K2Node_IfThenElse_0 -> K2Node_IfThenElse_2 -> K2Node_IfThenElse_1 -> Set TargetLoc -> Set TargetRot -> Set TargetField -> K2Node_Timeline_1 -> SceneComponent.K2_SetWorldLocation -> SceneComponent.K2_SetWorldRotation -> CameraComponent.SetFieldOfView
1. FocusTargetById -> Set bReturningToDefault -> Set StartRot -> Set StartLoc -> Set StartField -> K2Node_IfThenElse_0 -> K2Node_IfThenElse_2 -> K2Node_IfThenElse_1 -> Set TargetLoc -> Set TargetRot -> Set TargetField -> K2Node_Timeline_1 -> K2Node_IfThenElse_3 -> K2Node_CallDelegate_0
1. FocusTargetById -> Set bReturningToDefault -> Set StartRot -> Set StartLoc -> Set StartField -> K2Node_IfThenElse_0 -> K2Node_IfThenElse_2 -> Set TargetLoc -> Set TargetRot -> Set TargetField -> K2Node_Timeline_1 -> SceneComponent.K2_SetWorldLocation -> SceneComponent.K2_SetWorldRotation -> CameraComponent.SetFieldOfView
1. FocusTargetById -> Set bReturningToDefault -> Set StartRot -> Set StartLoc -> Set StartField -> K2Node_IfThenElse_0 -> K2Node_IfThenElse_2 -> Set TargetLoc -> Set TargetRot -> Set TargetField -> K2Node_Timeline_1 -> K2Node_IfThenElse_3 -> K2Node_CallDelegate_0
1. FocusTargetById -> Set bReturningToDefault -> Set StartRot -> Set StartLoc -> Set StartField -> K2Node_IfThenElse_0 -> Set TargetLoc -> Set TargetRot -> Set TargetField -> K2Node_Timeline_1 -> SceneComponent.K2_SetWorldLocation -> SceneComponent.K2_SetWorldRotation -> CameraComponent.SetFieldOfView
1. FocusTargetById -> Set bReturningToDefault -> Set StartRot -> Set StartLoc -> Set StartField -> K2Node_IfThenElse_0 -> Set TargetLoc -> Set TargetRot -> Set TargetField -> K2Node_Timeline_1 -> K2Node_IfThenElse_3 -> K2Node_CallDelegate_0
1. ReturnToDefaultView -> Set bReturningToDefault -> Set TargetLoc -> Set TargetRot -> Set StartLoc -> Set StartRot -> Set StartField -> Set TargetField -> K2Node_Timeline_1 -> SceneComponent.K2_SetWorldLocation -> SceneComponent.K2_SetWorldRotation -> CameraComponent.SetFieldOfView
1. ReturnToDefaultView -> Set bReturningToDefault -> Set TargetLoc -> Set TargetRot -> Set StartLoc -> Set StartRot -> Set StartField -> Set TargetField -> K2Node_Timeline_1 -> K2Node_IfThenElse_3 -> K2Node_CallDelegate_0

## 6. 关键变量

### 6.1 自定义变量

1. StartLoc
   类型：struct -> /Script/CoreUObject.ScriptStruct'/Script/CoreUObject.Vector'
   显示名：Start Loc
1. StartRot
   类型：struct -> /Script/CoreUObject.ScriptStruct'/Script/CoreUObject.Rotator'
   显示名：Start Rot
1. TargetLoc
   类型：struct -> /Script/CoreUObject.ScriptStruct'/Script/CoreUObject.Vector'
   显示名：Target Loc
1. TargetRot
   类型：struct -> /Script/CoreUObject.ScriptStruct'/Script/CoreUObject.Rotator'
   显示名：Target Rot
1. bReturningToDefault
   类型：bool
   显示名：Returning to Default
1. OnReturnToDefaultFinished
   类型：mcdelegate
   显示名：On Return to Default Finished
1. StartField
   类型：real/double
   显示名：Start Field
1. TargetField
   类型：real/double
   显示名：Target Field


## 7. 特殊设置

1. 当前导出中未发现额外特殊设置。

## 8. 其他图表

### 8.1 DeactivateLevel2Presentation

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：4
1. 连接数：3
1. 执行链：DeactivateLevel2Presentation -> SetActorHiddenInGame -> SetActorEnableCollision -> SetActorTickEnabled
1. 关键节点：
   - DeactivateLevel2Presentation
   - SetActorHiddenInGame
     输入：execute <- DeactivateLevel2Presentation:then
     输入：bNewHidden = true
   - SetActorEnableCollision
     输入：execute <- SetActorHiddenInGame:then
   - SetActorTickEnabled
     输入：execute <- SetActorEnableCollision:then

### 8.2 DeactivateLevel2Presentation_MERGED

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：4
1. 连接数：3
1. 执行链：DeactivateLevel2Presentation -> SetActorHiddenInGame -> SetActorEnableCollision -> SetActorTickEnabled
1. 关键节点：
   - DeactivateLevel2Presentation
   - SetActorHiddenInGame
     输入：execute <- DeactivateLevel2Presentation:then
     输入：bNewHidden = true
   - SetActorEnableCollision
     输入：execute <- SetActorHiddenInGame:then
   - SetActorTickEnabled
     输入：execute <- SetActorEnableCollision:then

### 8.3 ExecuteUbergraph_BP_ShowcaseModel

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：74
1. 连接数：83
1. 执行链：FocusTargetById -> Set bReturningToDefault -> Set StartRot -> Set StartLoc -> Set StartField -> K2Node_IfThenElse_0 -> K2Node_IfThenElse_2 -> K2Node_IfThenElse_1 -> Set TargetLoc -> Set TargetRot -> Set TargetField -> TimelineComponent.PlayFromStart
1. 执行链：FocusTargetById -> Set bReturningToDefault -> Set StartRot -> Set StartLoc -> Set StartField -> K2Node_IfThenElse_0 -> K2Node_IfThenElse_2 -> Set TargetLoc -> Set TargetRot -> Set TargetField -> TimelineComponent.PlayFromStart
1. 执行链：FocusTargetById -> Set bReturningToDefault -> Set StartRot -> Set StartLoc -> Set StartField -> K2Node_IfThenElse_0 -> Set TargetLoc -> Set TargetRot -> Set TargetField -> TimelineComponent.PlayFromStart
1. 执行链：ReturnToDefaultView -> Set bReturningToDefault -> Set TargetLoc -> Set TargetRot -> Set StartLoc -> Set StartRot -> Set StartField -> Set TargetField -> TimelineComponent.PlayFromStart
1. 执行链：ExecuteUbergraph_BP_ShowcaseModel
1. 执行链：TL_CameraMove__UpdateFunc -> SceneComponent.K2_SetWorldLocation -> SceneComponent.K2_SetWorldRotation -> CameraComponent.SetFieldOfView
1. 执行链：TL_CameraMove__FinishedFunc -> K2Node_IfThenElse_3 -> K2Node_CallDelegate_0
1. 关键节点：
   - KismetMathLibrary.VLerp
     输入：self = /Script/Engine.Default__KismetMathLibrary
     输入：A <- Get StartLoc:StartLoc
     输入：B <- Get TargetLoc:TargetLoc
     输入：Alpha <- Get TL_CameraMove_Alpha_461928E5476FEC03CAB8BFB1C102CD37:TL_CameraMove_Alpha_461928E5476FEC03CAB8BFB1C102CD37
   - KismetMathLibrary.RLerp
     输入：self = /Script/Engine.Default__KismetMathLibrary
     输入：A <- Get StartRot:StartRot
     输入：B <- Get TargetRot:TargetRot
     输入：Alpha <- Get TL_CameraMove_Alpha_461928E5476FEC03CAB8BFB1C102CD37:TL_CameraMove_Alpha_461928E5476FEC03CAB8BFB1C102CD37
   - SceneComponent.K2_SetWorldLocation
     输入：execute <- TL_CameraMove__UpdateFunc:then
     输入：self <- Get CameraComp:CameraComp
     输入：NewLocation <- KismetMathLibrary.VLerp:ReturnValue
   - SceneComponent.K2_SetWorldRotation
     输入：execute <- SceneComponent.K2_SetWorldLocation:then
     输入：self <- Get CameraComp:CameraComp
     输入：NewRotation <- KismetMathLibrary.RLerp:ReturnValue
   - FocusTargetById
   - SceneComponent.K2_GetComponentLocation
     输入：self <- Get CameraComp:CameraComp
   - Set StartLoc
     输入：execute <- Set StartRot:then
     输入：StartLoc <- SceneComponent.K2_GetComponentLocation:ReturnValue
   - SceneComponent.K2_GetComponentRotation
     输入：self <- Get CameraComp:CameraComp
   - Set StartRot
     输入：execute <- Set bReturningToDefault:then
     输入：StartRot <- SceneComponent.K2_GetComponentRotation:ReturnValue
   - Set bReturningToDefault
     输入：execute <- FocusTargetById:then
   - SceneComponent.K2_GetComponentLocation
     输入：self <- Get CamAnchor_TiangongTower:CamAnchor_TiangongTower
   - Set TargetLoc
     输入：execute <- K2Node_IfThenElse_0:then
     输入：TargetLoc <- SceneComponent.K2_GetComponentLocation:ReturnValue

### 8.4 FocusTargetById

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：3
1. 连接数：3
1. 执行链：FocusTargetById -> SetVariableOnPersistentFrame -> ExecuteUbergraph_BP_ShowcaseModel
1. 关键节点：
   - FocusTargetById
   - ExecuteUbergraph_BP_ShowcaseModel
     输入：execute <- SetVariableOnPersistentFrame:then

### 8.5 OnReturnToDefaultFinished

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：1
1. 连接数：0
1. 执行链：OnReturnToDefaultFinished
1. 关键节点：
   - OnReturnToDefaultFinished

### 8.6 OnReturnToDefaultFinished_MERGED

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：1
1. 连接数：0
1. 执行链：OnReturnToDefaultFinished
1. 关键节点：
   - OnReturnToDefaultFinished

### 8.7 ReturnToDefaultView

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：ReturnToDefaultView -> ExecuteUbergraph_BP_ShowcaseModel
1. 关键节点：
   - ReturnToDefaultView
   - ExecuteUbergraph_BP_ShowcaseModel
     输入：execute <- ReturnToDefaultView:then

### 8.8 TL_CameraMove__FinishedFunc

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：TL_CameraMove__FinishedFunc -> ExecuteUbergraph_BP_ShowcaseModel
1. 关键节点：
   - TL_CameraMove__FinishedFunc
   - ExecuteUbergraph_BP_ShowcaseModel
     输入：execute <- TL_CameraMove__FinishedFunc:then

### 8.9 TL_CameraMove__UpdateFunc

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：TL_CameraMove__UpdateFunc -> ExecuteUbergraph_BP_ShowcaseModel
1. 关键节点：
   - TL_CameraMove__UpdateFunc
   - ExecuteUbergraph_BP_ShowcaseModel
     输入：execute <- TL_CameraMove__UpdateFunc:then

### 8.10 UserConstructionScript

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：1
1. 连接数：0
1. 执行链：UserConstructionScript
1. 关键节点：
   - UserConstructionScript

### 8.11 UserConstructionScript_MERGED

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
