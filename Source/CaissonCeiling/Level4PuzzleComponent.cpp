#include "Level4PuzzleComponent.h"

#include "Level4PuzzlePieceActor.h"
#include "Level4PuzzleTargetLayoutActor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

ULevel4PuzzleComponent::ULevel4PuzzleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULevel4PuzzleComponent::BeginPlay()
{
	Super::BeginPlay();
	BuildConfigsFromLayouts();
}

void ULevel4PuzzleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bDragging || !DraggedPiece || !DragPlayerController)
	{
		return;
	}

	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	const FRuntimePiece* RuntimePiece = FindRuntimePiece(DraggedPiece->PieceIndex);
	if (!StageConfig || !RuntimePiece)
	{
		return;
	}

	FVector WorldOrigin = RuntimePiece->Config.TargetTransform.GetLocation();
	if (bSpawnRelativeToOwner && GetOwner())
	{
		WorldOrigin = GetOwner()->GetActorTransform().TransformPosition(WorldOrigin);
	}

	FVector WorldDirection;
	FVector WorldLocation;
	if (!DragPlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		return;
	}

	const FVector PlaneNormal = GetPlaneNormal(*StageConfig);
	const FPlane DragPlane(WorldOrigin, PlaneNormal);
	const FVector RayEnd = WorldLocation + WorldDirection * 100000.0f;
	const FVector Intersection = FMath::LinePlaneIntersection(WorldLocation, RayEnd, DragPlane);
	DraggedPiece->SetActorLocation(Intersection + DragOffset);
}

void ULevel4PuzzleComponent::StartLevel4Puzzle(ELevel4Difficulty InDifficulty)
{
	BuildConfigsFromLayouts();
	CurrentDifficulty = InDifficulty;
	bSessionActive = true;
	OnLevel4DifficultyChanged.Broadcast(CurrentDifficulty);

	StartStage(0);
}

void ULevel4PuzzleComponent::SetLevel4Difficulty(ELevel4Difficulty InDifficulty)
{
	if (CurrentDifficulty == InDifficulty)
	{
		return;
	}

	CurrentDifficulty = InDifficulty;
	OnLevel4DifficultyChanged.Broadcast(CurrentDifficulty);
}

void ULevel4PuzzleComponent::SelectPiece(int32 PieceIndex)
{
	if (!bSessionActive)
	{
		return;
	}

	SpawnPieceIfNeeded(PieceIndex);

	if (SelectedPieceIndex == PieceIndex)
	{
		OnLevel4PieceSelected.Broadcast(SelectedPieceIndex);
		return;
	}

	if (FRuntimePiece* OldPiece = FindRuntimePiece(SelectedPieceIndex))
	{
		if (OldPiece->Actor)
		{
			OldPiece->Actor->SetSelectedVisual(false);
		}
	}

	SelectedPieceIndex = PieceIndex;

	if (FRuntimePiece* NewPiece = FindRuntimePiece(SelectedPieceIndex))
	{
		if (NewPiece->Actor)
		{
			NewPiece->Actor->SetSelectedVisual(true);
		}
	}

	OnLevel4PieceSelected.Broadcast(SelectedPieceIndex);
}

