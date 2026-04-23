#pragma once

#include "CoreMinimal.h"
#include "Level3Types.generated.h"

UENUM(BlueprintType)
enum class ELevel3Phase : uint8
{
	Introduction UMETA(DisplayName="Introduction"),
	ToolSelecting UMETA(DisplayName="ToolSelecting"),
	Repairing UMETA(DisplayName="Repairing"),
	RepairResult UMETA(DisplayName="RepairResult"),
	Completed UMETA(DisplayName="Completed")
};

UENUM(BlueprintType)
enum class ELevel3SubStage : uint8
{
	None UMETA(DisplayName="None"),
	Dusting UMETA(DisplayName="Dusting"),
	Oiling UMETA(DisplayName="Oiling")
};

USTRUCT(BlueprintType)
struct FLevel3ToolSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FName ToolId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float CleanlinessDelta = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float IntegrityDelta = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float AestheticsDelta = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	int32 ToolSlotIndex = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FLevel3StageConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	ELevel3SubStage SubStage = ELevel3SubStage::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FText StageTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FText ToolPanelTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FText ResultTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	int32 VisibleToolCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float RequiredMetricValue = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float InitialDustReveal01 = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float InitialDustConcentration01 = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float InitialOilBlend01 = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	bool bUsesDustVisual = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	bool bUsesOilVisual = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	TArray<FLevel3ToolSpec> Tools;
};

USTRUCT(BlueprintType)
struct FLevel3RepairAreaState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FName AreaId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float CoveragePercent = 0.0f;
};

USTRUCT(BlueprintType)
struct FLevel3ProgressState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	ELevel3Phase Phase = ELevel3Phase::Introduction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	ELevel3SubStage SubStage = ELevel3SubStage::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FName SelectedToolId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FName ActiveAreaId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float CleanlinessPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float IntegrityPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float AestheticsPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float RepairCoveragePercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	bool bIsRepairStrokeActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	bool bIsVisualTransitionActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float VisualTransitionDurationSeconds = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float VisualTransitionRemainingSeconds = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float DustReveal01 = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float DustConcentration01 = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float OilBlend01 = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FText StageTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FText ToolPanelTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FText ResultTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	int32 VisibleToolCount = 0;
};

USTRUCT(BlueprintType)
struct FLevel3ResultState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	ELevel3SubStage SubStage = ELevel3SubStage::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	bool bSucceeded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	int32 CulturalContributionValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FName SelectedToolId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float FinalCleanlinessPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float FinalIntegrityPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float FinalAestheticsPercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float FinalCoveragePercent = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float FinalDustReveal01 = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float FinalDustConcentration01 = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	float FinalOilBlend01 = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level3")
	FText ResultTitle;
};
