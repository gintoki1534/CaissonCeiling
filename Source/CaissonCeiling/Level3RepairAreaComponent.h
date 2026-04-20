#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Level3Types.h"
#include "Level3RepairAreaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevel3RepairAreaCoverageChanged, const FLevel3RepairAreaState&, AreaState);

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAISSONCEILING_API ULevel3RepairAreaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULevel3RepairAreaComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FName AreaId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	bool bAreaEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float CoverageRateMultiplier;

	UPROPERTY(BlueprintAssignable, Category="Level3")
	FOnLevel3RepairAreaCoverageChanged OnCoverageChanged;

	UFUNCTION(BlueprintCallable, Category="Level3")
	void ResetArea();

	UFUNCTION(BlueprintCallable, Category="Level3")
	bool ApplyBrushSample(const FHitResult& HitResult, float CoveragePerSecond, float DeltaSeconds);

	UFUNCTION(BlueprintPure, Category="Level3")
	float GetCoveragePercent() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	FLevel3RepairAreaState GetAreaState() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	bool IsHitOnArea(const FHitResult& HitResult) const;

private:
	UPROPERTY(Transient)
	float CoveragePercent;
};