void ULevel4PuzzleComponent::BeginDragSelectedPiece(const FHitResult& HitResult, APlayerController* PlayerController)
{
	if (!bSessionActive || !PlayerController)
	{
		return;
	}

	FRuntimePiece* RuntimePiece = FindRuntimePiece(SelectedPieceIndex);
	if (!RuntimePiece || !RuntimePiece->Actor)
	{
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	if (HitActor != RuntimePiece->Actor)
	{
		return;
	}

	bDragging = true;
	DragPlayerController = PlayerController;
	DraggedPiece = RuntimePiece->Actor;
	DragOffset = RuntimePiece->Actor->GetActorLocation() - HitResult.ImpactPoint;
}

void ULevel4PuzzleComponent::EndDragSelectedPiece()
{
	if (!bDragging)
	{
		return;
	}

	bDragging = false;
	DragPlayerController = nullptr;
	DraggedPiece = nullptr;
	DragOffset = FVector::ZeroVector;
	EvaluateCurrentStage();
}

void ULevel4PuzzleComponent::RotateSelectedPiece90()
{
	if (!bSessionActive)
	{
		return;
	}

	FRuntimePiece* RuntimePiece = FindRuntimePiece(SelectedPieceIndex);
	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	if (!RuntimePiece || !RuntimePiece->Actor || !StageConfig)
	{
		return;
	}

	const FVector Normal = GetPlaneNormal(*StageConfig);
	const FQuat DeltaRotation(Normal, FMath::DegreesToRadians(90.0f));
	RuntimePiece->Actor->SetActorRotation((DeltaRotation * RuntimePiece->Actor->GetActorQuat()).Rotator());
	EvaluateCurrentStage();
}

bool ULevel4PuzzleComponent::IsLevel4SessionActive() const
{
	return bSessionActive;
}

ELevel4Difficulty ULevel4PuzzleComponent::GetCurrentDifficulty() const
{
	return CurrentDifficulty;
}

ELevel4StageId ULevel4PuzzleComponent::GetCurrentStageId() const
{
	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	return StageConfig ? StageConfig->StageId : ELevel4StageId::None;
}

int32 ULevel4PuzzleComponent::GetSelectedPieceIndex() const
{
	return SelectedPieceIndex;
}

ALevel4PuzzlePieceActor* ULevel4PuzzleComponent::GetPieceActor(int32 PieceIndex) const
{
	const FRuntimePiece* RuntimePiece = FindRuntimePiece(PieceIndex);
	return RuntimePiece ? RuntimePiece->Actor : nullptr;
}

bool ULevel4PuzzleComponent::IsPieceSpawned(int32 PieceIndex) const
{
	const FRuntimePiece* RuntimePiece = FindRuntimePiece(PieceIndex);
	return RuntimePiece && RuntimePiece->Actor;
}

bool ULevel4PuzzleComponent::IsPieceCorrect(int32 PieceIndex) const
{
	const FRuntimePiece* RuntimePiece = FindRuntimePiece(PieceIndex);
	return RuntimePiece && RuntimePiece->bIsCorrect;
}

void ULevel4PuzzleComponent::BuildConfigsFromLayouts()
{
	RuntimeStageConfigs = StageConfigs;

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TSubclassOf<ALevel4PuzzleTargetLayoutActor> LayoutClass : StageLayoutClasses)
	{
		if (!LayoutClass)
		{
			continue;
		}

		ALevel4PuzzleTargetLayoutActor* LayoutActor = World->SpawnActor<ALevel4PuzzleTargetLayoutActor>(LayoutClass, FTransform::Identity);
		if (!LayoutActor)
		{
			continue;
		}

		RuntimeStageConfigs.Add(LayoutActor->StageConfig);
		LayoutActor->Destroy();
	}
}

void ULevel4PuzzleComponent::StartStage(int32 NewStageIndex)
{
	if (RuntimeStageConfigs.Num() <= 0)
	{
		bSessionActive = false;
		OnLevel4Completed.Broadcast();
		return;
	}

	while (RuntimeStageConfigs.IsValidIndex(NewStageIndex) && ShouldSkipStage(RuntimeStageConfigs[NewStageIndex].StageId))
	{
		++NewStageIndex;
	}

	if (!RuntimeStageConfigs.IsValidIndex(NewStageIndex))
	{
		bSessionActive = false;
		OnLevel4Completed.Broadcast();
		return;
	}

	for (FRuntimePiece& RuntimePiece : RuntimePieces)
	{
		if (RuntimePiece.Actor)
		{
			RuntimePiece.Actor->Destroy();
		}
	}

	RuntimePieces.Reset();
	CurrentStageIndex = NewStageIndex;
	SelectedPieceIndex = INDEX_NONE;
	bDragging = false;
	DraggedPiece = nullptr;
	DragPlayerController = nullptr;

	const FLevel4PuzzleStageConfig& StageConfig = RuntimeStageConfigs[CurrentStageIndex];
	for (const FLevel4PuzzlePieceConfig& PieceConfig : StageConfig.Pieces)
	{
		FRuntimePiece RuntimePiece;
		RuntimePiece.Config = PieceConfig;
		RuntimePieces.Add(RuntimePiece);
	}

	OnLevel4StageChanged.Broadcast(StageConfig.StageId, CurrentStageIndex);
	SelectPiece(1);
}

