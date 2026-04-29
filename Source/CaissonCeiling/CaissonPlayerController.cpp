#include "CaissonPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "CaissonInteractComponent.h"
#include "CaissonPawn.h"
#include "Level3FlowComponent.h"
#include "Level4PuzzleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/Application/SlateApplication.h"

namespace
{
UCaissonInteractComponent* FindInteractComponentOnActorHierarchy(AActor* StartActor)
{
	TSet<AActor*> VisitedActors;
	TArray<AActor*> PendingActors;

	if (StartActor)
	{
		PendingActors.Add(StartActor);
	}

	while (PendingActors.Num() > 0)
	{
		AActor* CurrentActor = PendingActors.Pop(EAllowShrinking::No);
		if (!CurrentActor || VisitedActors.Contains(CurrentActor))
		{
			continue;
		}

		VisitedActors.Add(CurrentActor);

		if (UCaissonInteractComponent* InteractComp = CurrentActor->FindComponentByClass<UCaissonInteractComponent>())
		{
			if (InteractComp->bInteractionEnabled)
			{
				return InteractComp;
			}
		}

		if (AActor* OwnerActor = CurrentActor->GetOwner())
		{
			PendingActors.Add(OwnerActor);
		}

		if (AActor* AttachParentActor = CurrentActor->GetAttachParentActor())
		{
			PendingActors.Add(AttachParentActor);
		}

		if (AActor* ParentActor = CurrentActor->GetParentActor())
		{
			PendingActors.Add(ParentActor);
		}
	}

	return nullptr;
}

void ClearHoverTracking(TObjectPtr<UCaissonInteractComponent>& HoveredInteractComponent, bool bClearPersistentTarget)
{
	if (!HoveredInteractComponent)
	{
		return;
	}

	HoveredInteractComponent->SetHoverHighlight(false);

	if (bClearPersistentTarget || !HoveredInteractComponent->IsPersistentHighlightEnabled())
	{
		HoveredInteractComponent->ClearHighlightTarget();
	}

	HoveredInteractComponent = nullptr;
}
}

ACaissonPlayerController::ACaissonPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// Level2 默认按 3 个目标推进；当前仅允许按住右键时旋转模型。
	CurrentStep = 0;
	TotalSteps = 3;
	bRightClickSkipEnabled = false;
	bEnableInteractionDebugLogs = true;
	bEnableLevelTargetHover = false;
	bEnableLevelTargetClick = false;
	Level2FlowState = ELevel2FlowState::SearchingTargets;
	ActiveInspectTargetId = NAME_None;
	bRightMouseLookHeld = false;
	bCachedHoverEnabledBeforeInspect = false;
	bCachedClickEnabledBeforeInspect = false;
	Level3FlowComponent = CreateDefaultSubobject<ULevel3FlowComponent>(TEXT("Level3FlowComponent"));
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
			EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Started, this, &ACaissonPlayerController::OnPrimaryInteractPressed);
			EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Completed, this, &ACaissonPlayerController::OnPrimaryInteractReleased);
			EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Canceled, this, &ACaissonPlayerController::OnPrimaryInteractReleased);
		}

		if (RightClickAction)
		{
			EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &ACaissonPlayerController::OnRightMousePressed);
			EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Completed, this, &ACaissonPlayerController::OnRightMouseReleased);
			EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Canceled, this, &ACaissonPlayerController::OnRightMouseReleased);
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
	Level2FlowState = ELevel2FlowState::SearchingTargets;
	ActiveInspectTargetId = NAME_None;
	SetIgnoreLookInput(false);
	OnStepChanged.Broadcast(CurrentStep);
}

void ACaissonPlayerController::ResetLevel2TargetProgress()
{
	ActivatedLevel2TargetIds.Reset();
	Level2FlowState = ELevel2FlowState::SearchingTargets;
	ActiveInspectTargetId = NAME_None;
	SetIgnoreLookInput(false);

	if (CurrentHoveredInteractComponent)
	{
		ClearHoverTracking(CurrentHoveredInteractComponent, true);
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
		ClearHoverTracking(CurrentHoveredInteractComponent, false);
	}
}

bool ACaissonPlayerController::IsLevel2TargetActivated(FName TargetId) const
{
	return ActivatedLevel2TargetIds.Contains(TargetId);
}

void ACaissonPlayerController::OnRightMousePressed()
{
	if (ULevel4PuzzleComponent* Level4PuzzleComponent = GetLevel4PuzzleComponent())
	{
		if (Level4PuzzleComponent->IsLevel4SessionActive())
		{
			FHitResult Level4HitResult;
			if (GetCursorHitResult(Level4HitResult))
			{
				Level4PuzzleComponent->HandleRightClickPiece(Level4HitResult);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[Level4][RightClick] Cursor hit test returned false."));
			}
			return;
		}
	}

	bRightMouseLookHeld = true;
}

