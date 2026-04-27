#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Level4Types.h"
#include "Level4PuzzleTargetLayoutActor.generated.h"

UCLASS()
class CAISSONCEILING_API ALevel4PuzzleTargetLayoutActor : public AActor
{
	GENERATED_BODY()

public:
	ALevel4PuzzleTargetLayoutActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4")
	FLevel4PuzzleStageConfig StageConfig;
};
