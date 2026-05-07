#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level3RepairRegionActor.generated.h"

class USceneComponent;
class UPointLightComponent;
class UPrimitiveComponent;
class UStaticMeshComponent;
class ULevel3RepairAreaComponent;

UCLASS(BlueprintType, Blueprintable)
class CAISSONCEILING_API ALevel3RepairRegionActor : public AActor
{
	GENERATED_BODY()

public:
	ALevel3RepairRegionActor();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level3")
	TObjectPtr<USceneComponent> SceneRootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level3")
	TObjectPtr<UStaticMeshComponent> PreviewMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level3")
	TObjectPtr<ULevel3RepairAreaComponent> RepairAreaComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|TransitionPulse")
	bool bEnableTransitionPulseLights;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|TransitionPulse", meta=(ClampMin="1", ClampMax="6"))
	int32 TransitionPulseLightCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|TransitionPulse", meta=(ClampMin="0.1"))
	float TransitionPulseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|TransitionPulse", meta=(ClampMin="0.0"))
	float TransitionPulseFadeSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|TransitionPulse", meta=(ClampMin="0.0", ClampMax="2.0"))
	float TransitionPulseIntensityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|TransitionPulse", meta=(ClampMin="0.0"))
	float TransitionPulseMaxIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|TransitionPulse", meta=(ClampMin="0.1"))
	float TransitionPulseMinBrightnessRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|TransitionPulse", meta=(ClampMin="0.1"))
	float TransitionPulseRangeScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|TransitionPulse", meta=(ClampMin="0.0"))
	float TransitionPulseBoundsPadding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|TransitionPulse", meta=(ClampMin="1", ClampMax="2"))
	int32 TransitionPulseLightingChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|TransitionPulse")
	FLinearColor TransitionPulseColor;

	UFUNCTION(BlueprintCallable, Category="Level3|TransitionPulse")
	void SetTransitionPulseActive(bool bActive);

	UFUNCTION(BlueprintPure, Category="Level3|TransitionPulse")
	bool IsTransitionPulseActive() const;

private:
	struct FPrimitiveLightingChannelState
	{
		TWeakObjectPtr<UPrimitiveComponent> PrimitiveComponent;
		bool bChannel0 = true;
		bool bChannel1 = false;
		bool bChannel2 = false;
	};

	UPROPERTY(Transient)
	TArray<TObjectPtr<UPointLightComponent>> TransitionPulseLightComponents;

	TArray<FPrimitiveLightingChannelState> OverriddenLightingChannelStates;

	bool bTransitionPulseActive;
	float TransitionPulseTime;
	float TransitionPulseAlpha;

	void ConfigureRepairMeshCollision();
	void EnsureTransitionPulseLights();
	void RepositionTransitionPulseLights();
	void UpdateTransitionPulseLights();
	void UpdateTransitionPulseLightingChannels();
	void RestoreTransitionPulseLightingChannels();
	void GetTransitionPulseLightingChannelMask(bool& bOutChannel0, bool& bOutChannel1, bool& bOutChannel2) const;
	void CollectTransitionPulsePrimitiveComponents(TArray<UPrimitiveComponent*>& OutComponents) const;
	void UpdateTransitionPulseFade(float DeltaSeconds);
};
