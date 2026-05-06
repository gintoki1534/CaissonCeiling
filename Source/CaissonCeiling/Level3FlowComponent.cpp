#include "Level3FlowComponent.h"

#include "Components/PrimitiveComponent.h"
#include "EngineUtils.h"
#include "Level3RepairAreaComponent.h"
#include "TimerManager.h"

namespace
{
ULevel3RepairAreaComponent* FindRepairAreaOnActorHierarchy(AActor* StartActor)
{
	TSet<AActor*> VisitedActors;
	TArray<AActor*> PendingActors;

	if (StartActor)
	{
		PendingActors.Add(StartActor);
	}

	while (PendingActors.Num() > 0)
	{
		AActor* CurrentActor = PendingActors.Pop(EAllowShrinking::No);
		if (!CurrentActor || VisitedActors.Contains(CurrentActor))
		{
			continue;
		}

		VisitedActors.Add(CurrentActor);
		if (ULevel3RepairAreaComponent* RepairArea = CurrentActor->FindComponentByClass<ULevel3RepairAreaComponent>())
		{
			return RepairArea;
		}

		auto AddRelatedActor = [&PendingActors, &VisitedActors](AActor* RelatedActor)
		{
			if (RelatedActor && !VisitedActors.Contains(RelatedActor))
			{
				PendingActors.Add(RelatedActor);
			}
		};

		AddRelatedActor(CurrentActor->GetOwner());
		AddRelatedActor(CurrentActor->GetAttachParentActor());
		AddRelatedActor(CurrentActor->GetParentActor());
	}

	return nullptr;
}

void GetEnabledRepairAreas(UWorld* World, TArray<ULevel3RepairAreaComponent*>& OutRepairAreas)
{
	if (!World)
	{
		return;
	}

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		TArray<ULevel3RepairAreaComponent*> ActorRepairAreas;
		It->GetComponents<ULevel3RepairAreaComponent>(ActorRepairAreas);
		for (ULevel3RepairAreaComponent* RepairArea : ActorRepairAreas)
		{
			if (RepairArea && RepairArea->bAreaEnabled)
			{
				OutRepairAreas.Add(RepairArea);
			}
		}
	}
}

bool IsRepairMeshPrimitive(const UPrimitiveComponent* PrimitiveComponent)
{
	if (!PrimitiveComponent)
	{
		return false;
	}

	const FName ComponentName = PrimitiveComponent->GetFName();
	return ComponentName == TEXT("SM_RepairMesh")
		|| ComponentName == TEXT("PreviewMeshComp");
}

bool TraceRepairAreaMesh(ULevel3RepairAreaComponent* RepairArea, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& OutHitResult)
{
	AActor* AreaOwner = RepairArea ? RepairArea->GetOwner() : nullptr;
	if (!AreaOwner)
	{
		return false;
	}

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	AreaOwner->GetComponents<UPrimitiveComponent>(PrimitiveComponents);

	FHitResult BestHitResult;
	double BestDistanceSq = TNumericLimits<double>::Max();

	FCollisionQueryParams QueryParams(FName(TEXT("Level3RepairMeshTrace")), true);
	QueryParams.bTraceComplex = true;

	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		if (!IsRepairMeshPrimitive(PrimitiveComponent))
		{
			continue;
		}

		FHitResult ComponentHitResult;
		if (!PrimitiveComponent->LineTraceComponent(ComponentHitResult, TraceStart, TraceEnd, QueryParams))
		{
			continue;
		}

		const FVector ImpactPoint(ComponentHitResult.ImpactPoint);
		const double DistanceSq = FVector::DistSquared(TraceStart, ImpactPoint);
		if (DistanceSq < BestDistanceSq)
		{
			BestDistanceSq = DistanceSq;
			BestHitResult = ComponentHitResult;
			BestHitResult.TraceStart = TraceStart;
			BestHitResult.TraceEnd = TraceEnd;
			BestHitResult.Distance = FVector::Distance(TraceStart, ImpactPoint);
			BestHitResult.Component = PrimitiveComponent;
		}
	}

	if (BestDistanceSq < TNumericLimits<double>::Max())
	{
		OutHitResult = BestHitResult;
		return true;
	}

	return false;
}