void ACaissonPlayerController::OnRightMouseReleased()
{
	bRightMouseLookHeld = false;
}

void ACaissonPlayerController::Look(const FInputActionValue& Value)
{
	if (ULevel4PuzzleComponent* Level4PuzzleComponent = GetLevel4PuzzleComponent())
	{
		if (Level4PuzzleComponent->IsLevel4SessionActive())
		{
			return;
		}
	}

	if (!bRightMouseLookHeld)
	{
		return;
	}

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

void ACaissonPlayerController::OnPrimaryInteractPressed()
{
	if (ULevel4PuzzleComponent* Level4PuzzleComponent = GetLevel4PuzzleComponent())
	{
		if (Level4PuzzleComponent->IsLevel4SessionActive())
		{
			FHitResult Level4HitResult;
			if (GetCursorHitResult(Level4HitResult))
			{
				Level4PuzzleComponent->BeginDragSelectedPiece(Level4HitResult, this);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[Level4][BeginDrag] Cursor hit test returned false."));
			}
			return;
		}
	}

	if (Level3FlowComponent && Level3FlowComponent->IsLevel3SessionActive())
	{
		FHitResult Level3HitResult;
		if (GetCursorHitResult(Level3HitResult))
		{
			Level3FlowComponent->ApplySelectedToolToHit(Level3HitResult);
		}
		return;
	}

	if (Level2FlowState == ELevel2FlowState::WaitingAnyClickToContinue)
	{
		Level2FlowState = ELevel2FlowState::TransitionRequested;
		UE_LOG(LogTemp, Log, TEXT("[Level2] 检测到“任意点击继续”，请求进入下一关"));
		OnLevel2NextLevelRequested.Broadcast();
		return;
	}

	if (Level2FlowState == ELevel2FlowState::ShowingTargetInspect || Level2FlowState == ELevel2FlowState::TransitionRequested)
	{
		if (bEnableInteractionDebugLogs)
		{
			UE_LOG(LogTemp, Verbose, TEXT("[交互调试] 当前处于演出或跳转状态，忽略点击"));
		}
		return;
	}

	if (!bEnableLevelTargetClick)
	{
		if (bEnableInteractionDebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[交互调试] 当前阶段未开启点击交互"));
		}
		return;
	}

	FHitResult HitResult;
	UCaissonInteractComponent* InteractComp = GetInteractComponentUnderCursor(&HitResult);
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

	InteractComp->SetHighlightTarget(HitResult.GetComponent(), HitResult.GetActor());
	UE_LOG(LogTemp, Log, TEXT("[交互] 射线命中对象：%s"), *InteractComp->GetOwner()->GetName());

	HandleLevel2Interaction(InteractComp);
}

void ACaissonPlayerController::OnPrimaryInteractReleased()
{
	if (ULevel4PuzzleComponent* Level4PuzzleComponent = GetLevel4PuzzleComponent())
	{
		if (Level4PuzzleComponent->IsLevel4SessionActive())
		{
			Level4PuzzleComponent->EndDragSelectedPiece();
			return;
		}
	}

	// Level3 当前改为“单击结算”，释放左键时无需额外处理。
}

bool ACaissonPlayerController::HandleLevel2Interaction(UCaissonInteractComponent* InteractComp)
{
	if (!InteractComp)
	{
		return false;
	}

	const FName TargetId = InteractComp->InteractionId;
	if (TargetId.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Level2] 可交互对象未配置 InteractionId：%s"), *GetNameSafe(InteractComp->GetOwner()));
		return false;
	}

	if (!RequiredLevel2TargetIds.Contains(TargetId))
	{
		if (bEnableInteractionDebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[交互调试] 对象 %s 存在交互组件，但 InteractionId=%s 不在当前目标列表中"), *GetNameSafe(InteractComp->GetOwner()), *TargetId.ToString());
		}
		UE_LOG(LogTemp, Verbose, TEXT("[Level2] 点击目标 %s，但它不属于当前关卡目标"), *TargetId.ToString());
		return false;
	}

	const int32 TargetCount = RequiredLevel2TargetIds.Num();
	if (TargetCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Level2] 当前未配置 RequiredLevel2TargetIds，无法进行顺序判定"));
		return false;
	}

	// 按顺序判定：只能点击当前步骤对应的目标。
	if (CurrentStep < 0 || CurrentStep >= TargetCount)
	{
		if (bEnableInteractionDebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[交互调试] 当前步骤索引越界（CurrentStep=%d，TargetCount=%d），忽略本次点击"), CurrentStep, TargetCount);
		}
		return false;
	}

	const FName ExpectedTargetId = RequiredLevel2TargetIds[CurrentStep];
	if (TargetId != ExpectedTargetId)
	{
		if (bEnableInteractionDebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[交互调试] 点击顺序不正确：当前应点击 %s，实际点击 %s"), *ExpectedTargetId.ToString(), *TargetId.ToString());
		}
		return false;
	}

	if (ActivatedLevel2TargetIds.Contains(TargetId))
	{
		UE_LOG(LogTemp, Verbose, TEXT("[Level2] 目标 %s 已经完成，本次不重复计数"), *TargetId.ToString());
		return false;
	}

	// 只有顺序正确时才触发点击事件，避免“点错也点亮”。
	if (!InteractComp->TryInteract())
	{
		if (bEnableInteractionDebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[交互调试] 对象 %s 当前不可交互，忽略本次激活"), *GetNameSafe(InteractComp->GetOwner()));
		}
		return false;
	}

	// 记录本次激活的目标，并立即锁定，防止重复触发。
	ActivatedLevel2TargetIds.Add(TargetId);
	InteractComp->SetInteractionEnabled(false);
	InteractComp->SetPersistentHighlight(true);
	InteractComp->SetHoverHighlight(false);

	const int32 FoundCount = GetFoundLevel2TargetCount();

	UE_LOG(LogTemp, Log, TEXT("[Level2] 成功点亮目标 %s（%d/%d）"), *TargetId.ToString(), FoundCount, TargetCount);

	OnLevel2TargetActivated.Broadcast(TargetId);
	OnLevel2TargetProgressChanged.Broadcast(FoundCount, TargetCount);

	// 目前每找到一个关键目标就推进一步，3 个目标全部完成后正好推进完整个阶段。
	AdvanceStep();

	// 点击成功后进入“目标演出中”状态，蓝图执行镜头拉近 + 介绍 UI。
	Level2FlowState = ELevel2FlowState::ShowingTargetInspect;
	ActiveInspectTargetId = TargetId;
	bCachedHoverEnabledBeforeInspect = bEnableLevelTargetHover;
	bCachedClickEnabledBeforeInspect = bEnableLevelTargetClick;
	SetLevelTargetInteractionEnabled(false, false);
	SetIgnoreLookInput(true);

	const bool bIsFinalTarget = (FoundCount >= TargetCount);
	OnLevel2InspectStarted.Broadcast(TargetId, bIsFinalTarget);

	return true;
}

