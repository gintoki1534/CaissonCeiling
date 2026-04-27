#include "Level4PuzzlePieceActor.h"

#include "Components/StaticMeshComponent.h"

ALevel4PuzzlePieceActor::ALevel4PuzzlePieceActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SceneRoot);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComponent->SetGenerateOverlapEvents(false);
}

void ALevel4PuzzlePieceActor::BeginPlay()
{
	Super::BeginPlay();
	SetSelectedVisual(bIsSelected);
	SetCorrectVisual(bIsCorrect);
}

void ALevel4PuzzlePieceActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALevel4PuzzlePieceActor::InitializeLevel4Piece(int32 InPieceIndex)
{
	PieceIndex = InPieceIndex;
	bIsSpawnedInField = true;
}

void ALevel4PuzzlePieceActor::SetSelectedVisual(bool bNewSelected)
{
	bIsSelected = bNewSelected;

	if (bUseCustomDepthForSelection && MeshComponent)
	{
		MeshComponent->SetRenderCustomDepth(bIsSelected || bIsCorrect);
		if (bIsSelected)
		{
			MeshComponent->SetCustomDepthStencilValue(SelectedCustomDepthStencil);
		}
		else if (bIsCorrect)
		{
			MeshComponent->SetCustomDepthStencilValue(CorrectCustomDepthStencil);
		}
	}

	BP_OnSelectedVisualChanged(bIsSelected);
}

void ALevel4PuzzlePieceActor::SetCorrectVisual(bool bNewCorrect)
{
	bIsCorrect = bNewCorrect;

	if (bUseCustomDepthForSelection && MeshComponent)
	{
		MeshComponent->SetRenderCustomDepth(bIsSelected || bIsCorrect);
		if (!bIsSelected && bIsCorrect)
		{
			MeshComponent->SetCustomDepthStencilValue(CorrectCustomDepthStencil);
		}
	}

	BP_OnCorrectVisualChanged(bIsCorrect);
}

UPrimitiveComponent* ALevel4PuzzlePieceActor::GetDragPrimitive() const
{
	return MeshComponent;
}