ULevel3RepairAreaComponent* FindRepairAreaByRepairMeshTrace(UWorld* World, const FVector& TraceStart, const FVector& TraceEnd, FHitResult& OutRepairMeshHit)
{
	TArray<ULevel3RepairAreaComponent*> EnabledRepairAreas;
	GetEnabledRepairAreas(World, EnabledRepairAreas);
	if (EnabledRepairAreas.Num() == 0)
	{
		return nullptr;
	}

	if (TraceStart.Equals(TraceEnd))
	{
		return nullptr;
	}

	const FVector RayDirection = (TraceEnd - TraceStart).GetSafeNormal();
	if (RayDirection.IsNearlyZero())
	{
		return nullptr;
	}

	ULevel3RepairAreaComponent* BestRepairArea = nullptr;
	FHitResult BestHitResult;
	double BestDistanceSq = TNumericLimits<double>::Max();

	for (ULevel3RepairAreaComponent* RepairArea : EnabledRepairAreas)
	{
		FHitResult RepairMeshHit;
		if (TraceRepairAreaMesh(RepairArea, TraceStart, TraceEnd, RepairMeshHit))
		{
			const double DistanceSq = FVector::DistSquared(TraceStart, FVector(RepairMeshHit.ImpactPoint));
			if (DistanceSq < BestDistanceSq)
			{
				BestDistanceSq = DistanceSq;
				BestRepairArea = RepairArea;
				BestHitResult = RepairMeshHit;
			}
		}
	}

	if (BestRepairArea)
	{
		OutRepairMeshHit = BestHitResult;
		return BestRepairArea;
	}

	return nullptr;
}
}