void ULevel4PuzzleComponent::CompleteCurrentStage()
{
	StartStage(CurrentStageIndex + 1);
}

void ULevel4PuzzleComponent::EvaluateCurrentStage()
{
	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	if (!StageConfig)
	{
		return;
	}

	int32 CorrectCount = 0;
	int32 SpawnedCount = 0;

	for (FRuntimePiece& RuntimePiece : RuntimePieces)
	{
		if (!RuntimePiece.Actor)
		{
			continue;
		}

		++SpawnedCount;

		FTransform TargetTransform = RuntimePiece.Config.TargetTransform;
		if (bSpawnRelativeToOwner && GetOwner())
		{
			TargetTransform = RuntimePiece.Config.TargetTransform * GetOwner()->GetActorTransform();
		}

		const float PositionError = FVector::Dist(RuntimePiece.Actor->GetActorLocation(), TargetTransform.GetLocation());
		const float RotationError = GetRotationErrorDegrees(RuntimePiece.Actor->GetActorRotation(), TargetTransform.GetRotation().Rotator());
		const bool bNowCorrect = PositionError <= StageConfig->PositionTolerance && RotationError <= StageConfig->RotationToleranceDegrees;

		if (bNowCorrect)
		{
			RuntimePiece.Actor->SetActorTransform(TargetTransform);
			++CorrectCount;
		}

		if (RuntimePiece.bIsCorrect != bNowCorrect)
		{
			RuntimePiece.bIsCorrect = bNowCorrect;
			RuntimePiece.Actor->SetCorrectVisual(bNowCorrect);
			OnLevel4PieceCorrectChanged.Broadcast(RuntimePiece.Config.PieceIndex, bNowCorrect);
		}
	}

	if (SpawnedCount == RuntimePieces.Num() && CorrectCount == RuntimePieces.Num() && RuntimePieces.Num() > 0)
	{
		CompleteCurrentStage();
	}
}

void ULevel4PuzzleComponent::SpawnPieceIfNeeded(int32 PieceIndex)
{
	FRuntimePiece* RuntimePiece = FindRuntimePiece(PieceIndex);
	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	UWorld* World = GetWorld();
	if (!RuntimePiece || !StageConfig || !World || RuntimePiece->Actor || !RuntimePiece->Config.PieceClass)
	{
		return;
	}

	const FTransform SpawnTransform = MakeSpawnTransform(RuntimePiece->Config, *StageConfig);
	ALevel4PuzzlePieceActor* PieceActor = World->SpawnActor<ALevel4PuzzlePieceActor>(RuntimePiece->Config.PieceClass, SpawnTransform);
	if (!PieceActor)
	{
		return;
	}

	RuntimePiece->Actor = PieceActor;
	PieceActor->InitializeLevel4Piece(PieceIndex);
	OnLevel4PieceSpawned.Broadcast(PieceIndex, PieceActor);
	EvaluateCurrentStage();
}

bool ULevel4PuzzleComponent::ShouldSkipStage(ELevel4StageId StageId) const
{
	return CurrentDifficulty == ELevel4Difficulty::Normal && StageId == ELevel4StageId::CloudFrame2;
}

FLevel4PuzzleStageConfig* ULevel4PuzzleComponent::GetCurrentStageConfig()
{
	return RuntimeStageConfigs.IsValidIndex(CurrentStageIndex) ? &RuntimeStageConfigs[CurrentStageIndex] : nullptr;
}

const FLevel4PuzzleStageConfig* ULevel4PuzzleComponent::GetCurrentStageConfig() const
{
	return RuntimeStageConfigs.IsValidIndex(CurrentStageIndex) ? &RuntimeStageConfigs[CurrentStageIndex] : nullptr;
}

