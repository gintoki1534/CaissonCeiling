#include "Level4PuzzleComponent.h"

#include "Level4PuzzlePieceActor.h"
#include "Level4PuzzleTargetLayoutActor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

namespace
{
bool IntersectSegmentBox(const FVector& SegmentStart, const FVector& SegmentEnd, const FBox& Box, float& OutDistance)
{
	if (!Box.IsValid)
	{
		return false;
	}

	const FVector Segment = SegmentEnd - SegmentStart;
	float MinTime = 0.0f;
	float MaxTime = 1.0f;

	for (int32 AxisIndex = 0; AxisIndex < 3; ++AxisIndex)
	{
		const float StartValue = SegmentStart[AxisIndex];
		const float DirectionValue = Segment[AxisIndex];
		const float MinValue = Box.Min[AxisIndex];
		const float MaxValue = Box.Max[AxisIndex];

		if (FMath::IsNearlyZero(DirectionValue))
		{
			if (StartValue < MinValue || StartValue > MaxValue)
			{
				return false;
			}
			continue;
		}

		float NearTime = (MinValue - StartValue) / DirectionValue;
		float FarTime = (MaxValue - StartValue) / DirectionValue;
		if (NearTime > FarTime)
		{
			Swap(NearTime, FarTime);
		}

		MinTime = FMath::Max(MinTime, NearTime);
		MaxTime = FMath::Min(MaxTime, FarTime);
		if (MinTime > MaxTime)
		{
			return false;
		}
	}

	OutDistance = Segment.Size() * MinTime;
	return true;
}
}

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

	FVector Intersection;
	if (!GetCursorIntersectionOnStagePlane(DragPlayerController, *StageConfig, DragPlaneOrigin, Intersection))
	{
		return;
	}

	const FVector NewLocation = ProjectPointToStagePlane(Intersection + DragOffset, *StageConfig, DragActorPlaneOrigin);
	DraggedPiece->SetActorLocation(NewLocation);
}

void ULevel4PuzzleComponent::StartLevel4Puzzle(ELevel4Difficulty InDifficulty)
{
	BuildConfigsFromLayouts();
	CurrentDifficulty = InDifficulty;
	bDifficultySelected = false;
	bSessionActive = true;
	CurrentPhase = ELevel4PuzzlePhase::Playing;

	StartStage(0);
}

void ULevel4PuzzleComponent::SetLevel4Difficulty(ELevel4Difficulty InDifficulty)
{
	const bool bWasDifficultySelected = bDifficultySelected;
	bDifficultySelected = true;

	if (bWasDifficultySelected && CurrentDifficulty == InDifficulty)
	{
		return;
	}

	CurrentDifficulty = InDifficulty;
	OnLevel4DifficultyChanged.Broadcast(CurrentDifficulty);
}

void ULevel4PuzzleComponent::SelectPiece(int32 PieceIndex)
{
	SelectPieceInternal(PieceIndex, true);
}

