#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerManager.h"
#include "Level3Types.h"
#include "Level3FlowComponent.generated.h"

class ULevel3RepairAreaComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevel3PhaseChanged, ELevel3Phase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevel3SubStageChanged, ELevel3SubStage, NewSubStage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevel3ToolSelected, FName, ToolId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevel3ProgressChanged, const FLevel3ProgressState&, ProgressState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevel3RepairAreaUpdated, const FLevel3RepairAreaState&, AreaState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevel3ResultReady, const FLevel3ResultState&, ResultState);

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAISSONCEILING_API ULevel3FlowComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULevel3FlowComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|Stages")
	FLevel3StageConfig DustingStageConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|Stages")
	FLevel3StageConfig OilingStageConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|Rules")
	float CompletionThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|Rules")
	float MinMetricValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|Rules")
	float MaxMetricValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3|Rules")
	float VisualTransitionDurationSeconds;

	UPROPERTY(BlueprintAssignable, Category="Level3")
	FOnLevel3PhaseChanged OnLevel3PhaseChanged;

	UPROPERTY(BlueprintAssignable, Category="Level3")
	FOnLevel3SubStageChanged OnLevel3SubStageChanged;

	UPROPERTY(BlueprintAssignable, Category="Level3")
	FOnLevel3ToolSelected OnLevel3ToolSelected;

	UPROPERTY(BlueprintAssignable, Category="Level3")
	FOnLevel3ProgressChanged OnLevel3ProgressChanged;

	UPROPERTY(BlueprintAssignable, Category="Level3")
	FOnLevel3RepairAreaUpdated OnLevel3RepairAreaUpdated;

	UPROPERTY(BlueprintAssignable, Category="Level3")
	FOnLevel3ResultReady OnLevel3ResultReady;

	UFUNCTION(BlueprintCallable, Category="Level3")
	void StartLevel3Dusting();

	UFUNCTION(BlueprintCallable, Category="Level3")
	void StartLevel3Oiling();

	UFUNCTION(BlueprintCallable, Category="Level3")
	void AdvanceToOilingStage();

	UFUNCTION(BlueprintCallable, Category="Level3")
	void ResetLevel3State();

	UFUNCTION(BlueprintCallable, Category="Level3")
	bool SelectTool(FName ToolId);

	UFUNCTION(BlueprintCallable, Category="Level3")
	bool ApplySelectedToolToHit(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category="Level3")
	bool BeginRepairStroke(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category="Level3")
	void UpdateRepairStroke(float DeltaSeconds, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category="Level3")
	void EndRepairStroke();

	UFUNCTION(BlueprintCallable, Category="Level3")
	void CompleteResultPresentation();

	UFUNCTION(BlueprintCallable, Category="Level3|Debug")
	bool SkipCurrentStageByFillingMetrics();

	UFUNCTION(BlueprintPure, Category="Level3")
	bool IsLevel3SessionActive() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	bool IsRepairStrokeActive() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	bool IsVisualTransitionActive() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	float GetVisualTransitionRemainingSeconds() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	ELevel3Phase GetCurrentPhase() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	ELevel3SubStage GetCurrentSubStage() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	FLevel3ProgressState GetProgressState() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	FLevel3ResultState GetResultState() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	TArray<FLevel3ToolSpec> GetAvailableDustTools() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	TArray<FLevel3ToolSpec> GetAvailableCurrentStageTools() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	FLevel3StageConfig GetCurrentStageConfig() const;

private:
	UPROPERTY(Transient)
	bool bLevel3SessionActive;

	UPROPERTY(Transient)
	FLevel3ProgressState ProgressState;

	UPROPERTY(Transient)
	FLevel3ResultState ResultState;

	UPROPERTY(Transient)
	TObjectPtr<ULevel3RepairAreaComponent> ActiveRepairArea;

	FTimerHandle VisualTransitionTimerHandle;

	const FLevel3StageConfig* FindStageConfig(ELevel3SubStage SubStage) const;
	const FLevel3StageConfig* GetActiveStageConfig() const;
	const FLevel3ToolSpec* FindToolSpec(FName ToolId) const;
	void StartStage(ELevel3SubStage SubStage);
	void ResetProgressForStage(const FLevel3StageConfig& StageConfig);
	ULevel3RepairAreaComponent* ResolveRepairAreaFromHit(const FHitResult& HitResult) const;
	void RefreshStageVisualState();
	void RefreshDustState();
	void RefreshOilState();
	float QueryVisualTransitionRemainingSeconds() const;
	void StartVisualTransitionLock();
	void ClearVisualTransitionLock(bool bBroadcastProgress);
	void FinishVisualTransitionLock();
	void SetPhase(ELevel3Phase NewPhase);
	void SetSubStage(ELevel3SubStage NewSubStage);
	void BroadcastProgress();
	void HandleRepairAreaUpdated(ULevel3RepairAreaComponent* RepairArea);
	void ResetAllRepairAreas();
	void TryCompleteRepair();
};
