# BP_ShowcaseModel_Level4

## 1. 蓝图定位

本文档由 Docs/BlueprintExports/BP_ShowcaseModel_Level4.json 自动整理生成，用于记录当前蓝图的真实导出结构。

## 2. 基础信息

1. 蓝图名：BP_ShowcaseModel_Level4
1. 资源路径：/Script/Engine.Blueprint'/Game/UI/BP_ShowcaseModel_Level4.BP_ShowcaseModel_Level4'
1. 蓝图类型：Blueprint
1. 父类：CaissonCeiling.CaissonPawn
1. 生成类：BP_ShowcaseModel_Level4_C

## 3. 当前状态

1. 本文档基于蓝图导出数据自动生成。
1. 事件链、控件树、变量和特殊设置以导出内容为准。
1. 如果某些链路只是历史残留、未接入或未运行验证，需要后续人工补充确认。

## 4. 控件树


## 5. 事件图主链

### 5.2 主执行链

1. Event_PlayOilTransition -> K2Node_Timeline_0 -> MaterialInstanceDynamic.SetScalarParameterValue -> Set CurrentOilVisual01
1. Event_PlayOilTransition -> K2Node_Timeline_0 -> Set CurrentOilVisual01
1. Event_PlayDustTransition -> K2Node_Timeline_1 -> MaterialInstanceDynamic.SetScalarParameterValue -> Set CurrentDustVisual01
1. Event_PlayDustTransition -> K2Node_Timeline_1 -> Set CurrentDustVisual01
1. InitializeLevel3VisualMaterials -> GameplayStatics.GetActorOfClass -> Set RepairRegionRef -> BP_Level3RepairRegion_Dusting_C.GetRepairMeshComponent -> Set RepairMeshRef -> PrimitiveComponent.CreateDynamicMaterialInstance -> Set DustMID_Slot0 -> MaterialInstanceDynamic.SetScalarParameterValue -> Set CurrentDustVisual01 -> Set DustTransitionStart01 -> Set DustTransitionTarget01 -> PrimitiveComponent.CreateDynamicMaterialInstance -> Set OilMID_Slot0 -> MaterialInstanceDynamic.SetScalarParameterValue -> Set CurrentOilVisual01 -> Set OilTransitionStart01 -> Set OilTransitionTarget01 -> PrimitiveComponent.SetMaterial
1. ReceiveBeginPlay -> InitializeLevel3VisualMaterials

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
1. DustMID_Slot0
   类型：object -> /Script/CoreUObject.Class'/Script/Engine.MaterialInstanceDynamic'
   显示名：Dust MID Slot 0
1. OilMID_Slot0
   类型：object -> /Script/CoreUObject.Class'/Script/Engine.MaterialInstanceDynamic'
   显示名：Oil MID Slot 0
1. DustTransitionStart01
   类型：real/double
   显示名：Dust Transition Start 01
1. DustTransitionTarget01
   类型：real/double
   显示名：Dust Transition Target 01
1. OilTransitionStart01
   类型：real/double
   显示名：Oil Transition Start 01
1. OilTransitionTarget01
   类型：real/double
   显示名：Oil Transition Target 01
1. CurrentDustVisual01
   类型：real/double
   显示名：Current Dust Visual 01
1. CurrentOilVisual01
   类型：real/double
   显示名：Current Oil Visual 01
1. RepairRegionRef
   类型：object -> /Script/Engine.BlueprintGeneratedClass'/Game/UI/BP_Level3RepairRegion_Dusting.BP_Level3RepairRegion_Dusting_C'
   显示名：Repair Region Ref
1. RepairMeshRef
   类型：object -> /Script/CoreUObject.Class'/Script/Engine.StaticMeshComponent'
   显示名：Repair Mesh Ref
1. DustMaterial
   类型：object -> /Script/CoreUObject.Class'/Script/Engine.MaterialInterface'
   显示名：Dust Material
1. OilMaterial
   类型：object -> /Script/CoreUObject.Class'/Script/Engine.MaterialInterface'
   显示名：Oil Material


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
     输入：execute <- SetActorEnableCollision:then
     输入：bEnabled = true
   - SetActorEnableCollision
     输入：execute <- SetActorHiddenInGame:then
   - SetActorHiddenInGame
     输入：execute <- ActivateLevel3Presentation:then
   - ActivateLevel3Presentation