void ULevel4PuzzleComponent::SelectPieceInternal(int32 PieceIndex, bool bUserInitiated)
{
	if (!bSessionActive)
	{
		return;
	}

	if (CurrentPhase != ELevel4PuzzlePhase::Playing)
	{
		return;
	}

	if (bUserInitiated)
	{
		EnsureDifficultySelected();
	}

	RightClickPrimedPieceIndex = INDEX_NONE;
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

void ULevel4PuzzleComponent::EnsureDifficultySelected()
{
	if (bDifficultySelected)
	{
		return;
	}

	bDifficultySelected = true;
	CurrentDifficulty = ELevel4Difficulty::Normal;
	OnLevel4DifficultyChanged.Broadcast(CurrentDifficulty);
}

void ULevel4PuzzleComponent::ContinueLevel4AfterStageSolved()
{
	if (!bSessionActive || CurrentPhase != ELevel4PuzzlePhase::StageCompleted)
	{
		return;
	}

	StartStage(CurrentStageIndex + 1);
}

void ULevel4PuzzleComponent::BeginDragSelectedPiece(const FHitResult& HitResult, APlayerController* PlayerController)
{
	if (!bSessionActive || CurrentPhase != ELevel4PuzzlePhase::Playing || !PlayerController)
	{
		return;
	}

	EnsureDifficultySelected();

	ALevel4PuzzlePieceActor* HitPiece = ResolveHitPieceActor(HitResult);
	if (!HitPiece || !IsRuntimePieceActor(HitPiece))
	{
		LogHitDebug(TEXT("BeginDrag rejected: hit is not a spawned Level4 piece"), HitResult, HitPiece);
		return;
	}

	if (SelectedPieceIndex != HitPiece->PieceIndex)
	{
		SelectPieceInternal(HitPiece->PieceIndex, false);
	}
	RightClickPrimedPieceIndex = INDEX_NONE;

	FRuntimePiece* RuntimePiece = FindRuntimePiece(HitPiece->PieceIndex);
	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	if (!RuntimePiece || !RuntimePiece->Actor || !StageConfig)
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Level4][BeginDrag] Runtime invalid. HitPiece=%s HitPieceIndex=%d RuntimePiece=%s Actor=%s StageConfig=%s Selected=%d"),
				*GetNameSafe(HitPiece),
				HitPiece ? HitPiece->PieceIndex : INDEX_NONE,
				RuntimePiece ? TEXT("Valid") : TEXT("None"),
				RuntimePiece ? *GetNameSafe(RuntimePiece->Actor) : TEXT("None"),
				StageConfig ? TEXT("Valid") : TEXT("None"),
				SelectedPieceIndex);
		}
		return;
	}

	bDragging = true;
	DragPlayerController = PlayerController;
	DraggedPiece = RuntimePiece->Actor;
	DragActorPlaneOrigin = RuntimePiece->Actor->GetActorLocation();
	DragPlaneOrigin = HitResult.ImpactPoint;
	if (DragPlaneOrigin.ContainsNaN())
	{
		DragPlaneOrigin = HitResult.Location;
	}
	if (DragPlaneOrigin.ContainsNaN())
	{
		DragPlaneOrigin = RuntimePiece->Actor->GetActorLocation();
	}

	FVector PlaneIntersection;
	if (!GetCursorIntersectionOnStagePlane(PlayerController, *StageConfig, DragPlaneOrigin, PlaneIntersection, TEXT("BeginDrag")))
	{
		bDragging = false;
		DragPlayerController = nullptr;
		DraggedPiece = nullptr;
		DragPlaneOrigin = FVector::ZeroVector;
		DragActorPlaneOrigin = FVector::ZeroVector;
		return;
	}

	DragOffset = RuntimePiece->Actor->GetActorLocation() - PlaneIntersection;

	if (bEnableLevel4DebugLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("[Level4][BeginDrag] Started. Piece=%s PieceIndex=%d LockedAxis=%d CursorPlaneOrigin=%s ActorPlaneOrigin=%s HitImpact=%s PlaneIntersection=%s DragOffset=%s"),
			*GetNameSafe(RuntimePiece->Actor),
			RuntimePiece->Actor->PieceIndex,
			static_cast<int32>(StageConfig->LockedAxis),
			*DragPlaneOrigin.ToString(),
			*DragActorPlaneOrigin.ToString(),
			*HitResult.ImpactPoint.ToString(),
			*PlaneIntersection.ToString(),
			*DragOffset.ToString());
	}
}

void ULevel4PuzzleComponent::EndDragSelectedPiece()
{
	if (!bDragging)
	{
		return;
	}

	ALevel4PuzzlePieceActor* ReleasedPiece = DraggedPiece;
	bDragging = false;
	DragPlayerController = nullptr;
	DraggedPiece = nullptr;
	DragOffset = FVector::ZeroVector;
	DragPlaneOrigin = FVector::ZeroVector;
	DragActorPlaneOrigin = FVector::ZeroVector;
	SnapPieceToBestAnchorIfClose(ReleasedPiece);
	EvaluateCurrentStage();
}

void ULevel4PuzzleComponent::RotateSelectedPiece90()
{
	if (!bSessionActive || CurrentPhase != ELevel4PuzzlePhase::Playing)
	{
		return;
	}

	EnsureDifficultySelected();

	FRuntimePiece* RuntimePiece = FindRuntimePiece(SelectedPieceIndex);
	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	if (!RuntimePiece || !RuntimePiece->Actor || !StageConfig)
	{
		return;
	}
	if (!CanRotateStage(*StageConfig))
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("[Level4][Rotate] Rotation disabled for current stage. Stage=%d Piece=%d"),
				static_cast<int32>(StageConfig->StageId),
				RuntimePiece->Config.PieceIndex);
		}
		return;
	}

	const FVector Normal = GetPlaneNormal(*StageConfig);
	const FQuat DeltaRotation(Normal, FMath::DegreesToRadians(90.0f));
	RuntimePiece->Actor->SetActorRotation((DeltaRotation * RuntimePiece->Actor->GetActorQuat()).Rotator());
	EvaluateCurrentStage();
}