ULevel3FlowComponent::ULevel3FlowComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bLevel3SessionActive = false;
	CompletionThreshold = 2.0f;
	MinMetricValue = -2.0f;
	MaxMetricValue = 2.0f;
	VisualTransitionDurationSeconds = 5.0f;

	DustingStageConfig.SubStage = ELevel3SubStage::Dusting;
	DustingStageConfig.StageTitle = FText::FromString(TEXT("除尘"));
	DustingStageConfig.ToolPanelTitle = FText::FromString(TEXT("除尘工具"));
	DustingStageConfig.ResultTitle = FText::FromString(TEXT("除尘完成"));
	DustingStageConfig.VisibleToolCount = 5;
	DustingStageConfig.RequiredMetricValue = CompletionThreshold;
	DustingStageConfig.InitialDustReveal01 = 0.0f;
	DustingStageConfig.InitialDustConcentration01 = 1.0f;
	DustingStageConfig.InitialOilBlend01 = 0.0f;
	DustingStageConfig.bUsesDustVisual = true;
	DustingStageConfig.bUsesOilVisual = false;

	FLevel3ToolSpec DustTool1;
	DustTool1.ToolId = TEXT("Tool_1_WideBrush");
	DustTool1.DisplayName = FText::FromString(TEXT("宽口羊毛排刷"));
	DustTool1.CleanlinessDelta = 2.0f;
	DustTool1.AestheticsDelta = -1.0f;
	DustTool1.ToolSlotIndex = 1;
	DustingStageConfig.Tools.Add(DustTool1);

	FLevel3ToolSpec DustTool2;
	DustTool2.ToolId = TEXT("Tool_2_RoundSoftBrush");
	DustTool2.DisplayName = FText::FromString(TEXT("圆头除尘软刷"));
	DustTool2.CleanlinessDelta = 1.0f;
	DustTool2.AestheticsDelta = 1.0f;
	DustTool2.ToolSlotIndex = 2;
	DustingStageConfig.Tools.Add(DustTool2);

	FLevel3ToolSpec DustTool3;
	DustTool3.ToolId = TEXT("Tool_3_FineLineBrush");
	DustTool3.DisplayName = FText::FromString(TEXT("极细勾线描刷"));
	DustTool3.CleanlinessDelta = -1.0f;
	DustTool3.AestheticsDelta = 2.0f;
	DustTool3.ToolSlotIndex = 3;
	DustingStageConfig.Tools.Add(DustTool3);

	FLevel3ToolSpec DustTool4;
	DustTool4.ToolId = TEXT("Tool_4_CurvedGapBrush");
	DustTool4.DisplayName = FText::FromString(TEXT("弯曲柄刷"));
	DustTool4.IntegrityDelta = 2.0f;
	DustTool4.AestheticsDelta = -1.0f;
	DustTool4.ToolSlotIndex = 4;
	DustingStageConfig.Tools.Add(DustTool4);

	FLevel3ToolSpec DustTool5;
	DustTool5.ToolId = TEXT("Tool_5_FinishingBrush");
	DustTool5.DisplayName = FText::FromString(TEXT("墩头打磨油刷"));
	DustTool5.IntegrityDelta = 1.0f;
	DustTool5.AestheticsDelta = 1.0f;
	DustTool5.ToolSlotIndex = 5;
	DustingStageConfig.Tools.Add(DustTool5);

	OilingStageConfig.SubStage = ELevel3SubStage::Oiling;
	OilingStageConfig.StageTitle = FText::FromString(TEXT("涂油"));
	OilingStageConfig.ToolPanelTitle = FText::FromString(TEXT("涂油毛刷"));
	OilingStageConfig.ResultTitle = FText::FromString(TEXT("涂油完成"));
	OilingStageConfig.VisibleToolCount = 4;
	OilingStageConfig.RequiredMetricValue = CompletionThreshold;
	OilingStageConfig.InitialDustReveal01 = 1.0f;
	OilingStageConfig.InitialDustConcentration01 = 0.0f;
	OilingStageConfig.InitialOilBlend01 = 0.0f;
	OilingStageConfig.bUsesDustVisual = false;
	OilingStageConfig.bUsesOilVisual = true;

	FLevel3ToolSpec OilTool1;
	OilTool1.ToolId = TEXT("Tool_1_WideBrush");
	OilTool1.DisplayName = FText::FromString(TEXT("抛光携油团"));
	OilTool1.CleanlinessDelta = -1.0f;
	OilTool1.AestheticsDelta = 2.0f;
	OilTool1.ToolSlotIndex = 1;
	OilingStageConfig.Tools.Add(OilTool1);

	FLevel3ToolSpec OilTool2;
	OilTool2.ToolId = TEXT("Tool_2_RoundSoftBrush");
	OilTool2.DisplayName = FText::FromString(TEXT("去蜡打磨丝瓜络"));
	OilTool2.IntegrityDelta = 2.0f;
	OilTool2.AestheticsDelta = -1.0f;
	OilTool2.ToolSlotIndex = 2;
	OilingStageConfig.Tools.Add(OilTool2);

	FLevel3ToolSpec OilTool3;
	OilTool3.ToolId = TEXT("Tool_3_FineLineBrush");
	OilTool3.DisplayName = FText::FromString(TEXT("斜锋刷角修缮刷"));
	OilTool3.CleanlinessDelta = 2.0f;
	OilTool3.IntegrityDelta = -1.0f;
	OilTool3.ToolSlotIndex = 3;
	OilingStageConfig.Tools.Add(OilTool3);

	FLevel3ToolSpec OilTool4;
	OilTool4.ToolId = TEXT("Tool_4_CurvedGapBrush");
	OilTool4.DisplayName = FText::FromString(TEXT("调油宽托/油托"));
	OilTool4.IntegrityDelta = 1.0f;
	OilTool4.AestheticsDelta = 1.0f;
	OilTool4.ToolSlotIndex = 4;
	OilingStageConfig.Tools.Add(OilTool4);

	ProgressState.Phase = ELevel3Phase::Introduction;
	ProgressState.SubStage = ELevel3SubStage::None;
	ResultState = FLevel3ResultState();
}

void ULevel3FlowComponent::StartLevel3Dusting()
{
	bLevel3SessionActive = true;
	ResultState = FLevel3ResultState();
	ActiveRepairArea = nullptr;
	ClearVisualTransitionLock(false);
	ResetAllRepairAreas();
	StartStage(ELevel3SubStage::Dusting);
}

void ULevel3FlowComponent::StartLevel3Oiling()
{
	bLevel3SessionActive = true;
	ResultState = FLevel3ResultState();
	ActiveRepairArea = nullptr;
	ClearVisualTransitionLock(false);
	ResetAllRepairAreas();
	StartStage(ELevel3SubStage::Oiling);
}

