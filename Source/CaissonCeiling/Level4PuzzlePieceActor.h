#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level4PuzzlePieceActor.generated.h"

class UStaticMeshComponent;
class UMeshComponent;
class UPointLightComponent;
class UPrimitiveComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS()
class CAISSONCEILING_API ALevel4PuzzlePieceActor : public AActor
{
	GENERATED_BODY()

public:
	ALevel4PuzzlePieceActor();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level4")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level4")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(BlueprintReadOnly, Category="Level4")
	int32 PieceIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly, Category="Level4")
	bool bIsSelected = false;

	UPROPERTY(BlueprintReadOnly, Category="Level4")
	bool bIsCorrect = false;

	UPROPERTY(BlueprintReadOnly, Category="Level4")
	bool bIsSpawnedInField = false;

	UFUNCTION(BlueprintCallable, Category="Level4")
	void InitializeLevel4Piece(int32 InPieceIndex);

	UFUNCTION(BlueprintCallable, Category="Level4")
	void SetSelectedVisual(bool bNewSelected);

	UFUNCTION(BlueprintCallable, Category="Level4")
	void SetCorrectVisual(bool bNewCorrect);

	UFUNCTION(BlueprintCallable, Category="Level4")
	UPrimitiveComponent* GetDragPrimitive() const;

	UFUNCTION(BlueprintImplementableEvent, Category="Level4")
	void BP_OnSelectedVisualChanged(bool bNewSelected);

	UFUNCTION(BlueprintImplementableEvent, Category="Level4")
	void BP_OnCorrectVisualChanged(bool bNewCorrect);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual")
	bool bUseCustomDepthForSelection = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual")
	int32 SelectedCustomDepthStencil = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual")
	int32 CorrectCustomDepthStencil = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse")
	bool bEnablePulseLights = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse")
	bool bEnableOverlayPulse = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse")
	TObjectPtr<UMaterialInterface> PulseOverlayMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse", meta=(ClampMin="1", ClampMax="6"))
	int32 PulseLightCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse", meta=(ClampMin="0.1"))
	float PulseSpeed = 2.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse", meta=(ClampMin="0.0", ClampMax="2.0"))
	float PulseIntensityScale = 0.0005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse", meta=(ClampMin="0.0"))
	float SelectedPulseMaxIntensity = 4200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse", meta=(ClampMin="0.0"))
	float CorrectPulseMaxIntensity = 1600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse", meta=(ClampMin="0.1"))
	float PulseMinBrightnessRatio = 0.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse", meta=(ClampMin="0.1"))
	float PulseRangeScale = 2.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse", meta=(ClampMin="0.0"))
	float PulseBoundsPadding = 36.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse", meta=(ClampMin="1", ClampMax="2"))
	int32 PulseLightingChannel = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse")
	FLinearColor SelectedPulseColor = FLinearColor(1.0f, 0.72f, 0.22f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Visual|Pulse")
	FLinearColor CorrectPulseColor = FLinearColor(1.0f, 0.82f, 0.42f, 1.0f);

private:
	struct FLevel4PulseOverlaySlot
	{
		TWeakObjectPtr<UMeshComponent> MeshComponent;
		TWeakObjectPtr<UMaterialInterface> OriginalOverlayMaterial;
		TWeakObjectPtr<UMaterialInstanceDynamic> DynamicOverlayMaterial;
	};

	struct FPrimitiveLightingChannelState
	{
		TWeakObjectPtr<UPrimitiveComponent> PrimitiveComponent;
		bool bChannel0 = true;
		bool bChannel1 = false;
		bool bChannel2 = false;
	};

	UPROPERTY(Transient)
	TArray<TObjectPtr<UPointLightComponent>> PulseLightComponents;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UMaterialInstanceDynamic>> PulseOverlayMaterialInstances;

	TArray<FLevel4PulseOverlaySlot> PulseOverlaySlots;
	TArray<FPrimitiveLightingChannelState> OverriddenLightingChannelStates;

	float PulseTime = 0.0f;

	void RefreshVisualState();
	void ApplyCustomDepthVisual();
	float GetPulsePhaseSeconds() const;
	void EnsurePulseOverlayMaterials();
	void UpdatePulseOverlayMaterials();
	void RestorePulseOverlayMaterials();
	void EnsurePulseLights();
	void RepositionPulseLights();
	void UpdatePulseLights();
	void UpdatePulseLightingChannels();
	void RestorePulseLightingChannels();
	void GetPulseLightingChannelMask(bool& bOutChannel0, bool& bOutChannel1, bool& bOutChannel2) const;
	void CollectVisualPrimitiveComponents(TArray<UPrimitiveComponent*>& OutComponents) const;
	void CollectVisualMeshComponents(TArray<UMeshComponent*>& OutComponents) const;
};