void ULevel4PuzzleComponent::RotateSelectedPiece90AroundPivot(const FVector& PivotLocation)
{
	if (!bSessionActive || CurrentPhase != ELevel4PuzzlePhase::Playing)
	{
		return;
	}

	FRuntimePiece* RuntimePiece = FindRuntimePiece(SelectedPieceIndex);
	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	if (!RuntimePiece || !RuntimePiece->Actor || !StageConfig)
	{
		return;
	}
	if (!CanRotateStage(*StageConfig))
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("[Level4][RightClick] Rotation disabled for current stage. Stage=%d Piece=%d"),
				static_cast<int32>(StageConfig->StageId),
				RuntimePiece->Config.PieceIndex);
		}
		return;
	}

	const FVector Normal = GetPlaneNormal(*StageConfig);
	const FQuat DeltaRotation(Normal, FMath::DegreesToRadians(90.0f));
	const FVector OldLocation = RuntimePiece->Actor->GetActorLocation();
	const FVector NewLocation = PivotLocation + DeltaRotation.RotateVector(OldLocation - PivotLocation);
	const FQuat NewRotation = DeltaRotation * RuntimePiece->Actor->GetActorQuat();

	RuntimePiece->Actor->SetActorLocationAndRotation(NewLocation, NewRotation.Rotator());

	if (bEnableLevel4DebugLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("[Level4][RightClick] Rotated around pivot. Piece=%d Pivot=%s OldLocation=%s NewLocation=%s PivotOldDistance=%f PivotNewDistance=%f"),
			RuntimePiece->Config.PieceIndex,
			*PivotLocation.ToString(),
			*OldLocation.ToString(),
			*NewLocation.ToString(),
			FVector::Dist(PivotLocation, OldLocation),
			FVector::Dist(PivotLocation, NewLocation));
	}

	EvaluateCurrentStage();
}

void ULevel4PuzzleComponent::HandleRightClickPiece(const FHitResult& HitResult)
{
	if (!bSessionActive || CurrentPhase != ELevel4PuzzlePhase::Playing)
	{
		return;
	}

	EnsureDifficultySelected();

	ALevel4PuzzlePieceActor* HitPiece = ResolveHitPieceActor(HitResult);
	if (!HitPiece || !IsRuntimePieceActor(HitPiece))
	{
		LogHitDebug(TEXT("RightClick rejected: hit is not a spawned Level4 piece"), HitResult, HitPiece);
		return;
	}

	if (RightClickPrimedPieceIndex != HitPiece->PieceIndex)
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("[Level4][RightClick] Priming hit piece. HitPiece=%s HitPieceIndex=%d PreviousSelected=%d PreviousPrimed=%d"),
				*GetNameSafe(HitPiece),
				HitPiece->PieceIndex,
				SelectedPieceIndex,
				RightClickPrimedPieceIndex);
		}
		if (SelectedPieceIndex != HitPiece->PieceIndex)
		{
			SelectPieceInternal(HitPiece->PieceIndex, false);
		}
		RightClickPrimedPieceIndex = HitPiece->PieceIndex;
		return;
	}

	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	if (!StageConfig || !CanRotateStage(*StageConfig))
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("[Level4][RightClick] Rotation disabled after priming. Stage=%d Piece=%s PieceIndex=%d"),
				StageConfig ? static_cast<int32>(StageConfig->StageId) : INDEX_NONE,
				*GetNameSafe(HitPiece),
				HitPiece->PieceIndex);
		}
		return;
	}

	if (bEnableLevel4DebugLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("[Level4][RightClick] Rotating selected piece. Piece=%s PieceIndex=%d"),
			*GetNameSafe(HitPiece),
			HitPiece->PieceIndex);
	}
	FVector PivotLocation = HitResult.ImpactPoint;
	if (PivotLocation.ContainsNaN())
	{
		PivotLocation = HitPiece->GetActorLocation();
	}
	if (bEnableLevel4DebugLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("[Level4][RightClick] Rotation pivot chosen. Piece=%s PieceIndex=%d BlockingHit=%d ImpactPoint=%s Location=%s ActorLocation=%s Pivot=%s PivotActorDistance=%f"),
			*GetNameSafe(HitPiece),
			HitPiece->PieceIndex,
			HitResult.bBlockingHit ? 1 : 0,
			*HitResult.ImpactPoint.ToString(),
			*HitResult.Location.ToString(),
			*HitPiece->GetActorLocation().ToString(),
			*PivotLocation.ToString(),
			FVector::Dist(PivotLocation, HitPiece->GetActorLocation()));
	}
	RotateSelectedPiece90AroundPivot(PivotLocation);
}

void ULevel4PuzzleComponent::MoveSelectedPieceToTargetLocationKeepingRotation()
{
	if (!bSessionActive || CurrentPhase != ELevel4PuzzlePhase::Playing)
	{
		return;
	}

	EnsureDifficultySelected();

	FRuntimePiece* RuntimePiece = FindRuntimePiece(SelectedPieceIndex);
	if (!RuntimePiece || !RuntimePiece->Actor)
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Level4][MoveToTarget] No spawned selected piece. Selected=%d"), SelectedPieceIndex);
		}
		return;
	}

	const FTransform TargetTransform = GetWorldTargetTransform(RuntimePiece->Config);
	RuntimePiece->Actor->SetActorLocation(TargetTransform.GetLocation());

	if (bEnableLevel4DebugLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("[Level4][MoveToTarget] Moved selected piece to target location. Piece=%d TargetLocation=%s RotationKept=%s"),
			RuntimePiece->Config.PieceIndex,
			*TargetTransform.GetLocation().ToString(),
			*RuntimePiece->Actor->GetActorRotation().ToString());
	}

	EvaluateCurrentStage();
}