void ULevel3FlowComponent::AdvanceToOilingStage()
{
	if (!bLevel3SessionActive
		|| ProgressState.Phase != ELevel3Phase::RepairResult
		|| ProgressState.SubStage != ELevel3SubStage::Dusting
		|| ProgressState.bIsVisualTransitionActive)
	{
		return;
	}

	ResultState = FLevel3ResultState();
	ActiveRepairArea = nullptr;
	ClearVisualTransitionLock(false);
	ResetAllRepairAreas();
	StartStage(ELevel3SubStage::Oiling);
}

void ULevel3FlowComponent::ResetLevel3State()
{
	bLevel3SessionActive = false;
	ActiveRepairArea = nullptr;
	ClearVisualTransitionLock(false);
	ResetAllRepairAreas();
	ProgressState = FLevel3ProgressState();
	ResultState = FLevel3ResultState();
	OnLevel3ToolSelected.Broadcast(NAME_None);
	BroadcastProgress();
}

bool ULevel3FlowComponent::SelectTool(FName ToolId)
{
	if (!bLevel3SessionActive || ToolId.IsNone() || ProgressState.Phase == ELevel3Phase::RepairResult || ProgressState.Phase == ELevel3Phase::Completed)
	{
		return false;
	}

	if (!FindToolSpec(ToolId))
	{
		return false;
	}

	ProgressState.SelectedToolId = ToolId;
	OnLevel3ToolSelected.Broadcast(ToolId);
	BroadcastProgress();

	return true;
}

bool ULevel3FlowComponent::ApplySelectedToolToHit(const FHitResult& HitResult)
{
	const FLevel3ToolSpec* ToolSpec = nullptr;
	if (!CanApplySelectedTool(ToolSpec))
	{
		return false;
	}

	ULevel3RepairAreaComponent* RepairArea = ResolveRepairAreaFromHit(HitResult);
	if (!RepairArea)
	{
		return false;
	}

	return ApplyToolSpecToRepairArea(RepairArea, *ToolSpec);
}

bool ULevel3FlowComponent::ApplySelectedToolToRepairMeshRay(FVector TraceStart, FVector TraceEnd)
{
	const FLevel3ToolSpec* ToolSpec = nullptr;
	if (!CanApplySelectedTool(ToolSpec))
	{
		return false;
	}

	FHitResult RepairMeshHitResult;
	ULevel3RepairAreaComponent* RepairArea = FindRepairAreaByRepairMeshTrace(GetWorld(), TraceStart, TraceEnd, RepairMeshHitResult);
	if (!RepairArea)
	{
		return false;
	}

	return ApplyToolSpecToRepairArea(RepairArea, *ToolSpec);
}

bool ULevel3FlowComponent::CanApplySelectedTool(const FLevel3ToolSpec*& OutToolSpec) const
{
	OutToolSpec = nullptr;

	if (!bLevel3SessionActive || ProgressState.Phase == ELevel3Phase::RepairResult || ProgressState.Phase == ELevel3Phase::Completed)
	{
		return false;
	}

	if (ProgressState.bIsVisualTransitionActive)
	{
		return false;
	}

	if (ProgressState.SelectedToolId.IsNone())
	{
		return false;
	}

	OutToolSpec = FindToolSpec(ProgressState.SelectedToolId);
	if (!OutToolSpec)
	{
		return false;
	}

	return true;
}

bool ULevel3FlowComponent::ApplyToolSpecToRepairArea(ULevel3RepairAreaComponent* RepairArea, const FLevel3ToolSpec& ToolSpec)
{
	if (!RepairArea)
	{
		return false;
	}

	ActiveRepairArea = RepairArea;
	ProgressState.ActiveAreaId = RepairArea->AreaId;

	if (ProgressState.Phase == ELevel3Phase::ToolSelecting)
	{
		SetPhase(ELevel3Phase::Repairing);
	}

	ProgressState.CleanlinessPercent = FMath::Clamp(ProgressState.CleanlinessPercent + ToolSpec.CleanlinessDelta, MinMetricValue, MaxMetricValue);
	ProgressState.IntegrityPercent = FMath::Clamp(ProgressState.IntegrityPercent + ToolSpec.IntegrityDelta, MinMetricValue, MaxMetricValue);
	ProgressState.AestheticsPercent = FMath::Clamp(ProgressState.AestheticsPercent + ToolSpec.AestheticsDelta, MinMetricValue, MaxMetricValue);
	ProgressState.bIsRepairStrokeActive = false;
	ProgressState.RepairCoveragePercent = 0.0f;

	RefreshStageVisualState();
	StartVisualTransitionLock();
	HandleRepairAreaUpdated(RepairArea);
	BroadcastProgress();
	TryCompleteRepair();
	return true;
}

