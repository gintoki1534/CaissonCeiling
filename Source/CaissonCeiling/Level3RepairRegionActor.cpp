#include "Level3RepairRegionActor.h"

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