bool ULevel4PuzzleComponent::FindSpawnedPieceHitOnRay(const FVector& RayStart, const FVector& RayEnd, FHitResult& OutHitResult) const
{
	bool bFoundComponentHit = false;
	bool bFoundBoundsHit = false;
	float BestComponentDistance = TNumericLimits<float>::Max();
	float BestBoundsDistance = TNumericLimits<float>::Max();
	FHitResult BestComponentHitResult;
	FHitResult BestBoundsHitResult;
	FString DebugPieces;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(Level4PieceOnlyHit), true);
	if (const AActor* OwnerActor = GetOwner())
	{
		QueryParams.AddIgnoredActor(OwnerActor);
	}

	for (const FRuntimePiece& RuntimePiece : RuntimePieces)
	{
		ALevel4PuzzlePieceActor* PieceActor = RuntimePiece.Actor;
		if (!PieceActor)
		{
			continue;
		}

		TArray<UPrimitiveComponent*> InteractiveComponents;
		PieceActor->GetInteractivePrimitiveComponents(InteractiveComponents);
		if (InteractiveComponents.Num() <= 0)
		{
			if (UPrimitiveComponent* DragPrimitive = PieceActor->GetDragPrimitive())
			{
				InteractiveComponents.Add(DragPrimitive);
			}
		}

		for (UPrimitiveComponent* InteractiveComponent : InteractiveComponents)
		{
			if (!IsValid(InteractiveComponent) || !InteractiveComponent->IsVisible())
			{
				continue;
			}

			FHitResult ComponentHitResult;
			if (InteractiveComponent->LineTraceComponent(ComponentHitResult, RayStart, RayEnd, QueryParams))
			{
				const float HitDistance = FVector::Dist(RayStart, ComponentHitResult.ImpactPoint);
				if (HitDistance < BestComponentDistance)
				{
					BestComponentDistance = HitDistance;
					BestComponentHitResult = ComponentHitResult;
					BestComponentHitResult.bBlockingHit = true;
					BestComponentHitResult.HitObjectHandle = FActorInstanceHandle(PieceActor);
					BestComponentHitResult.Component = InteractiveComponent;
					if (BestComponentHitResult.ImpactPoint.IsNearlyZero() && !BestComponentHitResult.Location.IsNearlyZero())
					{
						BestComponentHitResult.ImpactPoint = BestComponentHitResult.Location;
					}
					bFoundComponentHit = true;
				}
				continue;
			}

			float BoundsDistance = 0.0f;
			const FBox BoundsBox = InteractiveComponent->Bounds.GetBox();
			if (IntersectSegmentBox(RayStart, RayEnd, BoundsBox, BoundsDistance) && BoundsDistance < BestBoundsDistance)
			{
				const FVector RayDirection = (RayEnd - RayStart).GetSafeNormal();
				const FVector HitLocation = RayStart + RayDirection * BoundsDistance;
				BestBoundsDistance = BoundsDistance;
				BestBoundsHitResult = FHitResult(PieceActor, InteractiveComponent, HitLocation, -RayDirection);
				BestBoundsHitResult.bBlockingHit = true;
				BestBoundsHitResult.HitObjectHandle = FActorInstanceHandle(PieceActor);
				BestBoundsHitResult.Component = InteractiveComponent;
				BestBoundsHitResult.Location = HitLocation;
				BestBoundsHitResult.ImpactPoint = HitLocation;
				BestBoundsHitResult.Distance = BoundsDistance;
				bFoundBoundsHit = true;
			}
		}

		if (bEnableLevel4DebugLogs)
		{
			const FBox ActorBounds = PieceActor->GetComponentsBoundingBox(true);
			DebugPieces += FString::Printf(TEXT(" Piece=%s Index=%d Bounds=%s;"),
				*GetNameSafe(PieceActor),
				PieceActor->PieceIndex,
				*ActorBounds.ToString());
		}
	}

	if (bFoundComponentHit || bFoundBoundsHit)
	{
		const bool bUseComponentHit = bFoundComponentHit;
		OutHitResult = bUseComponentHit ? BestComponentHitResult : BestBoundsHitResult;
		const float BestDistance = bUseComponentHit ? BestComponentDistance : BestBoundsDistance;
		if (bEnableLevel4DebugLogs)
		{
			ALevel4PuzzlePieceActor* HitPiece = ResolveHitPieceActor(OutHitResult);
			UE_LOG(LogTemp, Log, TEXT("[Level4][Hit] Piece-only trace selected spawned piece. HitType=%s Piece=%s PieceIndex=%d HitComponent=%s Distance=%f ImpactPoint=%s"),
				bUseComponentHit ? TEXT("Component") : TEXT("BoundsFallback"),
				*GetNameSafe(HitPiece),
				HitPiece ? HitPiece->PieceIndex : INDEX_NONE,
				*GetNameSafe(OutHitResult.GetComponent()),
				BestDistance,
				*OutHitResult.ImpactPoint.ToString());
		}
		return true;
	}

	if (bEnableLevel4DebugLogs)
	{
		int32 SpawnedPieceCount = 0;
		for (const FRuntimePiece& RuntimePiece : RuntimePieces)
		{
			if (RuntimePiece.Actor)
			{
				++SpawnedPieceCount;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("[Level4][Hit] Piece-only trace found no spawned piece. RayStart=%s RayEnd=%s SpawnedPieces=%d%s"),
			*RayStart.ToString(),
			*RayEnd.ToString(),
			SpawnedPieceCount,
			*DebugPieces);
	}

	return false;
}