ULevel4PuzzleComponent::FRuntimePiece* ULevel4PuzzleComponent::FindRuntimePiece(int32 PieceIndex)
{
	return RuntimePieces.FindByPredicate([PieceIndex](const FRuntimePiece& RuntimePiece)
	{
		return RuntimePiece.Config.PieceIndex == PieceIndex;
	});
}

const ULevel4PuzzleComponent::FRuntimePiece* ULevel4PuzzleComponent::FindRuntimePiece(int32 PieceIndex) const
{
	return RuntimePieces.FindByPredicate([PieceIndex](const FRuntimePiece& RuntimePiece)
	{
		return RuntimePiece.Config.PieceIndex == PieceIndex;
	});
}

FVector ULevel4PuzzleComponent::GetPlaneNormal(const FLevel4PuzzleStageConfig& StageConfig) const
{
	switch (StageConfig.LockedAxis)
	{
	case ELevel4LockedAxis::X:
		if (bSpawnRelativeToOwner && GetOwner())
		{
			return GetOwner()->GetActorTransform().TransformVectorNoScale(FVector::ForwardVector).GetSafeNormal();
		}
		return FVector::ForwardVector;
	case ELevel4LockedAxis::Y:
		if (bSpawnRelativeToOwner && GetOwner())
		{
			return GetOwner()->GetActorTransform().TransformVectorNoScale(FVector::RightVector).GetSafeNormal();
		}
		return FVector::RightVector;
	case ELevel4LockedAxis::Z:
	default:
		if (bSpawnRelativeToOwner && GetOwner())
		{
			return GetOwner()->GetActorTransform().TransformVectorNoScale(FVector::UpVector).GetSafeNormal();
		}
		return FVector::UpVector;
	}
}

FVector ULevel4PuzzleComponent::ProjectPointToStagePlane(const FVector& Point, const FLevel4PuzzleStageConfig& StageConfig, const FVector& PlaneOrigin) const
{
	const FVector Normal = GetPlaneNormal(StageConfig);
	return Point - FVector::DotProduct(Point - PlaneOrigin, Normal) * Normal;
}

FTransform ULevel4PuzzleComponent::MakeSpawnTransform(const FLevel4PuzzlePieceConfig& PieceConfig, const FLevel4PuzzleStageConfig& StageConfig) const
{
	FTransform TargetTransform = PieceConfig.TargetTransform;
	if (bSpawnRelativeToOwner && GetOwner())
	{
		TargetTransform = PieceConfig.TargetTransform * GetOwner()->GetActorTransform();
	}

	FVector SpawnLocation = TargetTransform.GetLocation();
	if (CurrentDifficulty == ELevel4Difficulty::Expert)
	{
		const FVector RandomOffset(
			FMath::FRandRange(StageConfig.ExpertRandomMin.X, StageConfig.ExpertRandomMax.X),
			FMath::FRandRange(StageConfig.ExpertRandomMin.Y, StageConfig.ExpertRandomMax.Y),
			FMath::FRandRange(StageConfig.ExpertRandomMin.Z, StageConfig.ExpertRandomMax.Z));
		SpawnLocation = TargetTransform.GetLocation() + RandomOffset;
		SpawnLocation = ProjectPointToStagePlane(SpawnLocation, StageConfig, TargetTransform.GetLocation());
	}

	SpawnLocation += GetPlaneNormal(StageConfig) * SpawnHeightOffset;

	const int32 QuarterTurns = FMath::RandRange(0, 3);
	const FQuat RandomRotation(GetPlaneNormal(StageConfig), FMath::DegreesToRadians(90.0f * QuarterTurns));
	const FQuat SpawnRotation = RandomRotation * TargetTransform.GetRotation();

	return FTransform(SpawnRotation, SpawnLocation, TargetTransform.GetScale3D());
}

float ULevel4PuzzleComponent::GetRotationErrorDegrees(const FRotator& A, const FRotator& B) const
{
	const FQuat Delta = A.Quaternion().Inverse() * B.Quaternion();
	return FMath::RadiansToDegrees(Delta.GetAngle());
}