### 8.2 ActivateLevel3Presentation_MERGED

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：4
1. 连接数：3
1. 执行链：ActivateLevel3Presentation -> SetActorHiddenInGame -> SetActorEnableCollision -> SetActorTickEnabled
1. 关键节点：
   - SetActorTickEnabled
     输入：execute <- SetActorEnableCollision:then
     输入：bEnabled = true
   - SetActorEnableCollision
     输入：execute <- SetActorHiddenInGame:then
   - SetActorHiddenInGame
     输入：execute <- ActivateLevel3Presentation:then
   - ActivateLevel3Presentation

### 8.3 ActivateRepairPresentation

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：7
1. 连接数：7
1. 执行链：ActivateRepairPresentation -> Set TargetArmLength -> SceneComponent.K2_SetWorldLocationAndRotation
1. 关键节点：
   - SceneComponent.K2_SetWorldLocationAndRotation
     输入：execute <- Set TargetArmLength:then
     输入：self <- Get SpringArmComp:SpringArmComp
     输入：NewLocation <- Get RepairArmLocation:RepairArmLocation
     输入：NewRotation <- Get RepairArmRotation:RepairArmRotation
   - Set TargetArmLength
     输入：execute <- ActivateRepairPresentation:then
     输入：TargetArmLength <- Get RepairArmLength:RepairArmLength
     输入：self <- Get SpringArmComp:SpringArmComp
   - ActivateRepairPresentation

### 8.4 ActivateRepairPresentation_MERGED

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：7
1. 连接数：7
1. 执行链：ActivateRepairPresentation -> Set TargetArmLength -> SceneComponent.K2_SetWorldLocationAndRotation
1. 关键节点：
   - SceneComponent.K2_SetWorldLocationAndRotation
     输入：execute <- Set TargetArmLength:then
     输入：self <- Get SpringArmComp:SpringArmComp
     输入：NewLocation <- Get RepairArmLocation:RepairArmLocation
     输入：NewRotation <- Get RepairArmRotation:RepairArmRotation
   - Set TargetArmLength
     输入：execute <- ActivateRepairPresentation:then
     输入：TargetArmLength <- Get RepairArmLength:RepairArmLength
     输入：self <- Get SpringArmComp:SpringArmComp
   - ActivateRepairPresentation

### 8.5 ApplyDustMaterialToRepairMesh

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：5
1. 连接数：5
1. 执行链：ApplyDustMaterialToRepairMesh -> IsValid -> PrimitiveComponent.SetMaterial
1. 关键节点：
   - PrimitiveComponent.SetMaterial
     输入：execute <- IsValid:Is Valid
     输入：self <- Get RepairMeshRef:RepairMeshRef
     输入：Material <- Get DustMID_Slot0:DustMID_Slot0
   - IsValid
     输入：exec <- ApplyDustMaterialToRepairMesh:then
     输入：InputObject <- Get RepairMeshRef:RepairMeshRef
   - ApplyDustMaterialToRepairMesh

### 8.6 ApplyDustMaterialToRepairMesh_MERGED

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：13
1. 连接数：10
1. 执行链：ApplyDustMaterialToRepairMesh -> K2Node_TunnelBoundary_2 -> K2Node_TunnelBoundary_3 -> K2Node_IfThenElse_15 -> K2Node_TunnelBoundary_0 -> PrimitiveComponent.SetMaterial
1. 执行链：ApplyDustMaterialToRepairMesh -> K2Node_TunnelBoundary_2 -> K2Node_TunnelBoundary_3 -> K2Node_IfThenElse_15 -> K2Node_TunnelBoundary_1
1. 关键节点：
   - KismetSystemLibrary.IsValid
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：Object <- Get RepairMeshRef:RepairMeshRef
   - PrimitiveComponent.SetMaterial
     输入：execute <- K2Node_TunnelBoundary_0:Is Valid2
     输入：self <- Get RepairMeshRef:RepairMeshRef
     输入：Material <- Get DustMID_Slot0:DustMID_Slot0
   - ApplyDustMaterialToRepairMesh

### 8.7 ApplyOilMaterialToRepairMesh

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：5
1. 连接数：5
1. 执行链：ApplyOilMaterialToRepairMesh -> IsValid -> PrimitiveComponent.SetMaterial
1. 关键节点：
   - PrimitiveComponent.SetMaterial
     输入：execute <- IsValid:Is Valid
     输入：self <- Get RepairMeshRef:RepairMeshRef
     输入：Material <- Get OilMID_Slot0:OilMID_Slot0
   - IsValid
     输入：exec <- ApplyOilMaterialToRepairMesh:then
     输入：InputObject <- Get RepairMeshRef:RepairMeshRef
   - ApplyOilMaterialToRepairMesh