bool ULevel4PuzzleComponent::IsLevel4SessionActive() const
{
	return bSessionActive && CurrentPhase != ELevel4PuzzlePhase::Inactive && CurrentPhase != ELevel4PuzzlePhase::Completed;
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

ELevel4PuzzlePhase ULevel4PuzzleComponent::GetCurrentPhase() const
{
	return CurrentPhase;
}

bool ULevel4PuzzleComponent::HasLevel4DifficultySelection() const
{
	return bDifficultySelected;
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
		CurrentPhase = ELevel4PuzzlePhase::Completed;
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
		CurrentPhase = ELevel4PuzzlePhase::Completed;
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
	CurrentPhase = ELevel4PuzzlePhase::Playing;
	SelectedPieceIndex = INDEX_NONE;
	RightClickPrimedPieceIndex = INDEX_NONE;
	bDragging = false;
	DraggedPiece = nullptr;
	DragPlayerController = nullptr;
	DragPlaneOrigin = FVector::ZeroVector;
	DragActorPlaneOrigin = FVector::ZeroVector;

	const FLevel4PuzzleStageConfig& StageConfig = RuntimeStageConfigs[CurrentStageIndex];
	for (const FLevel4PuzzlePieceConfig& PieceConfig : StageConfig.Pieces)
	{
		FRuntimePiece RuntimePiece;
		RuntimePiece.Config = PieceConfig;
		RuntimePieces.Add(RuntimePiece);
	}

	OnLevel4StageChanged.Broadcast(StageConfig.StageId, CurrentStageIndex);
	SelectPieceInternal(1, false);
}

void ULevel4PuzzleComponent::CompleteCurrentStage()
{
	if (!RuntimeStageConfigs.IsValidIndex(CurrentStageIndex) || CurrentPhase != ELevel4PuzzlePhase::Playing)
	{
		return;
	}

	CurrentPhase = ELevel4PuzzlePhase::StageCompleted;
	bDragging = false;
	DraggedPiece = nullptr;
	DragPlayerController = nullptr;
	DragOffset = FVector::ZeroVector;
	DragPlaneOrigin = FVector::ZeroVector;
	DragActorPlaneOrigin = FVector::ZeroVector;

	const FLevel4PuzzleStageConfig& StageConfig = RuntimeStageConfigs[CurrentStageIndex];
	OnLevel4StageSolved.Broadcast(StageConfig.StageId, CurrentStageIndex);
}

void ULevel4PuzzleComponent::EvaluateCurrentStage()
{
	if (CurrentPhase != ELevel4PuzzlePhase::Playing)
	{
		return;
	}

	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	if (!StageConfig)
	{
		return;
	}

	int32 CorrectCount = 0;
	int32 SpawnedCount = 0;
	bool bAllPiecesAlignedToSameAnchor = false;

	for (const FRuntimePiece& AnchorPiece : RuntimePieces)
	{
		if (!AnchorPiece.Actor)
		{
			continue;
		}

		int32 AlignedToAnchorCount = 1;
		for (const FRuntimePiece& RuntimePiece : RuntimePieces)
		{
			if (!RuntimePiece.Actor || RuntimePiece.Config.PieceIndex == AnchorPiece.Config.PieceIndex)
			{
				continue;
			}

			float PositionError = 0.0f;
			float RotationError = 0.0f;
			if (IsPieceAlignedToAnchor(RuntimePiece, AnchorPiece, *StageConfig, PositionError, RotationError))
			{
				++AlignedToAnchorCount;
			}
		}

		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("[Level4][Evaluate] Anchor=%d AlignedCount=%d TotalPieces=%d"),
				AnchorPiece.Config.PieceIndex,
				AlignedToAnchorCount,
				RuntimePieces.Num());
		}

		if (AlignedToAnchorCount == RuntimePieces.Num() && RuntimePieces.Num() > 0)
		{
			bAllPiecesAlignedToSameAnchor = true;
			break;
		}
	}

	for (FRuntimePiece& RuntimePiece : RuntimePieces)
	{
		if (!RuntimePiece.Actor)
		{
			continue;
		}

		++SpawnedCount;

		bool bNowCorrect = false;
		float BestPositionError = TNumericLimits<float>::Max();
		float BestRotationError = TNumericLimits<float>::Max();

		for (const FRuntimePiece& AnchorPiece : RuntimePieces)
		{
			if (!AnchorPiece.Actor || AnchorPiece.Config.PieceIndex == RuntimePiece.Config.PieceIndex)
			{
				continue;
			}

			float PositionError = 0.0f;
			float RotationError = 0.0f;
			if (IsPieceAlignedToAnchor(RuntimePiece, AnchorPiece, *StageConfig, PositionError, RotationError))
			{
				bNowCorrect = true;
				BestPositionError = PositionError;
				BestRotationError = RotationError;
				break;
			}

			if (PositionError < BestPositionError || (FMath::IsNearlyEqual(PositionError, BestPositionError) && RotationError < BestRotationError))
			{
				BestPositionError = PositionError;
				BestRotationError = RotationError;
			}
		}

		if (RuntimePiece.bIsCorrect != bNowCorrect)
		{
			RuntimePiece.bIsCorrect = bNowCorrect;
			RuntimePiece.Actor->SetCorrectVisual(bNowCorrect);
			OnLevel4PieceCorrectChanged.Broadcast(RuntimePiece.Config.PieceIndex, bNowCorrect);
		}

		if (bNowCorrect)
		{
			++CorrectCount;
		}

		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("[Level4][Evaluate] Piece=%d Correct=%d BestRelativePositionError=%f BestRelativeRotationError=%f Tolerance=(%f,%f)"),
				RuntimePiece.Config.PieceIndex,
				bNowCorrect ? 1 : 0,
				BestPositionError,
				BestRotationError,
				StageConfig->PositionTolerance,
				StageConfig->RotationToleranceDegrees);
		}
	}

	if (SpawnedCount == RuntimePieces.Num() && bAllPiecesAlignedToSameAnchor && RuntimePieces.Num() > 0)
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("[Level4][Evaluate] Stage completed by relative alignment. Spawned=%d CorrectVisualPieces=%d StageIndex=%d"), SpawnedCount, CorrectCount, CurrentStageIndex);
		}
		CompleteCurrentStage();
	}
}

