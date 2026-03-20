#include "CaissonPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "CaissonInteractComponent.h"
#include "CaissonPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/Application/SlateApplication.h"

ACaissonPlayerController::ACaissonPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// Level2 默认按 3 个目标推进，右键跳过默认关闭。
	CurrentStep = 0;
	TotalSteps = 3;
	bRightClickSkipEnabled = false;
	bEnableInteractionDebugLogs = true;
	bEnableLevelTargetHover = false;
	bEnableLevelTargetClick = false;
}

void ACaissonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	OnLevel2TargetProgressChanged.Broadcast(GetFoundLevel2TargetCount(), RequiredLevel2TargetIds.Num());
}

void ACaissonPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	UpdateHoveredInteractable();
}

void ACaissonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACaissonPlayerController::Look);
		}

		if (ClickAction)
		{
			// 鼠标左键点击用 Started 更稳，避免按下后被 UI 或拖拽状态吞掉。
			EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &ACaissonPlayerController::OnInteract);
		}

		if (RightClickAction)
		{
			EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &ACaissonPlayerController::OnRightClickSkip);
		}
	}
}

void ACaissonPlayerController::AdvanceStep()
{
	if (CurrentStep >= TotalSteps)
	{
		UE_LOG(LogTemp, Warning, TEXT("[C++ 步骤] 已到达最后一步，无法继续推进"));
		return;
	}

	++CurrentStep;
	UE_LOG(LogTemp, Log, TEXT("[C++ 步骤] 推进到第 %d 步（共 %d 步）"), CurrentStep, TotalSteps);
	OnStepChanged.Broadcast(CurrentStep);
}

void ACaissonPlayerController::ResetSteps()
{
	CurrentStep = 0;
	OnStepChanged.Broadcast(CurrentStep);
}

void ACaissonPlayerController::ResetLevel2TargetProgress()
{
	ActivatedLevel2TargetIds.Reset();

	if (CurrentHoveredInteractComponent)
	{
		CurrentHoveredInteractComponent->SetHoverHighlight(false);
		CurrentHoveredInteractComponent = nullptr;
	}

	OnLevel2TargetProgressChanged.Broadcast(GetFoundLevel2TargetCount(), RequiredLevel2TargetIds.Num());
}

int32 ACaissonPlayerController::GetFoundLevel2TargetCount() const
{
	return ActivatedLevel2TargetIds.Num();
}

void ACaissonPlayerController::SetLevelTargetInteractionEnabled(bool bHoverEnabled, bool bClickEnabled)
{
	bEnableLevelTargetHover = bHoverEnabled;
	bEnableLevelTargetClick = bClickEnabled;

	if (!bEnableLevelTargetHover && CurrentHoveredInteractComponent)
	{
		CurrentHoveredInteractComponent->SetHoverHighlight(false);
		CurrentHoveredInteractComponent = nullptr;
	}
}

bool ACaissonPlayerController::IsLevel2TargetActivated(FName TargetId) const
{
	return ActivatedLevel2TargetIds.Contains(TargetId);
}

