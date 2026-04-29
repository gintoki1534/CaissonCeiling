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

	const FVector NewLocation = ProjectPointToStagePlane(Intersection + DragOffset, *StageConfig, DragPlaneOrigin);
	DraggedPiece->SetActorLocation(NewLocation);
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

	ALevel4PuzzlePieceActor* HitPiece = ResolveHitPieceActor(HitResult);
	if (!HitPiece || !IsRuntimePieceActor(HitPiece))
	{
		LogHitDebug(TEXT("BeginDrag rejected: hit is not a spawned Level4 piece"), HitResult, HitPiece);
		return;
	}

	if (SelectedPieceIndex != HitPiece->PieceIndex)
	{
		SelectPiece(HitPiece->PieceIndex);
	}

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
	DragPlaneOrigin = RuntimePiece->Actor->GetActorLocation();

	FVector PlaneIntersection;
	if (!GetCursorIntersectionOnStagePlane(PlayerController, *StageConfig, DragPlaneOrigin, PlaneIntersection, TEXT("BeginDrag")))
	{
		bDragging = false;
		DragPlayerController = nullptr;
		DraggedPiece = nullptr;
		DragPlaneOrigin = FVector::ZeroVector;
		return;
	}

	DragOffset = RuntimePiece->Actor->GetActorLocation() - PlaneIntersection;

	if (bEnableLevel4DebugLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("[Level4][BeginDrag] Started. Piece=%s PieceIndex=%d LockedAxis=%d PlaneOrigin=%s PlaneIntersection=%s DragOffset=%s"),
			*GetNameSafe(RuntimePiece->Actor),
			RuntimePiece->Actor->PieceIndex,
			static_cast<int32>(StageConfig->LockedAxis),
			*DragPlaneOrigin.ToString(),
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

	bDragging = false;
	DragPlayerController = nullptr;
	DraggedPiece = nullptr;
	DragOffset = FVector::ZeroVector;
	DragPlaneOrigin = FVector::ZeroVector;
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

void ULevel4PuzzleComponent::HandleRightClickPiece(const FHitResult& HitResult)
{
	if (!bSessionActive)
	{
		return;
	}

	ALevel4PuzzlePieceActor* HitPiece = ResolveHitPieceActor(HitResult);
	if (!HitPiece || !IsRuntimePieceActor(HitPiece))
	{
		LogHitDebug(TEXT("RightClick rejected: hit is not a spawned Level4 piece"), HitResult, HitPiece);
		return;
	}

	if (SelectedPieceIndex != HitPiece->PieceIndex)
	{
		if (bEnableLevel4DebugLogs)
		{
			UE_LOG(LogTemp, Log, TEXT("[Level4][RightClick] Selecting hit piece. HitPiece=%s HitPieceIndex=%d PreviousSelected=%d"),
				*GetNameSafe(HitPiece),
				HitPiece->PieceIndex,
				SelectedPieceIndex);
		}
		SelectPiece(HitPiece->PieceIndex);
		return;
	}

	if (bEnableLevel4DebugLogs)
	{
		UE_LOG(LogTemp, Log, TEXT("[Level4][RightClick] Rotating selected piece. Piece=%s PieceIndex=%d"),
			*GetNameSafe(HitPiece),
			HitPiece->PieceIndex);
	}
	RotateSelectedPiece90();
}

bool ULevel4PuzzleComponent::FindSpawnedPieceHitOnRay(const FVector& RayStart, const FVector& RayEnd, FHitResult& OutHitResult) const
{
	bool bFoundHit = false;
	float BestDistance = TNumericLimits<float>::Max();
	FHitResult BestHitResult;
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

		if (UPrimitiveComponent* DragPrimitive = PieceActor->GetDragPrimitive())
		{
			FHitResult ComponentHitResult;
			if (DragPrimitive->LineTraceComponent(ComponentHitResult, RayStart, RayEnd, QueryParams))
			{
				const float HitDistance = FVector::Dist(RayStart, ComponentHitResult.ImpactPoint);
				if (HitDistance < BestDistance)
				{
					BestDistance = HitDistance;
					BestHitResult = ComponentHitResult;
					bFoundHit = true;
				}
				continue;
			}

			float BoundsDistance = 0.0f;
			const FBox BoundsBox = DragPrimitive->Bounds.GetBox();
			if (IntersectSegmentBox(RayStart, RayEnd, BoundsBox, BoundsDistance) && BoundsDistance < BestDistance)
			{
				const FVector RayDirection = (RayEnd - RayStart).GetSafeNormal();
				const FVector HitLocation = RayStart + RayDirection * BoundsDistance;
				BestDistance = BoundsDistance;
				BestHitResult = FHitResult(PieceActor, DragPrimitive, HitLocation, -RayDirection);
				BestHitResult.Distance = BoundsDistance;
				bFoundHit = true;
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

	if (bFoundHit)
	{
		OutHitResult = BestHitResult;
		if (bEnableLevel4DebugLogs)
		{
			ALevel4PuzzlePieceActor* HitPiece = ResolveHitPieceActor(OutHitResult);
			UE_LOG(LogTemp, Log, TEXT("[Level4][Hit] Piece-only trace selected spawned piece. Piece=%s PieceIndex=%d HitComponent=%s Distance=%f"),
				*GetNameSafe(HitPiece),
				HitPiece ? HitPiece->PieceIndex : INDEX_NONE,
				*GetNameSafe(OutHitResult.GetComponent()),
				BestDistance);
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
	DragPlaneOrigin = FVector::ZeroVector;

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