void ACaissonPlayerController::CompleteLevel2InspectPresentation()
{
	if (Level2FlowState != ELevel2FlowState::ShowingTargetInspect)
	{
		if (bEnableInteractionDebugLogs)
		{
			UE_LOG(LogTemp, Warning, TEXT("[交互调试] 调用了 CompleteLevel2InspectPresentation，但当前不在演出状态"));
		}
		return;
	}

	const int32 FoundCount = GetFoundLevel2TargetCount();
	const int32 TargetCount = RequiredLevel2TargetIds.Num();

	ActiveInspectTargetId = NAME_None;

	if (TargetCount > 0 && FoundCount >= TargetCount)
	{
		Level2FlowState = ELevel2FlowState::WaitingAnyClickToContinue;
		UE_LOG(LogTemp, Log, TEXT("[Level2] 三个目标演出完成，等待任意点击继续"));
		OnLevel2FinalContinuePromptRequested.Broadcast();
		return;
	}

	Level2FlowState = ELevel2FlowState::SearchingTargets;
	SetLevelTargetInteractionEnabled(bCachedHoverEnabledBeforeInspect, bCachedClickEnabledBeforeInspect);
	SetIgnoreLookInput(false);
}

bool ACaissonPlayerController::IsLevel2WaitingForAnyClickToContinue() const
{
	return Level2FlowState == ELevel2FlowState::WaitingAnyClickToContinue;
}

void ACaissonPlayerController::UpdateHoveredInteractable()
{
	if (Level3FlowComponent && Level3FlowComponent->IsLevel3SessionActive())
	{
		if (CurrentHoveredInteractComponent)
		{
			ClearHoverTracking(CurrentHoveredInteractComponent, false);
		}
		return;
	}

	if (!bEnableLevelTargetHover)
	{
		if (CurrentHoveredInteractComponent)
		{
			ClearHoverTracking(CurrentHoveredInteractComponent, false);
		}
		return;
	}

	FHitResult HitResult;
	UCaissonInteractComponent* NewHoveredInteractComp = GetInteractComponentUnderCursor(&HitResult);
	if (CurrentHoveredInteractComponent == NewHoveredInteractComp)
	{
		if (CurrentHoveredInteractComponent)
		{
			CurrentHoveredInteractComponent->SetHighlightTarget(HitResult.GetComponent(), HitResult.GetActor());
		}
		return;
	}

	if (CurrentHoveredInteractComponent)
	{
		if (bEnableInteractionDebugLogs && CurrentHoveredInteractComponent->GetOwner())
		{
			UE_LOG(LogTemp, Log, TEXT("[交互调试] 鼠标离开对象：%s"), *CurrentHoveredInteractComponent->GetOwner()->GetName());
		}
		CurrentHoveredInteractComponent->SetHoverHighlight(false);
		if (!CurrentHoveredInteractComponent->IsPersistentHighlightEnabled())
		{
			CurrentHoveredInteractComponent->ClearHighlightTarget();
		}
	}

	CurrentHoveredInteractComponent = NewHoveredInteractComp;

	if (CurrentHoveredInteractComponent)
	{
		CurrentHoveredInteractComponent->SetHighlightTarget(HitResult.GetComponent(), HitResult.GetActor());
		if (bEnableInteractionDebugLogs && CurrentHoveredInteractComponent->GetOwner())
		{
			UE_LOG(LogTemp, Log, TEXT("[交互调试] 鼠标进入对象：%s"), *CurrentHoveredInteractComponent->GetOwner()->GetName());
		}
		CurrentHoveredInteractComponent->SetHoverHighlight(true);
	}
}

