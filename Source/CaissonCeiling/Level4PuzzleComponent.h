#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Level4Types.h"
#include "Level4PuzzleComponent.generated.h"

class ALevel4PuzzlePieceActor;
class ALevel4PuzzleTargetLayoutActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevel4StageChanged, ELevel4StageId, NewStage, int32, StageIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevel4DifficultyChanged, ELevel4Difficulty, NewDifficulty);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevel4PieceSelected, int32, PieceIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevel4PieceSpawned, int32, PieceIndex, ALevel4PuzzlePieceActor*, PieceActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevel4PieceCorrectChanged, int32, PieceIndex, bool, bIsCorrect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevel4StageSolved, ELevel4StageId, SolvedStage, int32, StageIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevel4Completed);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAISSONCEILING_API ULevel4PuzzleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULevel4PuzzleComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Stages")
	TArray<FLevel4PuzzleStageConfig> StageConfigs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Stages")
	TArray<TSubclassOf<ALevel4PuzzleTargetLayoutActor>> StageLayoutClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Spawn")
	FVector SpawnOrigin = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Spawn")
	bool bSpawnRelativeToOwner = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Spawn")
	float SpawnHeightOffset = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Debug")
	bool bEnableLevel4DebugLogs = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Snap")
	bool bEnableSnapOnDragRelease = true;

	UPROPERTY(BlueprintAssignable, Category="Level4|Events")
	FOnLevel4StageChanged OnLevel4StageChanged;

	UPROPERTY(BlueprintAssignable, Category="Level4|Events")
	FOnLevel4DifficultyChanged OnLevel4DifficultyChanged;

	UPROPERTY(BlueprintAssignable, Category="Level4|Events")
	FOnLevel4PieceSelected OnLevel4PieceSelected;

	UPROPERTY(BlueprintAssignable, Category="Level4|Events")
	FOnLevel4PieceSpawned OnLevel4PieceSpawned;

	UPROPERTY(BlueprintAssignable, Category="Level4|Events")
	FOnLevel4PieceCorrectChanged OnLevel4PieceCorrectChanged;

	UPROPERTY(BlueprintAssignable, Category="Level4|Events")
	FOnLevel4StageSolved OnLevel4StageSolved;

	UPROPERTY(BlueprintAssignable, Category="Level4|Events")
	FOnLevel4Completed OnLevel4Completed;

	UFUNCTION(BlueprintCallable, Category="Level4")
	void StartLevel4Puzzle(ELevel4Difficulty InDifficulty);

	UFUNCTION(BlueprintCallable, Category="Level4")
	void SetLevel4Difficulty(ELevel4Difficulty InDifficulty);

	UFUNCTION(BlueprintCallable, Category="Level4")
	void SelectPiece(int32 PieceIndex);

	UFUNCTION(BlueprintCallable, Category="Level4")
	void ContinueLevel4AfterStageSolved();

	UFUNCTION(BlueprintCallable, Category="Level4")
	void BeginDragSelectedPiece(const FHitResult& HitResult, APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category="Level4")
	void EndDragSelectedPiece();

	UFUNCTION(BlueprintCallable, Category="Level4")
	void RotateSelectedPiece90();

	UFUNCTION(BlueprintCallable, Category="Level4")
	void HandleRightClickPiece(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category="Level4")
	void MoveSelectedPieceToTargetLocationKeepingRotation();

	bool FindSpawnedPieceHitOnRay(const FVector& RayStart, const FVector& RayEnd, FHitResult& OutHitResult) const;

	UFUNCTION(BlueprintCallable, Category="Level4")
	bool IsLevel4SessionActive() const;

	UFUNCTION(BlueprintPure, Category="Level4")
	ELevel4Difficulty GetCurrentDifficulty() const;

	UFUNCTION(BlueprintPure, Category="Level4")
	ELevel4StageId GetCurrentStageId() const;

	UFUNCTION(BlueprintPure, Category="Level4")
	ELevel4PuzzlePhase GetCurrentPhase() const;

	UFUNCTION(BlueprintPure, Category="Level4")
	bool HasLevel4DifficultySelection() const;

	UFUNCTION(BlueprintPure, Category="Level4")
	int32 GetSelectedPieceIndex() const;

	UFUNCTION(BlueprintPure, Category="Level4")
	ALevel4PuzzlePieceActor* GetPieceActor(int32 PieceIndex) const;

	UFUNCTION(BlueprintPure, Category="Level4")
	bool IsPieceSpawned(int32 PieceIndex) const;

	UFUNCTION(BlueprintPure, Category="Level4")
	bool IsPieceCorrect(int32 PieceIndex) const;