void ACaissonPlayerController::OnRightClickSkip()
{
	if (!bRightClickSkipEnabled)
	{
		UE_LOG(LogTemp, Verbose, TEXT("[Level2] 右键调试跳过已关闭"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[Level2] 检测到右键调试跳过，尝试推进步骤"));
	AdvanceStep();
}

void ACaissonPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	APawn* MyPawn = GetPawn();
	if (!MyPawn)
	{
		return;
	}

	if (ACaissonPawn* CaissonPawn = Cast<ACaissonPawn>(MyPawn))
	{
		CaissonPawn->OnLookVectorReceived(LookAxisVector);
		return;
	}

	FRotator CurrentRotation = MyPawn->GetActorRotation();
	CurrentRotation.Yaw += LookAxisVector.X;
	CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch + LookAxisVector.Y, -80.0f, 80.0f);
	CurrentRotation.Roll = 0.0f;
	MyPawn->SetActorRotation(CurrentRotation);
}

void ACaissonPlayerController::OnInteract()
{
	if (!bEnableLevelTargetClick)
	{
		if (bEnableInteractionDebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[交互调试] 当前阶段未开启点击交互"));
		}
		return;
	}

	UCaissonInteractComponent* InteractComp = GetInteractComponentUnderCursor();
	if (!InteractComp || !InteractComp->GetOwner())
	{
		FHitResult RawHitResult;
		const bool bRawHit = GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, RawHitResult);
		if (bEnableInteractionDebugLogs)
		{
			if (!bRawHit)
			{
				UE_LOG(LogTemp, Warning, TEXT("[交互调试] 点击时没有命中任何对象"));
			}
			else if (AActor* RawHitActor = RawHitResult.GetActor())
			{
				UE_LOG(LogTemp, Warning, TEXT("[交互调试] 点击命中了对象 %s，但它没有可交互组件"), *RawHitActor->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[交互调试] 点击命中了碰撞，但没有拿到 Actor"));
			}
		}
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[交互] 射线命中对象：%s"), *InteractComp->GetOwner()->GetName());

	if (InteractComp->TryInteract())
	{
		HandleLevel2Interaction(InteractComp);
	}
}

void ACaissonPlayerController::HandleLevel2Interaction(UCaissonInteractComponent* InteractComp)
{
	if (!InteractComp)
	{
		return;
	}

	const FName TargetId = InteractComp->InteractionId;
	if (TargetId.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Level2] 可交互对象未配置 InteractionId：%s"), *GetNameSafe(InteractComp->GetOwner()));
		return;
	}

	if (!RequiredLevel2TargetIds.Contains(TargetId))
	{
		if (bEnableInteractionDebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[交互调试] 对象 %s 存在交互组件，但 InteractionId=%s 不在当前目标列表中"), *GetNameSafe(InteractComp->GetOwner()), *TargetId.ToString());
		}
		UE_LOG(LogTemp, Verbose, TEXT("[Level2] 点击目标 %s，但它不属于当前关卡目标"), *TargetId.ToString());
		return;
	}

	if (ActivatedLevel2TargetIds.Contains(TargetId))
	{
		UE_LOG(LogTemp, Verbose, TEXT("[Level2] 目标 %s 已经完成，本次不重复计数"), *TargetId.ToString());
		return;
	}

	// 记录本次激活的目标，并立即锁定，防止重复触发。
	ActivatedLevel2TargetIds.Add(TargetId);
	InteractComp->SetInteractionEnabled(false);
	InteractComp->SetPersistentHighlight(true);
	InteractComp->SetHoverHighlight(false);

	const int32 FoundCount = GetFoundLevel2TargetCount();
	const int32 TargetCount = RequiredLevel2TargetIds.Num();

	UE_LOG(LogTemp, Log, TEXT("[Level2] 成功点亮目标 %s（%d/%d）"), *TargetId.ToString(), FoundCount, TargetCount);

	OnLevel2TargetActivated.Broadcast(TargetId);
	OnLevel2TargetProgressChanged.Broadcast(FoundCount, TargetCount);

	// 目前每找到一个关键目标就推进一步，3 个目标全部完成后正好推进完整个阶段。
	AdvanceStep();
}

void ACaissonPlayerController::UpdateHoveredInteractable()
{
	if (!bEnableLevelTargetHover)
	{
		if (CurrentHoveredInteractComponent)
		{
			CurrentHoveredInteractComponent->SetHoverHighlight(false);
			CurrentHoveredInteractComponent = nullptr;
		}
		return;
	}

	UCaissonInteractComponent* NewHoveredInteractComp = GetInteractComponentUnderCursor();
	if (CurrentHoveredInteractComponent == NewHoveredInteractComp)
	{
		return;
	}

	if (CurrentHoveredInteractComponent)
	{
		if (bEnableInteractionDebugLogs && CurrentHoveredInteractComponent->GetOwner())
		{
			UE_LOG(LogTemp, Log, TEXT("[交互调试] 鼠标离开对象：%s"), *CurrentHoveredInteractComponent->GetOwner()->GetName());
		}
		CurrentHoveredInteractComponent->SetHoverHighlight(false);
	}

	CurrentHoveredInteractComponent = NewHoveredInteractComp;

	if (CurrentHoveredInteractComponent)
	{
		if (bEnableInteractionDebugLogs && CurrentHoveredInteractComponent->GetOwner())
		{
			UE_LOG(LogTemp, Log, TEXT("[交互调试] 鼠标进入对象：%s"), *CurrentHoveredInteractComponent->GetOwner()->GetName());
		}
		CurrentHoveredInteractComponent->SetHoverHighlight(true);
	}
}

UCaissonInteractComponent* ACaissonPlayerController::GetInteractComponentUnderCursor() const
{
	FHitResult HitResult;
	const bool bHit = GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult);
	if (!bHit)
	{
		return nullptr;
	}

	AActor* HitActor = HitResult.GetActor();
	if (!HitActor)
	{
		return nullptr;
	}

	UCaissonInteractComponent* InteractComp = HitActor->FindComponentByClass<UCaissonInteractComponent>();
	if (!InteractComp || !InteractComp->bInteractionEnabled)
	{
		return nullptr;
	}

	return InteractComp;
}

UUserWidget* ACaissonPlayerController::OpenCaissonWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if (!WidgetClass)
	{
		return nullptr;
	}

	UUserWidget* NewWidget = CreateWidget<UUserWidget>(this, WidgetClass);
	if (NewWidget)
	{
		NewWidget->AddToViewport();

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(NewWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);

		bShowMouseCursor = true;
	}

	return NewWidget;
}

void ACaissonPlayerController::CloseCaissonWidget(UUserWidget* WidgetToClose)
{
	if (WidgetToClose)
	{
		WidgetToClose->RemoveFromParent();
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	FSlateApplication::Get().SetAllUserFocusToGameViewport();
}
