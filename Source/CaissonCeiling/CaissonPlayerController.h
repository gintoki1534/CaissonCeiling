#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CaissonPlayerController.generated.h"

class UCaissonInteractComponent;
class UInputAction;
class UInputMappingContext;
class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStepChanged, int32, NewStep);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevel2TargetProgressChanged, int32, FoundCount, int32, TargetCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevel2TargetActivated, FName, TargetId);

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
	void OnInteract();
	void OnRightClickSkip();
	void HandleLevel2Interaction(UCaissonInteractComponent* InteractComp);
	void UpdateHoveredInteractable();
	UCaissonInteractComponent* GetInteractComponentUnderCursor() const;

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

	// 调试开关：是否允许右键直接跳过步骤。
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

	UFUNCTION(BlueprintCallable, Category="CaissonUI")
	UUserWidget* OpenCaissonWidget(TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category="CaissonUI")
	void CloseCaissonWidget(UUserWidget* WidgetToClose);

private:
	UPROPERTY(Transient)
	TSet<FName> ActivatedLevel2TargetIds;

	UPROPERTY(Transient)
	TObjectPtr<UCaissonInteractComponent> CurrentHoveredInteractComponent;
};