void ULevel4PuzzleComponent::SpawnPieceIfNeeded(int32 PieceIndex)
{
	FRuntimePiece* RuntimePiece = FindRuntimePiece(PieceIndex);
	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	UWorld* World = GetWorld();
	if (!RuntimePiece)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Level4] 当前阶段没有 PieceIndex=%d 的配置，请检查 StageConfig.Pieces 的 PieceIndex 是否为 1..5。"), PieceIndex);
		return;
	}

	if (!StageConfig || !World || RuntimePiece->Actor)
	{
		return;
	}

	if (!RuntimePiece->Config.PieceClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Level4] PieceIndex=%d 的 PieceClass 为空，请在当前 Layout 的 StageConfig.Pieces 中配置碎片蓝图类。"), PieceIndex);
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

void ULevel4PuzzleComponent::SnapPieceToBestAnchorIfClose(ALevel4PuzzlePieceActor* PieceActor)
{
	if (!bEnableSnapOnDragRelease || !PieceActor)
	{
		return;
	}

	FRuntimePiece* RuntimePiece = FindRuntimePiece(PieceActor->PieceIndex);
	const FLevel4PuzzleStageConfig* StageConfig = GetCurrentStageConfig();
	if (!RuntimePiece || !RuntimePiece->Actor || !StageConfig)
	{
		return;
	}

	bool bFoundSnapTarget = false;
	float BestPositionError = TNumericLimits<float>::Max();
	float BestRotationError = TNumericLimits<float>::Max();
	int32 BestAnchorPieceIndex = INDEX_NONE;
	FTransform BestSnapTransform = RuntimePiece->Actor->GetActorTransform();

	for (const FRuntimePiece& AnchorPiece : RuntimePieces)
	{
		if (!AnchorPiece.Actor || AnchorPiece.Config.PieceIndex == RuntimePiece->Config.PieceIndex)
		{
			continue;
		}

		float PositionError = 0.0f;
		float RotationError = 0.0f;
		if (!IsPieceAlignedToAnchor(*RuntimePiece, AnchorPiece, *StageConfig, PositionError, RotationError))
		{
			if (bEnableLevel4DebugLogs)
			{
				UE_LOG(LogTemp, Verbose, TEXT("[Level4][Snap] Anchor rejected. Piece=%d Anchor=%d PositionError=%f RotationError=%f Tolerance=(%f,%f)"),
					RuntimePiece->Config.PieceIndex,
					AnchorPiece.Config.PieceIndex,
					PositionError,
					RotationError,
					StageConfig->PositionTolerance,
					StageConfig->RotationToleranceDegrees);
			}
			continue;
		}

		if (PositionError < BestPositionError || (FMath::IsNearlyEqual(PositionError, BestPositionError) && RotationError < BestRotationError))
		{
			const FTransform PieceTargetTransform = GetWorldTargetTransform(RuntimePiece->Config);
			const FTransform AnchorTargetTransform = GetWorldTargetTransform(AnchorPiece.Config);
			const FTransform PieceTargetRelativeToAnchor = PieceTargetTransform.GetRelativeTransform(AnchorTargetTransform);

			bFoundSnapTarget = true;
			BestPositionError = PositionError;
			BestRotationError = RotationError;
			BestAnchorPieceIndex = AnchorPiece.Config.PieceIndex;
			BestSnapTransform = PieceTargetRelativeToAnchor * AnchorPiece.Actor->GetActorTransform();
		}
	}

	if (!bFoundSnapTarget)
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("[Level4][Snap] No snap target. Piece=%d"), RuntimePiece->Config.PieceIndex);
		}
		return;
	}

	RuntimePiece->Actor->SetActorTransform(BestSnapTransform);

	if (bEnableLevel4DebugLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("[Level4][Snap] Snapped piece to anchor. Piece=%d Anchor=%d PositionError=%f RotationError=%f NewTransform=%s"),
			RuntimePiece->Config.PieceIndex,
			BestAnchorPieceIndex,
			BestPositionError,
			BestRotationError,
			*BestSnapTransform.ToHumanReadableString());
	}
}