### 8.8 ApplyOilMaterialToRepairMesh_MERGED

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：13
1. 连接数：10
1. 执行链：ApplyOilMaterialToRepairMesh -> K2Node_TunnelBoundary_2 -> K2Node_TunnelBoundary_3 -> K2Node_IfThenElse_15 -> K2Node_TunnelBoundary_0 -> PrimitiveComponent.SetMaterial
1. 执行链：ApplyOilMaterialToRepairMesh -> K2Node_TunnelBoundary_2 -> K2Node_TunnelBoundary_3 -> K2Node_IfThenElse_15 -> K2Node_TunnelBoundary_1
1. 关键节点：
   - KismetSystemLibrary.IsValid
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：Object <- Get RepairMeshRef:RepairMeshRef
   - PrimitiveComponent.SetMaterial
     输入：execute <- K2Node_TunnelBoundary_0:Is Valid2
     输入：self <- Get RepairMeshRef:RepairMeshRef
     输入：Material <- Get OilMID_Slot0:OilMID_Slot0
   - ApplyOilMaterialToRepairMesh

### 8.9 EdGraph_0

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：4
1. 连接数：5
1. 关键节点：
   - KismetSystemLibrary.IsValid
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：Object <- K2Node_Tunnel_90:InputObject

### 8.10 EdGraph_1

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：4
1. 连接数：5
1. 关键节点：
   - KismetSystemLibrary.IsValid
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：Object <- K2Node_Tunnel_90:InputObject

### 8.11 EdGraph_2

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：4
1. 连接数：5
1. 关键节点：
   - KismetSystemLibrary.IsValid
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：Object <- K2Node_Tunnel_90:InputObject

### 8.12 EdGraph_3

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：4
1. 连接数：5
1. 关键节点：
   - KismetSystemLibrary.IsValid
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：Object <- K2Node_Tunnel_90:InputObject

### 8.13 Event_PlayDustTransition

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：Event_PlayDustTransition -> ExecuteUbergraph_BP_ShowcaseModel_Level4
1. 关键节点：
   - ExecuteUbergraph_BP_ShowcaseModel_Level4
     输入：execute <- Event_PlayDustTransition:then
   - Event_PlayDustTransition

### 8.14 Event_PlayOilTransition

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：Event_PlayOilTransition -> ExecuteUbergraph_BP_ShowcaseModel_Level4
1. 关键节点：
   - ExecuteUbergraph_BP_ShowcaseModel_Level4
     输入：execute <- Event_PlayOilTransition:then
   - Event_PlayOilTransition