bool ULevel3FlowComponent::BeginRepairStroke(const FHitResult& HitResult)
{
	return ApplySelectedToolToHit(HitResult);
}

void ULevel3FlowComponent::UpdateRepairStroke(float DeltaSeconds, const FHitResult& HitResult)
{
	(void)DeltaSeconds;
	(void)HitResult;
}

void ULevel3FlowComponent::EndRepairStroke()
{
	// Level3 当前改为“单击结算”，不再维护持续笔划状态。
}

void ULevel3FlowComponent::CompleteResultPresentation()
{
	if (!bLevel3SessionActive || ProgressState.Phase != ELevel3Phase::RepairResult || ProgressState.bIsVisualTransitionActive)
	{
		return;
	}

	if (ProgressState.SubStage == ELevel3SubStage::Dusting)
	{
		AdvanceToOilingStage();
		return;
	}

	SetPhase(ELevel3Phase::Completed);
	bLevel3SessionActive = false;
	BroadcastProgress();
}

bool ULevel3FlowComponent::SkipCurrentStageByFillingMetrics()
{
	if (!bLevel3SessionActive || ProgressState.Phase == ELevel3Phase::Completed)
	{
		return false;
	}

	ClearVisualTransitionLock(false);

	if (ProgressState.Phase == ELevel3Phase::RepairResult)
	{
		CompleteResultPresentation();
		return true;
	}

	const FLevel3StageConfig* StageConfig = GetActiveStageConfig();
	if (!StageConfig)
	{
		return false;
	}

	const float RequiredMetricValue = (StageConfig->RequiredMetricValue > KINDA_SMALL_NUMBER)
		? StageConfig->RequiredMetricValue
		: CompletionThreshold;

	ProgressState.CleanlinessPercent = FMath::Max(ProgressState.CleanlinessPercent, RequiredMetricValue);
	ProgressState.IntegrityPercent = FMath::Max(ProgressState.IntegrityPercent, RequiredMetricValue);
	ProgressState.AestheticsPercent = FMath::Max(ProgressState.AestheticsPercent, RequiredMetricValue);
	ProgressState.bIsRepairStrokeActive = false;
	ProgressState.RepairCoveragePercent = 0.0f;

	RefreshStageVisualState();

	if (ProgressState.Phase != ELevel3Phase::Repairing)
	{
		SetPhase(ELevel3Phase::Repairing);
	}

	BroadcastProgress();
	TryCompleteRepair();
	return ProgressState.Phase == ELevel3Phase::RepairResult;
}

bool ULevel3FlowComponent::IsLevel3SessionActive() const
{
	return bLevel3SessionActive;
}

bool ULevel3FlowComponent::IsRepairStrokeActive() const
{
	return ProgressState.bIsRepairStrokeActive;
}

bool ULevel3FlowComponent::IsVisualTransitionActive() const
{
	return ProgressState.bIsVisualTransitionActive;
}

float ULevel3FlowComponent::GetVisualTransitionRemainingSeconds() const
{
	return QueryVisualTransitionRemainingSeconds();
}

ELevel3Phase ULevel3FlowComponent::GetCurrentPhase() const
{
	return ProgressState.Phase;
}

ELevel3SubStage ULevel3FlowComponent::GetCurrentSubStage() const
{
	return ProgressState.SubStage;
}

FLevel3ProgressState ULevel3FlowComponent::GetProgressState() const
{
	FLevel3ProgressState Snapshot = ProgressState;
	Snapshot.VisualTransitionDurationSeconds = VisualTransitionDurationSeconds;
	Snapshot.VisualTransitionRemainingSeconds = QueryVisualTransitionRemainingSeconds();
	return Snapshot;
}

FLevel3ResultState ULevel3FlowComponent::GetResultState() const
{
	return ResultState;
}