bool ULevel4PuzzleComponent::CanRotateStage(const FLevel4PuzzleStageConfig& StageConfig) const
{
	return StageConfig.StageId != ELevel4StageId::FinalAssembly;
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

ALevel4PuzzlePieceActor* ULevel4PuzzleComponent::ResolveHitPieceActor(const FHitResult& HitResult) const
{
	TSet<AActor*> VisitedActors;
	TArray<AActor*> PendingActors;

	if (AActor* HitActor = HitResult.GetActor())
	{
		PendingActors.Add(HitActor);
	}

	if (UActorComponent* HitComponent = HitResult.GetComponent())
	{
		if (AActor* ComponentOwner = HitComponent->GetOwner())
		{
			PendingActors.Add(ComponentOwner);
		}
	}

	while (PendingActors.Num() > 0)
	{
		AActor* CurrentActor = PendingActors.Pop(EAllowShrinking::No);
		if (!CurrentActor || VisitedActors.Contains(CurrentActor))
		{
			continue;
		}

		VisitedActors.Add(CurrentActor);

		if (ALevel4PuzzlePieceActor* PieceActor = Cast<ALevel4PuzzlePieceActor>(CurrentActor))
		{
			return PieceActor;
		}

		if (AActor* OwnerActor = CurrentActor->GetOwner())
		{
			PendingActors.Add(OwnerActor);
		}

		if (AActor* AttachParentActor = CurrentActor->GetAttachParentActor())
		{
			PendingActors.Add(AttachParentActor);
		}

		if (AActor* ParentActor = CurrentActor->GetParentActor())
		{
			PendingActors.Add(ParentActor);
		}
	}

	return nullptr;
}

bool ULevel4PuzzleComponent::IsRuntimePieceActor(const ALevel4PuzzlePieceActor* PieceActor) const
{
	if (!PieceActor)
	{
		return false;
	}

	const FRuntimePiece* RuntimePiece = FindRuntimePiece(PieceActor->PieceIndex);
	return RuntimePiece && RuntimePiece->Actor == PieceActor;
}

void ULevel4PuzzleComponent::LogHitDebug(const TCHAR* Context, const FHitResult& HitResult, const ALevel4PuzzlePieceActor* ResolvedPiece) const
{
	if (!bEnableLevel4DebugLogs)
	{
		return;
	}

	const UActorComponent* HitComponent = HitResult.GetComponent();
	UE_LOG(LogTemp, Warning, TEXT("[Level4][Hit] %s. BlockingHit=%d HitActor=%s HitActorClass=%s HitComponent=%s ComponentOwner=%s ResolvedPiece=%s ResolvedPieceIndex=%d Selected=%d"),
		Context,
		HitResult.bBlockingHit ? 1 : 0,
		*GetNameSafe(HitResult.GetActor()),
		HitResult.GetActor() ? *GetNameSafe(HitResult.GetActor()->GetClass()) : TEXT("None"),
		*GetNameSafe(HitComponent),
		HitComponent ? *GetNameSafe(HitComponent->GetOwner()) : TEXT("None"),
		*GetNameSafe(ResolvedPiece),
		ResolvedPiece ? ResolvedPiece->PieceIndex : INDEX_NONE,
		SelectedPieceIndex);
}

FTransform ULevel4PuzzleComponent::GetWorldTargetTransform(const FLevel4PuzzlePieceConfig& PieceConfig) const
{
	if (bSpawnRelativeToOwner && GetOwner())
	{
		return PieceConfig.TargetTransform * GetOwner()->GetActorTransform();
	}

	return PieceConfig.TargetTransform;
}

bool ULevel4PuzzleComponent::IsPieceAlignedToAnchor(const FRuntimePiece& RuntimePiece, const FRuntimePiece& AnchorPiece, const FLevel4PuzzleStageConfig& StageConfig, float& OutPositionError, float& OutRotationError) const
{
	OutPositionError = TNumericLimits<float>::Max();
	OutRotationError = TNumericLimits<float>::Max();

	if (!RuntimePiece.Actor || !AnchorPiece.Actor)
	{
		return false;
	}

	const FTransform PieceTargetTransform = GetWorldTargetTransform(RuntimePiece.Config);
	const FTransform AnchorTargetTransform = GetWorldTargetTransform(AnchorPiece.Config);
	const FTransform PieceTargetRelativeToAnchor = PieceTargetTransform.GetRelativeTransform(AnchorTargetTransform);
	const FTransform ExpectedPieceTransform = PieceTargetRelativeToAnchor * AnchorPiece.Actor->GetActorTransform();

	OutPositionError = FVector::Dist(RuntimePiece.Actor->GetActorLocation(), ExpectedPieceTransform.GetLocation());
	OutRotationError = GetRotationErrorDegrees(RuntimePiece.Actor->GetActorRotation(), ExpectedPieceTransform.GetRotation().Rotator());

	return OutPositionError <= StageConfig.PositionTolerance && OutRotationError <= StageConfig.RotationToleranceDegrees;
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

bool ULevel4PuzzleComponent::GetCursorIntersectionOnStagePlane(APlayerController* PlayerController, const FLevel4PuzzleStageConfig& StageConfig, const FVector& PlaneOrigin, FVector& OutIntersection, const TCHAR* DebugContext) const
{
	if (!PlayerController)
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Level4][Plane] %s failed: PlayerController is null."), DebugContext);
		}
		return false;
	}

	FVector WorldDirection;
	FVector WorldLocation;
	if (!PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Level4][Plane] %s failed: DeprojectMousePositionToWorld returned false."), DebugContext);
		}
		return false;
	}

	const FVector PlaneNormal = GetPlaneNormal(StageConfig);
	const float Denominator = FVector::DotProduct(WorldDirection, PlaneNormal);
	if (FMath::IsNearlyZero(Denominator))
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Level4][Plane] %s failed: mouse ray is parallel to movement plane. LockedAxis=%d WorldLocation=%s WorldDirection=%s PlaneOrigin=%s PlaneNormal=%s Denominator=%f"),
				DebugContext,
				static_cast<int32>(StageConfig.LockedAxis),
				*WorldLocation.ToString(),
				*WorldDirection.ToString(),
				*PlaneOrigin.ToString(),
				*PlaneNormal.ToString(),
				Denominator);
		}
		return false;
	}

	const float Distance = FVector::DotProduct(PlaneOrigin - WorldLocation, PlaneNormal) / Denominator;
	OutIntersection = WorldLocation + WorldDirection * Distance;
	return true;
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
		SpawnLocation = SpawnOrigin;
		if (bSpawnRelativeToOwner && GetOwner())
		{
			SpawnLocation = GetOwner()->GetActorTransform().TransformPosition(SpawnOrigin);
		}
	}

	SpawnLocation += GetPlaneNormal(StageConfig) * SpawnHeightOffset;

	FQuat SpawnRotation = TargetTransform.GetRotation();
	if (CanRotateStage(StageConfig))
	{
		const int32 QuarterTurns = FMath::RandRange(0, 3);
		const FQuat RandomRotation(GetPlaneNormal(StageConfig), FMath::DegreesToRadians(90.0f * QuarterTurns));
		SpawnRotation = RandomRotation * SpawnRotation;
	}

	return FTransform(SpawnRotation, SpawnLocation, TargetTransform.GetScale3D());
}

float ULevel4PuzzleComponent::GetRotationErrorDegrees(const FRotator& A, const FRotator& B) const
{
	const FQuat Delta = A.Quaternion().Inverse() * B.Quaternion();
	return FMath::RadiansToDegrees(Delta.GetAngle());
}
