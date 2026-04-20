#include "Level3FlowComponent.h"

#include "EngineUtils.h"
#include "Level3RepairAreaComponent.h"

ULevel3FlowComponent::ULevel3FlowComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bLevel3SessionActive = false;
	CompletionThreshold = 1.0f;
	MaxMetricValue = 2.0f;

	FLevel3ToolSpec Tool1WideBrush;
	Tool1WideBrush.ToolId = TEXT("Tool_1_WideBrush");
	Tool1WideBrush.DisplayName = FText::FromString(TEXT("WideBrush"));
	Tool1WideBrush.CleanlinessDelta = 1.0f;
	Tool1WideBrush.IntegrityDelta = 0.0f;
	Tool1WideBrush.AestheticsDelta = -1.0f;
	AvailableDustTools.Add(Tool1WideBrush);

	FLevel3ToolSpec Tool2RoundSoftBrush;
	Tool2RoundSoftBrush.ToolId = TEXT("Tool_2_RoundSoftBrush");
	Tool2RoundSoftBrush.DisplayName = FText::FromString(TEXT("RoundSoftBrush"));
	Tool2RoundSoftBrush.CleanlinessDelta = 1.0f;
	Tool2RoundSoftBrush.IntegrityDelta = -1.0f;
	Tool2RoundSoftBrush.AestheticsDelta = 1.0f;
	AvailableDustTools.Add(Tool2RoundSoftBrush);

	FLevel3ToolSpec Tool3FineLineBrush;
	Tool3FineLineBrush.ToolId = TEXT("Tool_3_FineLineBrush");
	Tool3FineLineBrush.DisplayName = FText::FromString(TEXT("FineLineBrush"));
	Tool3FineLineBrush.CleanlinessDelta = 0.0f;
	Tool3FineLineBrush.IntegrityDelta = 1.0f;
	Tool3FineLineBrush.AestheticsDelta = 1.0f;
	AvailableDustTools.Add(Tool3FineLineBrush);

	FLevel3ToolSpec Tool4CurvedGapBrush;
	Tool4CurvedGapBrush.ToolId = TEXT("Tool_4_CurvedGapBrush");
	Tool4CurvedGapBrush.DisplayName = FText::FromString(TEXT("CurvedGapBrush"));
	Tool4CurvedGapBrush.CleanlinessDelta = -1.0f;
	Tool4CurvedGapBrush.IntegrityDelta = 1.0f;
	Tool4CurvedGapBrush.AestheticsDelta = 0.0f;
	AvailableDustTools.Add(Tool4CurvedGapBrush);

	FLevel3ToolSpec Tool5FinishingBrush;
	Tool5FinishingBrush.ToolId = TEXT("Tool_5_FinishingBrush");
	Tool5FinishingBrush.DisplayName = FText::FromString(TEXT("FinishingBrush"));
	Tool5FinishingBrush.CleanlinessDelta = 1.0f;
	Tool5FinishingBrush.IntegrityDelta = 1.0f;
	Tool5FinishingBrush.AestheticsDelta = -1.0f;
	AvailableDustTools.Add(Tool5FinishingBrush);

	ProgressState.Phase = ELevel3Phase::Introduction;
	ResultState = FLevel3ResultState();
}

void ULevel3FlowComponent::StartLevel3Dusting()
{
	bLevel3SessionActive = true;
	ResultState = FLevel3ResultState();
	ActiveRepairArea = nullptr;
	ResetAllRepairAreas();

	ProgressState = FLevel3ProgressState();
	ProgressState.Phase = ELevel3Phase::Introduction;
	ProgressState.CleanlinessPercent = 0.0f;
	ProgressState.IntegrityPercent = 0.0f;
	ProgressState.AestheticsPercent = 0.0f;
	ProgressState.RepairCoveragePercent = 0.0f;
	ProgressState.SelectedToolId = NAME_None;
	ProgressState.ActiveAreaId = NAME_None;
	ProgressState.bIsRepairStrokeActive = false;
	ProgressState.DustReveal01 = 0.0f;
	ProgressState.DustConcentration01 = 1.0f;

	SetPhase(ELevel3Phase::ToolSelecting);
	BroadcastProgress();
}

void ULevel3FlowComponent::ResetLevel3State()
{
	bLevel3SessionActive = false;
	ActiveRepairArea = nullptr;
	ResetAllRepairAreas();
	ProgressState = FLevel3ProgressState();
	ProgressState.Phase = ELevel3Phase::Introduction;
	ResultState = FLevel3ResultState();

	SetPhase(ELevel3Phase::Introduction);
	BroadcastProgress();
}