bool ACaissonPlayerController::GetCursorHitResult(FHitResult& OutHitResult) const
{
	return GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, OutHitResult);
}

UCaissonInteractComponent* ACaissonPlayerController::GetInteractComponentUnderCursor(FHitResult* OutHitResult) const
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

	if (OutHitResult)
	{
		*OutHitResult = HitResult;
	}

	return FindInteractComponentOnActorHierarchy(HitActor);
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

void ACaissonPlayerController::StartLevel3Dusting()
{
	if (Level3FlowComponent)
	{
		Level3FlowComponent->StartLevel3Dusting();
	}
}

void ACaissonPlayerController::StartLevel3Oiling()
{
	if (Level3FlowComponent)
	{
		Level3FlowComponent->StartLevel3Oiling();
	}
}

void ACaissonPlayerController::AdvanceLevel3ToOiling()
{
	if (Level3FlowComponent)
	{
		Level3FlowComponent->AdvanceToOilingStage();
	}
}

void ACaissonPlayerController::ResetLevel3Dusting()
{
	if (Level3FlowComponent)
	{
		Level3FlowComponent->ResetLevel3State();
	}
}

bool ACaissonPlayerController::SelectLevel3Tool(FName ToolId)
{
	return Level3FlowComponent ? Level3FlowComponent->SelectTool(ToolId) : false;
}

void ACaissonPlayerController::ContinueLevel3AfterResult()
{
	if (Level3FlowComponent)
	{
		Level3FlowComponent->CompleteResultPresentation();
	}
}

void ACaissonPlayerController::CompleteLevel3ResultPresentation()
{
	if (Level3FlowComponent)
	{
		Level3FlowComponent->CompleteResultPresentation();
	}
}

ELevel3SubStage ACaissonPlayerController::GetCurrentLevel3SubStage() const
{
	return Level3FlowComponent ? Level3FlowComponent->GetCurrentSubStage() : ELevel3SubStage::None;
}

ULevel3FlowComponent* ACaissonPlayerController::GetLevel3FlowComponent() const
{
	return Level3FlowComponent;
}

bool ACaissonPlayerController::IsLevel3VisualTransitionActive() const
{
	return Level3FlowComponent ? Level3FlowComponent->IsVisualTransitionActive() : false;
}

float ACaissonPlayerController::GetLevel3VisualTransitionRemainingSeconds() const
{
	return Level3FlowComponent ? Level3FlowComponent->GetVisualTransitionRemainingSeconds() : 0.0f;
}

void ACaissonPlayerController::StartLevel4Puzzle(ELevel4Difficulty Difficulty)
{
	if (ULevel4PuzzleComponent* Level4PuzzleComponent = GetLevel4PuzzleComponent())
	{
		Level4PuzzleComponent->StartLevel4Puzzle(Difficulty);
	}
}

void ACaissonPlayerController::SetLevel4Difficulty(ELevel4Difficulty Difficulty)
{
	if (ULevel4PuzzleComponent* Level4PuzzleComponent = GetLevel4PuzzleComponent())
	{
		Level4PuzzleComponent->SetLevel4Difficulty(Difficulty);
	}
}

void ACaissonPlayerController::SelectLevel4Piece(int32 PieceIndex)
{
	if (ULevel4PuzzleComponent* Level4PuzzleComponent = GetLevel4PuzzleComponent())
	{
		Level4PuzzleComponent->SelectPiece(PieceIndex);
	}
}

ULevel4PuzzleComponent* ACaissonPlayerController::GetLevel4PuzzleComponent() const
{
	APawn* MyPawn = GetPawn();
	return MyPawn ? MyPawn->FindComponentByClass<ULevel4PuzzleComponent>() : nullptr;
}