TArray<FLevel3ToolSpec> ULevel3FlowComponent::GetAvailableDustTools() const
{
	return DustingStageConfig.Tools;
}

TArray<FLevel3ToolSpec> ULevel3FlowComponent::GetAvailableCurrentStageTools() const
{
	if (const FLevel3StageConfig* StageConfig = GetActiveStageConfig())
	{
		return StageConfig->Tools;
	}

	return TArray<FLevel3ToolSpec>();
}

FLevel3StageConfig ULevel3FlowComponent::GetCurrentStageConfig() const
{
	if (const FLevel3StageConfig* StageConfig = GetActiveStageConfig())
	{
		return *StageConfig;
	}

	return FLevel3StageConfig();
}

const FLevel3StageConfig* ULevel3FlowComponent::FindStageConfig(ELevel3SubStage SubStage) const
{
	switch (SubStage)
	{
	case ELevel3SubStage::Dusting:
		return &DustingStageConfig;
	case ELevel3SubStage::Oiling:
		return &OilingStageConfig;
	default:
		return nullptr;
	}
}

const FLevel3StageConfig* ULevel3FlowComponent::GetActiveStageConfig() const
{
	return FindStageConfig(ProgressState.SubStage);
}

const FLevel3ToolSpec* ULevel3FlowComponent::FindToolSpec(FName ToolId) const
{
	const FLevel3StageConfig* StageConfig = GetActiveStageConfig();
	if (!StageConfig)
	{
		return nullptr;
	}

	return StageConfig->Tools.FindByPredicate([ToolId](const FLevel3ToolSpec& ToolSpec)
	{
		return ToolSpec.ToolId == ToolId;
	});
}

void ULevel3FlowComponent::StartStage(ELevel3SubStage SubStage)
{
	const FLevel3StageConfig* StageConfig = FindStageConfig(SubStage);
	if (!StageConfig)
	{
		return;
	}

	ResetProgressForStage(*StageConfig);
	SetSubStage(StageConfig->SubStage);
	SetPhase(ELevel3Phase::ToolSelecting);
	OnLevel3ToolSelected.Broadcast(NAME_None);
	BroadcastProgress();
}

void ULevel3FlowComponent::ResetProgressForStage(const FLevel3StageConfig& StageConfig)
{
	ProgressState = FLevel3ProgressState();
	ProgressState.Phase = ELevel3Phase::Introduction;
	ProgressState.SubStage = ELevel3SubStage::None;
	ProgressState.SelectedToolId = NAME_None;
	ProgressState.ActiveAreaId = NAME_None;
	ProgressState.CleanlinessPercent = 0.0f;
	ProgressState.IntegrityPercent = 0.0f;
	ProgressState.AestheticsPercent = 0.0f;
	ProgressState.RepairCoveragePercent = 0.0f;
	ProgressState.bIsRepairStrokeActive = false;
	ProgressState.bIsVisualTransitionActive = false;
	ProgressState.VisualTransitionDurationSeconds = VisualTransitionDurationSeconds;
	ProgressState.VisualTransitionRemainingSeconds = 0.0f;
	ProgressState.DustReveal01 = StageConfig.InitialDustReveal01;
	ProgressState.DustConcentration01 = StageConfig.InitialDustConcentration01;
	ProgressState.OilBlend01 = StageConfig.InitialOilBlend01;
	ProgressState.StageTitle = StageConfig.StageTitle;
	ProgressState.ToolPanelTitle = StageConfig.ToolPanelTitle;
	ProgressState.ResultTitle = StageConfig.ResultTitle;
	ProgressState.VisibleToolCount = StageConfig.VisibleToolCount;
}

ULevel3RepairAreaComponent* ULevel3FlowComponent::ResolveRepairAreaFromHit(const FHitResult& HitResult) const
{
	AActor* HitActor = HitResult.GetActor();
	if (!HitActor)
	{
		return nullptr;
	}

	ULevel3RepairAreaComponent* RepairArea = HitActor->FindComponentByClass<ULevel3RepairAreaComponent>();
	if (!RepairArea)
	{
		if (ULevel3RepairAreaComponent* RelatedRepairArea = FindRepairAreaOnActorHierarchy(HitActor))
		{
			if (RelatedRepairArea->bAreaEnabled)
			{
				return RelatedRepairArea;
			}
		}

		const FVector TraceStart(HitResult.TraceStart);
		const FVector TraceEnd(HitResult.TraceEnd);
		FHitResult RepairMeshHitResult;
		return FindRepairAreaByRepairMeshTrace(GetWorld(), TraceStart, TraceEnd, RepairMeshHitResult);
	}

	if (!RepairArea->IsHitOnArea(HitResult))
	{
		return nullptr;
	}

	return RepairArea;
}

