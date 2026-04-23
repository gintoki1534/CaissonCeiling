#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Level3Types.h"
#include "CaissonPlayerController.generated.h"

class UCaissonInteractComponent;
class UInputAction;
class UInputMappingContext;
class ULevel3FlowComponent;
class UUserWidget;

UENUM(BlueprintType)
enum class ELevel2FlowState : uint8
{
	SearchingTargets UMETA(DisplayName="SearchingTargets"),
	ShowingTargetInspect UMETA(DisplayName="ShowingTargetInspect"),
	WaitingAnyClickToContinue UMETA(DisplayName="WaitingAnyClickToContinue"),
	TransitionRequested UMETA(DisplayName="TransitionRequested")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStepChanged, int32, NewStep);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevel2TargetProgressChanged, int32, FoundCount, int32, TargetCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevel2TargetActivated, FName, TargetId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevel2InspectStarted, FName, TargetId, bool, bIsFinalTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevel2FinalContinuePromptRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevel2NextLevelRequested);

UCLASS()
class CAISSONCEILING_API ACaissonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACaissonPlayerController();

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Context")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* ClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* RightClickAction;

	void Look(const FInputActionValue& Value);
	void OnPrimaryInteractPressed();
	void OnPrimaryInteractReleased();
	void OnRightMousePressed();
	void OnRightMouseReleased();
	bool HandleLevel2Interaction(UCaissonInteractComponent* InteractComp);
	bool GetCursorHitResult(FHitResult& OutHitResult) const;
	void UpdateHoveredInteractable();
	UCaissonInteractComponent* GetInteractComponentUnderCursor(FHitResult* OutHitResult = nullptr) const;

public:
	UPROPERTY(BlueprintReadOnly, Category="CaissonStep")
	int32 CurrentStep;

	// 当前阶段总步骤数，通常与目标数保持一致。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CaissonStep")
	int32 TotalSteps;

	UPROPERTY(BlueprintAssignable, Category="CaissonStep")
	FOnStepChanged OnStepChanged;

	// Level2 当前要求玩家找到的目标 ID 列表。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level2|Targets")
	TArray<FName> RequiredLevel2TargetIds;

	// 兼容旧蓝图配置保留的历史参数；当前右键不再用于调试跳步。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level2|Targets")
	bool bRightClickSkipEnabled;

	// 开启后，点击时会输出更详细的交互调试日志。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool bEnableInteractionDebugLogs;

	// 是否允许当前阶段进行目标悬停高亮。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level2|Targets")
	bool bEnableLevelTargetHover;

	// 是否允许当前阶段进行目标点击交互。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level2|Targets")
	bool bEnableLevelTargetClick;

	UPROPERTY(BlueprintAssignable, Category="Level2|Targets")
	FOnLevel2TargetProgressChanged OnLevel2TargetProgressChanged;

	UPROPERTY(BlueprintAssignable, Category="Level2|Targets")
	FOnLevel2TargetActivated OnLevel2TargetActivated;

	// 正确点击目标后触发：蓝图据此执行镜头拉近和介绍 UI 展示。
	UPROPERTY(BlueprintAssignable, Category="Level2|Flow")
	FOnLevel2InspectStarted OnLevel2InspectStarted;

	// 第三个目标流程结束后触发：蓝图显示“点击任何地方继续”提示。
	UPROPERTY(BlueprintAssignable, Category="Level2|Flow")
	FOnLevel2FinalContinuePromptRequested OnLevel2FinalContinuePromptRequested;

	// 收到“任意点击继续”后触发：蓝图在这里执行关卡跳转（例如打开 Level3）。
	UPROPERTY(BlueprintAssignable, Category="Level2|Flow")
	FOnLevel2NextLevelRequested OnLevel2NextLevelRequested;

	UPROPERTY(BlueprintReadOnly, Category="Level2|Flow")
	ELevel2FlowState Level2FlowState;

	UFUNCTION(BlueprintCallable, Category="CaissonStep")
	void AdvanceStep();

	UFUNCTION(BlueprintCallable, Category="CaissonStep")
	void ResetSteps();

	UFUNCTION(BlueprintCallable, Category="Level2|Targets")
	void ResetLevel2TargetProgress();

	UFUNCTION(BlueprintCallable, Category="Level2|Targets")
	void SetLevelTargetInteractionEnabled(bool bHoverEnabled, bool bClickEnabled);

	UFUNCTION(BlueprintPure, Category="Level2|Targets")
	int32 GetFoundLevel2TargetCount() const;

	UFUNCTION(BlueprintPure, Category="Level2|Targets")
	bool IsLevel2TargetActivated(FName TargetId) const;

	// 蓝图在“介绍 UI 已点击继续 + 镜头已回原位”后调用，结束当前目标演出流程。
	UFUNCTION(BlueprintCallable, Category="Level2|Flow")
	void CompleteLevel2InspectPresentation();

	UFUNCTION(BlueprintPure, Category="Level2|Flow")
	bool IsLevel2WaitingForAnyClickToContinue() const;

	UFUNCTION(BlueprintCallable, Category="CaissonUI")
	UUserWidget* OpenCaissonWidget(TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category="CaissonUI")
	void CloseCaissonWidget(UUserWidget* WidgetToClose);

	UFUNCTION(BlueprintCallable, Category="Level3")
	void StartLevel3Dusting();

	UFUNCTION(BlueprintCallable, Category="Level3")
	void StartLevel3Oiling();

	UFUNCTION(BlueprintCallable, Category="Level3")
	void AdvanceLevel3ToOiling();

	UFUNCTION(BlueprintCallable, Category="Level3")
	void ResetLevel3Dusting();

	UFUNCTION(BlueprintCallable, Category="Level3")
	bool SelectLevel3Tool(FName ToolId);

	UFUNCTION(BlueprintCallable, Category="Level3")
	void ContinueLevel3AfterResult();

	UFUNCTION(BlueprintCallable, Category="Level3")
	void CompleteLevel3ResultPresentation();

	UFUNCTION(BlueprintPure, Category="Level3")
	ELevel3SubStage GetCurrentLevel3SubStage() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	ULevel3FlowComponent* GetLevel3FlowComponent() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	bool IsLevel3VisualTransitionActive() const;

	UFUNCTION(BlueprintPure, Category="Level3")
	float GetLevel3VisualTransitionRemainingSeconds() const;

private:
	UPROPERTY(Transient)
	TSet<FName> ActivatedLevel2TargetIds;

	UPROPERTY(Transient)
	TObjectPtr<UCaissonInteractComponent> CurrentHoveredInteractComponent;

	UPROPERTY(Transient)
	FName ActiveInspectTargetId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level3", meta=(AllowPrivateAccess="true"))
	TObjectPtr<ULevel3FlowComponent> Level3FlowComponent;

	bool bRightMouseLookHeld;
	bool bCachedHoverEnabledBeforeInspect;
	bool bCachedClickEnabledBeforeInspect;
};