### 8.15 ExecuteUbergraph_BP_ShowcaseModel_Level4

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：61
1. 连接数：56
1. 执行链：TL_DustTransition_5s__FinishedFunc -> Set CurrentDustVisual01
1. 执行链：TL_DustTransition_5s__UpdateFunc -> MaterialInstanceDynamic.SetScalarParameterValue -> Set CurrentDustVisual01
1. 执行链：TL_OilTransition_5s__FinishedFunc -> Set CurrentOilVisual01
1. 执行链：TL_OilTransition_5s__UpdateFunc -> MaterialInstanceDynamic.SetScalarParameterValue -> Set CurrentOilVisual01
1. 执行链：ExecuteUbergraph_BP_ShowcaseModel_Level4
1. 执行链：Event_PlayOilTransition -> TimelineComponent.PlayFromStart
1. 执行链：Event_PlayDustTransition -> TimelineComponent.PlayFromStart
1. 执行链：InitializeLevel3VisualMaterials -> GameplayStatics.GetActorOfClass -> Set RepairRegionRef -> BP_Level3RepairRegion_Dusting_C.GetRepairMeshComponent -> Set RepairMeshRef -> PrimitiveComponent.CreateDynamicMaterialInstance -> Set DustMID_Slot0 -> MaterialInstanceDynamic.SetScalarParameterValue -> Set CurrentDustVisual01 -> Set DustTransitionStart01 -> Set DustTransitionTarget01 -> PrimitiveComponent.CreateDynamicMaterialInstance -> Set OilMID_Slot0 -> MaterialInstanceDynamic.SetScalarParameterValue -> Set CurrentOilVisual01 -> Set OilTransitionStart01 -> Set OilTransitionTarget01 -> PrimitiveComponent.SetMaterial
1. 执行链：ReceiveBeginPlay -> InitializeLevel3VisualMaterials
1. 关键节点：
   - TimelineComponent.PlayFromStart
     输入：execute <- Event_PlayDustTransition:then
     输入：self <- Get TL_DustTransition_5s:TL_DustTransition_5s
   - TimelineComponent.PlayFromStart
     输入：execute <- Event_PlayOilTransition:then
     输入：self <- Get TL_OilTransition_5s:TL_OilTransition_5s
   - ExecuteUbergraph_BP_ShowcaseModel_Level4
   - Set RepairRegionRef
     输入：execute <- GameplayStatics.GetActorOfClass:then
     输入：RepairRegionRef <- GameplayStatics.GetActorOfClass:ReturnValue
   - GameplayStatics.GetActorOfClass
     输入：execute <- InitializeLevel3VisualMaterials:then
     输入：self = /Script/Engine.Default__GameplayStatics
     输入：ActorClass = /Game/UI/BP_Level3RepairRegion_Dusting.BP_Level3RepairRegion_Dusting_C
   - PrimitiveComponent.SetMaterial
     输入：execute <- Set OilTransitionTarget01:then
     输入：self <- Get RepairMeshRef:RepairMeshRef
     输入：Material <- Get DustMID_Slot0:DustMID_Slot0
   - Set OilTransitionTarget01
     输入：execute <- Set OilTransitionStart01:then
   - Set OilTransitionStart01
     输入：execute <- Set CurrentOilVisual01:then
   - Set CurrentOilVisual01
     输入：execute <- MaterialInstanceDynamic.SetScalarParameterValue:then
   - MaterialInstanceDynamic.SetScalarParameterValue
     输入：execute <- Set OilMID_Slot0:then
     输入：self <- Get OilMID_Slot0:OilMID_Slot0
     输入：ParameterName = OilBlend01
   - Set OilMID_Slot0
     输入：execute <- PrimitiveComponent.CreateDynamicMaterialInstance:then
     输入：OilMID_Slot0 <- PrimitiveComponent.CreateDynamicMaterialInstance:ReturnValue
   - PrimitiveComponent.CreateDynamicMaterialInstance
     输入：execute <- Set DustTransitionTarget01:then
     输入：self <- Get RepairMeshRef:RepairMeshRef
     输入：SourceMaterial <- Get OilMaterial:OilMaterial

### 8.16 InitializeLevel3VisualMaterials

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：InitializeLevel3VisualMaterials -> ExecuteUbergraph_BP_ShowcaseModel_Level4
1. 关键节点：
   - ExecuteUbergraph_BP_ShowcaseModel_Level4
     输入：execute <- InitializeLevel3VisualMaterials:then
   - InitializeLevel3VisualMaterials

### 8.17 ReceiveBeginPlay

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：ReceiveBeginPlay -> ExecuteUbergraph_BP_ShowcaseModel_Level4
1. 关键节点：
   - ExecuteUbergraph_BP_ShowcaseModel_Level4
     输入：execute <- ReceiveBeginPlay:then
   - ReceiveBeginPlay

### 8.18 ReturnToLevel3DefaultView

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：7
1. 连接数：7
1. 执行链：ReturnToLevel3DefaultView -> Set TargetArmLength -> SceneComponent.K2_SetWorldLocationAndRotation
1. 关键节点：
   - SceneComponent.K2_SetWorldLocationAndRotation
     输入：execute <- Set TargetArmLength:then
     输入：self <- Get SpringArmComp:SpringArmComp
     输入：NewLocation <- Get DefaultArmLocation:DefaultArmLocation
     输入：NewRotation <- Get DefaultArmRotation:DefaultArmRotation
   - Set TargetArmLength
     输入：execute <- ReturnToLevel3DefaultView:then
     输入：TargetArmLength <- Get DefaultArmLength:DefaultArmLength
     输入：self <- Get SpringArmComp:SpringArmComp
   - ReturnToLevel3DefaultView

### 8.19 ReturnToLevel3DefaultView_MERGED

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：7
1. 连接数：7
1. 执行链：ReturnToLevel3DefaultView -> Set TargetArmLength -> SceneComponent.K2_SetWorldLocationAndRotation
1. 关键节点：
   - SceneComponent.K2_SetWorldLocationAndRotation
     输入：execute <- Set TargetArmLength:then
     输入：self <- Get SpringArmComp:SpringArmComp
     输入：NewLocation <- Get DefaultArmLocation:DefaultArmLocation
     输入：NewRotation <- Get DefaultArmRotation:DefaultArmRotation
   - Set TargetArmLength
     输入：execute <- ReturnToLevel3DefaultView:then
     输入：TargetArmLength <- Get DefaultArmLength:DefaultArmLength
     输入：self <- Get SpringArmComp:SpringArmComp
   - ReturnToLevel3DefaultView