bool ULevel3FlowComponent::SelectTool(FName ToolId)
{
	if (!bLevel3SessionActive || ToolId.IsNone() || !FindToolSpec(ToolId))
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
	if (!bLevel3SessionActive || ProgressState.Phase == ELevel3Phase::RepairResult || ProgressState.Phase == ELevel3Phase::Completed)
	{
		return false;
	}

	if (ProgressState.SelectedToolId.IsNone())
	{
		return false;
	}

	const FLevel3ToolSpec* ToolSpec = FindToolSpec(ProgressState.SelectedToolId);
	if (!ToolSpec)
	{
		return false;
	}

	ULevel3RepairAreaComponent* RepairArea = ResolveRepairAreaFromHit(HitResult);
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

	ProgressState.CleanlinessPercent = FMath::Clamp(ProgressState.CleanlinessPercent + ToolSpec->CleanlinessDelta, 0.0f, MaxMetricValue);
	ProgressState.IntegrityPercent = FMath::Clamp(ProgressState.IntegrityPercent + ToolSpec->IntegrityDelta, 0.0f, MaxMetricValue);
	ProgressState.AestheticsPercent = FMath::Clamp(ProgressState.AestheticsPercent + ToolSpec->AestheticsDelta, 0.0f, MaxMetricValue);
	ProgressState.bIsRepairStrokeActive = false;
	ProgressState.RepairCoveragePercent = 0.0f;

	RefreshDustState();
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
	if (!bLevel3SessionActive || ProgressState.Phase != ELevel3Phase::RepairResult)
	{
		return;
	}

	SetPhase(ELevel3Phase::Completed);
	bLevel3SessionActive = false;
	BroadcastProgress();
}

bool ULevel3FlowComponent::IsLevel3SessionActive() const
{
	return bLevel3SessionActive;
}

bool ULevel3FlowComponent::IsRepairStrokeActive() const
{
	return ProgressState.bIsRepairStrokeActive;
}

ELevel3Phase ULevel3FlowComponent::GetCurrentPhase() const
{
	return ProgressState.Phase;
}

FLevel3ProgressState ULevel3FlowComponent::GetProgressState() const
{
	return ProgressState;
}

FLevel3ResultState ULevel3FlowComponent::GetResultState() const
{
	return ResultState;
}

TArray<FLevel3ToolSpec> ULevel3FlowComponent::GetAvailableDustTools() const
{
	return AvailableDustTools;
}

const FLevel3ToolSpec* ULevel3FlowComponent::FindToolSpec(FName ToolId) const
{
	return AvailableDustTools.FindByPredicate([ToolId](const FLevel3ToolSpec& ToolSpec)
	{
		return ToolSpec.ToolId == ToolId;
	});
}

ULevel3RepairAreaComponent* ULevel3FlowComponent::ResolveRepairAreaFromHit(const FHitResult& HitResult) const
{
	AActor* HitActor = HitResult.GetActor();
	if (!HitActor)
	{
		return nullptr;
	}

	ULevel3RepairAreaComponent* RepairArea = HitActor->FindComponentByClass<ULevel3RepairAreaComponent>();
	if (!RepairArea || !RepairArea->IsHitOnArea(HitResult))
	{
		return nullptr;
	}

	return RepairArea;
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

void ULevel3FlowComponent::RefreshDustState()
{
	const float CleanlinessReady01 = FMath::Clamp(ProgressState.CleanlinessPercent / CompletionThreshold, 0.0f, 1.0f);
	const float IntegrityReady01 = FMath::Clamp(ProgressState.IntegrityPercent / CompletionThreshold, 0.0f, 1.0f);
	const float AestheticsReady01 = FMath::Clamp(ProgressState.AestheticsPercent / CompletionThreshold, 0.0f, 1.0f);

	ProgressState.DustReveal01 = (CleanlinessReady01 + IntegrityReady01 + AestheticsReady01) / 3.0f;
	ProgressState.DustConcentration01 = 1.0f - ProgressState.DustReveal01;
}

void ULevel3FlowComponent::BroadcastProgress()
{
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

	const bool bCleanEnough = ProgressState.CleanlinessPercent > CompletionThreshold;
	const bool bIntegritySafe = ProgressState.IntegrityPercent > CompletionThreshold;
	const bool bLooksAcceptable = ProgressState.AestheticsPercent > CompletionThreshold;

	if (!bCleanEnough || !bIntegritySafe || !bLooksAcceptable)
	{
		return;
	}

	ResultState.bSucceeded = true;
	ResultState.SelectedToolId = ProgressState.SelectedToolId;
	ResultState.FinalCleanlinessPercent = ProgressState.CleanlinessPercent;
	ResultState.FinalIntegrityPercent = ProgressState.IntegrityPercent;
	ResultState.FinalAestheticsPercent = ProgressState.AestheticsPercent;
	ResultState.FinalCoveragePercent = 0.0f;
	ResultState.FinalDustReveal01 = ProgressState.DustReveal01;
	ResultState.FinalDustConcentration01 = ProgressState.DustConcentration01;
	ResultState.CulturalContributionValue = FMath::RoundToInt(
		(ResultState.FinalCleanlinessPercent + ResultState.FinalIntegrityPercent + ResultState.FinalAestheticsPercent) / 3.0f);

	ProgressState.bIsRepairStrokeActive = false;
	SetPhase(ELevel3Phase::RepairResult);
	BroadcastProgress();
	OnLevel3ResultReady.Broadcast(ResultState);
}