void ULevel3FlowComponent::RefreshStageVisualState()
{
	switch (ProgressState.SubStage)
	{
	case ELevel3SubStage::Dusting:
		RefreshDustState();
		break;
	case ELevel3SubStage::Oiling:
		RefreshOilState();
		break;
	default:
		break;
	}
}

void ULevel3FlowComponent::RefreshDustState()
{
	const FLevel3StageConfig* StageConfig = GetActiveStageConfig();
	const float GoalValue = (StageConfig && StageConfig->RequiredMetricValue > KINDA_SMALL_NUMBER)
		? StageConfig->RequiredMetricValue
		: CompletionThreshold;

	const float CleanlinessReady01 = FMath::Clamp(ProgressState.CleanlinessPercent / GoalValue, 0.0f, 1.0f);
	const float IntegrityReady01 = FMath::Clamp(ProgressState.IntegrityPercent / GoalValue, 0.0f, 1.0f);
	const float AestheticsReady01 = FMath::Clamp(ProgressState.AestheticsPercent / GoalValue, 0.0f, 1.0f);

	ProgressState.DustReveal01 = (CleanlinessReady01 + IntegrityReady01 + AestheticsReady01) / 3.0f;
	ProgressState.DustConcentration01 = 1.0f - ProgressState.DustReveal01;
}

void ULevel3FlowComponent::RefreshOilState()
{
	const FLevel3StageConfig* StageConfig = GetActiveStageConfig();
	const float GoalValue = (StageConfig && StageConfig->RequiredMetricValue > KINDA_SMALL_NUMBER)
		? StageConfig->RequiredMetricValue
		: CompletionThreshold;

	const float CleanlinessReady01 = FMath::Clamp(ProgressState.CleanlinessPercent / GoalValue, 0.0f, 1.0f);
	const float IntegrityReady01 = FMath::Clamp(ProgressState.IntegrityPercent / GoalValue, 0.0f, 1.0f);
	const float AestheticsReady01 = FMath::Clamp(ProgressState.AestheticsPercent / GoalValue, 0.0f, 1.0f);

	ProgressState.OilBlend01 = (CleanlinessReady01 + IntegrityReady01 + AestheticsReady01) / 3.0f;
}

float ULevel3FlowComponent::QueryVisualTransitionRemainingSeconds() const
{
	if (!ProgressState.bIsVisualTransitionActive)
	{
		return 0.0f;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return FMath::Max(0.0f, ProgressState.VisualTransitionRemainingSeconds);
	}

	const float TimerRemaining = World->GetTimerManager().GetTimerRemaining(VisualTransitionTimerHandle);
	return TimerRemaining > 0.0f ? TimerRemaining : 0.0f;
}

void ULevel3FlowComponent::StartVisualTransitionLock()
{
	const float LockDuration = FMath::Max(0.0f, VisualTransitionDurationSeconds);
	if (LockDuration <= KINDA_SMALL_NUMBER)
	{
		ClearVisualTransitionLock(false);
		return;
	}

	ProgressState.bIsVisualTransitionActive = true;
	ProgressState.VisualTransitionDurationSeconds = LockDuration;
	ProgressState.VisualTransitionRemainingSeconds = LockDuration;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(VisualTransitionTimerHandle);
		World->GetTimerManager().SetTimer(
			VisualTransitionTimerHandle,
			this,
			&ULevel3FlowComponent::FinishVisualTransitionLock,
			LockDuration,
			false);
	}
}

void ULevel3FlowComponent::ClearVisualTransitionLock(bool bBroadcastProgress)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(VisualTransitionTimerHandle);
	}

	const bool bWasActive = ProgressState.bIsVisualTransitionActive
		|| ProgressState.VisualTransitionRemainingSeconds > 0.0f;

	ProgressState.bIsVisualTransitionActive = false;
	ProgressState.VisualTransitionDurationSeconds = VisualTransitionDurationSeconds;
	ProgressState.VisualTransitionRemainingSeconds = 0.0f;

	if (bBroadcastProgress && bWasActive)
	{
		BroadcastProgress();
	}
}

