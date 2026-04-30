#pragma once

#include "CoreMinimal.h"
#include "Level4Types.generated.h"

class ALevel4PuzzlePieceActor;

UENUM(BlueprintType)
enum class ELevel4Difficulty : uint8
{
	Normal UMETA(DisplayName="Normal"),
	Expert UMETA(DisplayName="Expert")
};

UENUM(BlueprintType)
enum class ELevel4StageId : uint8
{
	None UMETA(DisplayName="None"),
	CloudFrame1 UMETA(DisplayName="CloudFrame1"),
	CloudFrame2 UMETA(DisplayName="CloudFrame2"),
	StarMap UMETA(DisplayName="StarMap"),
	FinalAssembly UMETA(DisplayName="FinalAssembly")
};

UENUM(BlueprintType)
enum class ELevel4LockedAxis : uint8
{
	X UMETA(DisplayName="X"),
	Y UMETA(DisplayName="Y"),
	Z UMETA(DisplayName="Z")
};

UENUM(BlueprintType)
enum class ELevel4PuzzlePhase : uint8
{
	Inactive UMETA(DisplayName="Inactive"),
	Playing UMETA(DisplayName="Playing"),
	StageCompleted UMETA(DisplayName="StageCompleted"),
	Completed UMETA(DisplayName="Completed")
};

USTRUCT(BlueprintType)
struct FLevel4PuzzlePieceConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4")
	int32 PieceIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4")
	TSubclassOf<ALevel4PuzzlePieceActor> PieceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4")
	FTransform TargetTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Spawn")
	bool bUseCustomNormalSpawnTransform = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Spawn")
	FTransform NormalSpawnTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Spawn")
	bool bUseCustomExpertSpawnTransform = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Spawn")
	FTransform ExpertSpawnTransform = FTransform::Identity;
};

USTRUCT(BlueprintType)
struct FLevel4PuzzleStageConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4")
	ELevel4StageId StageId = ELevel4StageId::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4")
	ELevel4LockedAxis LockedAxis = ELevel4LockedAxis::Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4")
	TArray<FLevel4PuzzlePieceConfig> Pieces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Expert")
	FVector ExpertRandomMin = FVector(-250.0, 0.0, -250.0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Expert")
	FVector ExpertRandomMax = FVector(250.0, 0.0, 250.0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Check")
	float PositionTolerance = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level4|Check")
	float RotationToleranceDegrees = 5.0f;
};
