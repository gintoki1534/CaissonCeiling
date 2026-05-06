#include "Level3RepairRegionActor.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Level3RepairAreaComponent.h"

ALevel3RepairRegionActor::ALevel3RepairRegionActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComp"));
	SetRootComponent(SceneRootComp);

	PreviewMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMeshComp"));
	PreviewMeshComp->SetupAttachment(SceneRootComp);
	PreviewMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PreviewMeshComp->SetCollisionResponseToAllChannels(ECR_Block);

	RepairAreaComp = CreateDefaultSubobject<ULevel3RepairAreaComponent>(TEXT("RepairAreaComp"));
}

void ALevel3RepairRegionActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ConfigureRepairMeshCollision();
}

void ALevel3RepairRegionActor::BeginPlay()
{
	Super::BeginPlay();
	ConfigureRepairMeshCollision();
}

void ALevel3RepairRegionActor::ConfigureRepairMeshCollision()
{
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);

	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		if (!PrimitiveComponent)
		{
			continue;
		}

		const bool bIsRepairMesh = PrimitiveComponent->GetFName() == TEXT("SM_RepairMesh")
			|| PrimitiveComponent == PreviewMeshComp;
		if (!bIsRepairMesh)
		{
			continue;
		}

		PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PrimitiveComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		PrimitiveComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}
}