void ULevel3FlowComponent::FinishVisualTransitionLock()
{
	ClearVisualTransitionLock(true);
}

void ULevel3FlowComponent::SetPhase(ELevel3Phase NewPhase)
{
	if (ProgressState.Phase == NewPhase)
	{
		return;
	}

	ProgressState.Phase = NewPhase;
	OnLevel3PhaseChanged.Broadcast(NewPhase);
}

void ULevel3FlowComponent::SetSubStage(ELevel3SubStage NewSubStage)
{
	if (ProgressState.SubStage == NewSubStage)
	{
		return;
	}

	ProgressState.SubStage = NewSubStage;
	OnLevel3SubStageChanged.Broadcast(NewSubStage);
}

void ULevel3FlowComponent::BroadcastProgress()
{
	ProgressState.VisualTransitionDurationSeconds = VisualTransitionDurationSeconds;
	ProgressState.VisualTransitionRemainingSeconds = QueryVisualTransitionRemainingSeconds();
	OnLevel3ProgressChanged.Broadcast(ProgressState);
}

void ULevel3FlowComponent::HandleRepairAreaUpdated(ULevel3RepairAreaComponent* RepairArea)
{
	if (!RepairArea)
	{
		return;
	}

	OnLevel3RepairAreaUpdated.Broadcast(RepairArea->GetAreaState());
}

void ULevel3FlowComponent::ResetAllRepairAreas()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<AActor> It(World); It; ++It)
	{
		TArray<ULevel3RepairAreaComponent*> RepairAreas;
		It->GetComponents<ULevel3RepairAreaComponent>(RepairAreas);
		for (ULevel3RepairAreaComponent* RepairArea : RepairAreas)
		{
			if (RepairArea)
			{
				RepairArea->ResetArea();
			}
		}
	}
}

void ULevel3FlowComponent::TryCompleteRepair()
{
	if (ProgressState.Phase != ELevel3Phase::Repairing)
	{
		return;
	}

	const FLevel3StageConfig* StageConfig = GetActiveStageConfig();
	if (!StageConfig)
	{
		return;
	}

	const float RequiredMetricValue = (StageConfig->RequiredMetricValue > KINDA_SMALL_NUMBER)
		? StageConfig->RequiredMetricValue
		: CompletionThreshold;

	const bool bCleanEnough = ProgressState.CleanlinessPercent >= RequiredMetricValue;
	const bool bIntegritySafe = ProgressState.IntegrityPercent >= RequiredMetricValue;
	const bool bLooksAcceptable = ProgressState.AestheticsPercent >= RequiredMetricValue;

	if (!bCleanEnough || !bIntegritySafe || !bLooksAcceptable)
	{
		return;
	}

	ResultState = FLevel3ResultState();
	ResultState.SubStage = StageConfig->SubStage;
	ResultState.bSucceeded = true;
	ResultState.SelectedToolId = ProgressState.SelectedToolId;
	ResultState.FinalCleanlinessPercent = ProgressState.CleanlinessPercent;
	ResultState.FinalIntegrityPercent = ProgressState.IntegrityPercent;
	ResultState.FinalAestheticsPercent = ProgressState.AestheticsPercent;
	ResultState.FinalCoveragePercent = 0.0f;
	ResultState.FinalDustReveal01 = ProgressState.DustReveal01;
	ResultState.FinalDustConcentration01 = ProgressState.DustConcentration01;
	ResultState.FinalOilBlend01 = ProgressState.OilBlend01;
	ResultState.ResultTitle = StageConfig->ResultTitle;
	ResultState.CulturalContributionValue = FMath::RoundToInt(
		(ResultState.FinalCleanlinessPercent + ResultState.FinalIntegrityPercent + ResultState.FinalAestheticsPercent) / 3.0f);

	ProgressState.bIsRepairStrokeActive = false;
	SetPhase(ELevel3Phase::RepairResult);
	BroadcastProgress();
	OnLevel3ResultReady.Broadcast(ResultState);
}