private:
	struct FRuntimePiece
	{
		FLevel4PuzzlePieceConfig Config;
		ALevel4PuzzlePieceActor* Actor = nullptr;
		bool bIsCorrect = false;
	};

	void BuildConfigsFromLayouts();
	void StartStage(int32 NewStageIndex);
	void CompleteCurrentStage();
	void EvaluateCurrentStage();
	void SelectPieceInternal(int32 PieceIndex, bool bUserInitiated);
	void EnsureDifficultySelected();
	void SpawnPieceIfNeeded(int32 PieceIndex);
	void RotateSelectedPiece90AroundPivot(const FVector& PivotLocation);
	void SnapPieceToBestAnchorIfClose(ALevel4PuzzlePieceActor* PieceActor);
	bool CanRotateStage(const FLevel4PuzzleStageConfig& StageConfig) const;
	bool ShouldSkipStage(ELevel4StageId StageId) const;
	FLevel4PuzzleStageConfig* GetCurrentStageConfig();
	const FLevel4PuzzleStageConfig* GetCurrentStageConfig() const;
	FRuntimePiece* FindRuntimePiece(int32 PieceIndex);
	const FRuntimePiece* FindRuntimePiece(int32 PieceIndex) const;
	ALevel4PuzzlePieceActor* ResolveHitPieceActor(const FHitResult& HitResult) const;
	bool IsRuntimePieceActor(const ALevel4PuzzlePieceActor* PieceActor) const;
	void LogHitDebug(const TCHAR* Context, const FHitResult& HitResult, const ALevel4PuzzlePieceActor* ResolvedPiece) const;
	FTransform GetWorldTargetTransform(const FLevel4PuzzlePieceConfig& PieceConfig) const;
	bool IsPieceAlignedToAnchor(const FRuntimePiece& RuntimePiece, const FRuntimePiece& AnchorPiece, const FLevel4PuzzleStageConfig& StageConfig, float& OutPositionError, float& OutRotationError) const;
	FVector GetPlaneNormal(const FLevel4PuzzleStageConfig& StageConfig) const;
	FVector ProjectPointToStagePlane(const FVector& Point, const FLevel4PuzzleStageConfig& StageConfig, const FVector& PlaneOrigin) const;
	bool GetCursorIntersectionOnStagePlane(APlayerController* PlayerController, const FLevel4PuzzleStageConfig& StageConfig, const FVector& PlaneOrigin, FVector& OutIntersection, const TCHAR* DebugContext = TEXT("")) const;
	FTransform MakeSpawnTransform(const FLevel4PuzzlePieceConfig& PieceConfig, const FLevel4PuzzleStageConfig& StageConfig) const;
	float GetRotationErrorDegrees(const FRotator& A, const FRotator& B) const;

	UPROPERTY(Transient)
	TArray<FLevel4PuzzleStageConfig> RuntimeStageConfigs;

	TArray<FRuntimePiece> RuntimePieces;

	UPROPERTY(Transient)
	TObjectPtr<APlayerController> DragPlayerController;

	UPROPERTY(Transient)
	TObjectPtr<ALevel4PuzzlePieceActor> DraggedPiece;

	ELevel4Difficulty CurrentDifficulty = ELevel4Difficulty::Normal;
	ELevel4PuzzlePhase CurrentPhase = ELevel4PuzzlePhase::Inactive;
	int32 CurrentStageIndex = INDEX_NONE;
	int32 SelectedPieceIndex = INDEX_NONE;
	int32 RightClickPrimedPieceIndex = INDEX_NONE;
	bool bSessionActive = false;
	bool bDifficultySelected = false;
	bool bDragging = false;
	FVector DragOffset = FVector::ZeroVector;
	FVector DragPlaneOrigin = FVector::ZeroVector;
	FVector DragActorPlaneOrigin = FVector::ZeroVector;
};
