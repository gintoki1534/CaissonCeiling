#include "Level3RepairAreaComponent.h"

#include "Components/PrimitiveComponent.h"

ULevel3RepairAreaComponent::ULevel3RepairAreaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	AreaId = TEXT("DustingArea");
	bAreaEnabled = true;
	CoverageRateMultiplier = 1.0f;
	CoveragePercent = 0.0f;
}

void ULevel3RepairAreaComponent::ResetArea()
{
	CoveragePercent = 0.0f;
	OnCoverageChanged.Broadcast(GetAreaState());
}

bool ULevel3RepairAreaComponent::ApplyBrushSample(const FHitResult& HitResult, float CoveragePerSecond, float DeltaSeconds)
{
	if (!bAreaEnabled || DeltaSeconds <= 0.0f || CoveragePerSecond <= 0.0f || !IsHitOnArea(HitResult))
	{
		return false;
	}

	const float PreviousCoverage = CoveragePercent;
	const float CoverageDelta = CoveragePerSecond * CoverageRateMultiplier * DeltaSeconds;
	CoveragePercent = FMath::Clamp(CoveragePercent + CoverageDelta, 0.0f, 100.0f);

	if (!FMath::IsNearlyEqual(PreviousCoverage, CoveragePercent))
	{
		OnCoverageChanged.Broadcast(GetAreaState());
	}

	return CoveragePercent > PreviousCoverage;
}

float ULevel3RepairAreaComponent::GetCoveragePercent() const
{
	return CoveragePercent;
}

FLevel3RepairAreaState ULevel3RepairAreaComponent::GetAreaState() const
{
	FLevel3RepairAreaState AreaState;
	AreaState.AreaId = AreaId;
	AreaState.CoveragePercent = CoveragePercent;
	return AreaState;
}

bool ULevel3RepairAreaComponent::IsHitOnArea(const FHitResult& HitResult) const
{
	const AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return false;
	}

	if (HitResult.GetActor() == OwnerActor)
	{
		return true;
	}

	if (const UPrimitiveComponent* HitComponent = HitResult.GetComponent())
	{
		return HitComponent->GetOwner() == OwnerActor;
	}

	return false;
}