### 8.20 TL_DustTransition_5s__FinishedFunc

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：TL_DustTransition_5s__FinishedFunc -> ExecuteUbergraph_BP_ShowcaseModel_Level4
1. 关键节点：
   - ExecuteUbergraph_BP_ShowcaseModel_Level4
     输入：execute <- TL_DustTransition_5s__FinishedFunc:then
   - TL_DustTransition_5s__FinishedFunc

### 8.21 TL_DustTransition_5s__UpdateFunc

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：TL_DustTransition_5s__UpdateFunc -> ExecuteUbergraph_BP_ShowcaseModel_Level4
1. 关键节点：
   - ExecuteUbergraph_BP_ShowcaseModel_Level4
     输入：execute <- TL_DustTransition_5s__UpdateFunc:then
   - TL_DustTransition_5s__UpdateFunc

### 8.22 TL_OilTransition_5s__FinishedFunc

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：TL_OilTransition_5s__FinishedFunc -> ExecuteUbergraph_BP_ShowcaseModel_Level4
1. 关键节点：
   - ExecuteUbergraph_BP_ShowcaseModel_Level4
     输入：execute <- TL_OilTransition_5s__FinishedFunc:then
   - TL_OilTransition_5s__FinishedFunc

### 8.23 TL_OilTransition_5s__UpdateFunc

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：2
1. 连接数：1
1. 执行链：TL_OilTransition_5s__UpdateFunc -> ExecuteUbergraph_BP_ShowcaseModel_Level4
1. 关键节点：
   - ExecuteUbergraph_BP_ShowcaseModel_Level4
     输入：execute <- TL_OilTransition_5s__UpdateFunc:then
   - TL_OilTransition_5s__UpdateFunc

### 8.24 UpdateDustVisual

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：9
1. 连接数：10
1. 执行链：UpdateDustVisual -> IsValid -> MaterialInstanceDynamic.K2_GetScalarParameterValue -> Set DustTransitionStart01 -> Set DustTransitionTarget01 -> ApplyDustMaterialToRepairMesh -> Event_PlayDustTransition
1. 关键节点：
   - Event_PlayDustTransition
     输入：execute <- ApplyDustMaterialToRepairMesh:then
   - ApplyDustMaterialToRepairMesh
     输入：execute <- Set DustTransitionTarget01:then
   - Set DustTransitionTarget01
     输入：execute <- Set DustTransitionStart01:then
     输入：DustTransitionTarget01 <- Get NewParamDustConcentration01:NewParamDustConcentration01
   - Set DustTransitionStart01
     输入：execute <- MaterialInstanceDynamic.K2_GetScalarParameterValue:then
     输入：DustTransitionStart01 <- MaterialInstanceDynamic.K2_GetScalarParameterValue:ReturnValue
   - MaterialInstanceDynamic.K2_GetScalarParameterValue
     输入：execute <- IsValid:Is Valid
     输入：self <- Get DustMID_Slot0:DustMID_Slot0
     输入：ParameterName = DustConcentration01
   - IsValid
     输入：exec <- UpdateDustVisual:then
     输入：InputObject <- Get DustMID_Slot0:DustMID_Slot0
   - UpdateDustVisual

### 8.25 UpdateDustVisual_MERGED

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：17
1. 连接数：15
1. 执行链：UpdateDustVisual -> K2Node_TunnelBoundary_2 -> K2Node_TunnelBoundary_3 -> K2Node_IfThenElse_15 -> K2Node_TunnelBoundary_0 -> MaterialInstanceDynamic.K2_GetScalarParameterValue -> Set DustTransitionStart01 -> Set DustTransitionTarget01 -> ApplyDustMaterialToRepairMesh -> Event_PlayDustTransition
1. 执行链：UpdateDustVisual -> K2Node_TunnelBoundary_2 -> K2Node_TunnelBoundary_3 -> K2Node_IfThenElse_15 -> K2Node_TunnelBoundary_1
1. 关键节点：
   - KismetSystemLibrary.IsValid
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：Object <- Get DustMID_Slot0:DustMID_Slot0
   - Event_PlayDustTransition
     输入：execute <- ApplyDustMaterialToRepairMesh:then
   - ApplyDustMaterialToRepairMesh
     输入：execute <- Set DustTransitionTarget01:then
   - Set DustTransitionTarget01
     输入：execute <- Set DustTransitionStart01:then
     输入：DustTransitionTarget01 <- Get NewParamDustConcentration01:NewParamDustConcentration01
   - Set DustTransitionStart01
     输入：execute <- MaterialInstanceDynamic.K2_GetScalarParameterValue:then
     输入：DustTransitionStart01 <- MaterialInstanceDynamic.K2_GetScalarParameterValue:ReturnValue
   - MaterialInstanceDynamic.K2_GetScalarParameterValue
     输入：execute <- K2Node_TunnelBoundary_0:Is Valid2
     输入：self <- Get DustMID_Slot0:DustMID_Slot0
     输入：ParameterName = DustConcentration01
   - UpdateDustVisual

