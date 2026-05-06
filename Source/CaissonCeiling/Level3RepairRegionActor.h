#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level3RepairRegionActor.generated.h"

class USceneComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level3")
	TObjectPtr<USceneComponent> SceneRootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level3")
	TObjectPtr<UStaticMeshComponent> PreviewMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level3")
	TObjectPtr<ULevel3RepairAreaComponent> RepairAreaComp;

private:
	void ConfigureRepairMeshCollision();
};