### 8.26 UpdateOilVisual

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：9
1. 连接数：10
1. 执行链：UpdateOilVisual -> IsValid -> MaterialInstanceDynamic.K2_GetScalarParameterValue -> Set OilTransitionStart01 -> Set OilTransitionTarget01 -> ApplyOilMaterialToRepairMesh -> Event_PlayOilTransition
1. 关键节点：
   - Event_PlayOilTransition
     输入：execute <- ApplyOilMaterialToRepairMesh:then
   - ApplyOilMaterialToRepairMesh
     输入：execute <- Set OilTransitionTarget01:then
   - Set OilTransitionTarget01
     输入：execute <- Set OilTransitionStart01:then
     输入：OilTransitionTarget01 <- Get TargetOilBlend01:TargetOilBlend01
   - Set OilTransitionStart01
     输入：execute <- MaterialInstanceDynamic.K2_GetScalarParameterValue:then
     输入：OilTransitionStart01 <- MaterialInstanceDynamic.K2_GetScalarParameterValue:ReturnValue
   - MaterialInstanceDynamic.K2_GetScalarParameterValue
     输入：execute <- IsValid:Is Valid
     输入：self <- Get OilMID_Slot0:OilMID_Slot0
     输入：ParameterName = OilBlend01
   - IsValid
     输入：exec <- UpdateOilVisual:then
     输入：InputObject <- Get OilMID_Slot0:OilMID_Slot0
   - UpdateOilVisual

### 8.27 UpdateOilVisual_MERGED

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：17
1. 连接数：15
1. 执行链：UpdateOilVisual -> K2Node_TunnelBoundary_2 -> K2Node_TunnelBoundary_3 -> K2Node_IfThenElse_15 -> K2Node_TunnelBoundary_0 -> MaterialInstanceDynamic.K2_GetScalarParameterValue -> Set OilTransitionStart01 -> Set OilTransitionTarget01 -> ApplyOilMaterialToRepairMesh -> Event_PlayOilTransition
1. 执行链：UpdateOilVisual -> K2Node_TunnelBoundary_2 -> K2Node_TunnelBoundary_3 -> K2Node_IfThenElse_15 -> K2Node_TunnelBoundary_1
1. 关键节点：
   - KismetSystemLibrary.IsValid
     输入：self = /Script/Engine.Default__KismetSystemLibrary
     输入：Object <- Get OilMID_Slot0:OilMID_Slot0
   - Event_PlayOilTransition
     输入：execute <- ApplyOilMaterialToRepairMesh:then
   - ApplyOilMaterialToRepairMesh
     输入：execute <- Set OilTransitionTarget01:then
   - Set OilTransitionTarget01
     输入：execute <- Set OilTransitionStart01:then
     输入：OilTransitionTarget01 <- Get TargetOilBlend01:TargetOilBlend01
   - Set OilTransitionStart01
     输入：execute <- MaterialInstanceDynamic.K2_GetScalarParameterValue:then
     输入：OilTransitionStart01 <- MaterialInstanceDynamic.K2_GetScalarParameterValue:ReturnValue
   - MaterialInstanceDynamic.K2_GetScalarParameterValue
     输入：execute <- K2Node_TunnelBoundary_0:Is Valid2
     输入：self <- Get OilMID_Slot0:OilMID_Slot0
     输入：ParameterName = OilBlend01
   - UpdateOilVisual

### 8.28 UserConstructionScript

1. 图类型：BlueprintGraph.EdGraphSchema_K2
1. 模块数：1
1. 连接数：0
1. 执行链：UserConstructionScript
1. 关键节点：
   - UserConstructionScript

### 8.29 UserConstructionScript_MERGED

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
