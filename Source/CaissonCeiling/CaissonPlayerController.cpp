// Fill out your copyright notice in the Description page of Project Settings.


#include "CaissonPlayerController.h"
#include "CaissonInteractComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "CaissonPawn.h"

ACaissonPlayerController::ACaissonPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// 步骤系统初始化
	CurrentStep = 0;
	TotalSteps = 3; // 默认 3 步，可在蓝图里覆盖
}

void ACaissonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 添加输入映射上下文
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACaissonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// 绑定视角转动 (Look)
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACaissonPlayerController::Look);
		}

		// 绑定点击交互 (Click)
		if (ClickAction)
		{
			EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &ACaissonPlayerController::OnInteract);
		}

		// 绑定右键 -> 推进步骤 (Right Click)
		if (RightClickAction)
		{
			EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &ACaissonPlayerController::OnRightClickSkip);
		}
	}
}

// =============== 步骤管理系统 ===============

void ACaissonPlayerController::AdvanceStep()
{
	if (CurrentStep < TotalSteps)
	{
		CurrentStep++;
		UE_LOG(LogTemp, Warning, TEXT("[C++ 步骤] 推进到第 %d 步 (共 %d 步)"), CurrentStep, TotalSteps);
		
		// 向所有监听者广播（UI 就是通过这个来刷新界面的）
		OnStepChanged.Broadcast(CurrentStep);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[C++ 步骤] 已到达最后一步，无法继续推进"));
	}
}

void ACaissonPlayerController::ResetSteps()
{
	CurrentStep = 0;
	OnStepChanged.Broadcast(CurrentStep);
}

void ACaissonPlayerController::OnRightClickSkip()
{
	UE_LOG(LogTemp, Warning, TEXT("[C++ 右键] 检测到右键点击，尝试推进步骤..."));
	AdvanceStep();
}

// =============== 视角与交互 ===============

void ACaissonPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	APawn* MyPawn = GetPawn();
	if (!MyPawn) return;

	// 优先尝试 CaissonPawn（它有内置的灵敏度和俯仰限制）
	if (ACaissonPawn* CaissonPawn = Cast<ACaissonPawn>(MyPawn))
	{
		CaissonPawn->OnLookVectorReceived(LookAxisVector);
	}
	else
	{
		// 通用后备方案：直接旋转任何被 Possess 的 Pawn/Actor
		float YawOffset = LookAxisVector.X * 1.0f;
		float PitchOffset = LookAxisVector.Y * 1.0f;

		FRotator CurrentRotation = MyPawn->GetActorRotation();
		CurrentRotation.Yaw += YawOffset;
		CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch + PitchOffset, -80.0f, 80.0f);
		CurrentRotation.Roll = 0.0f;
		MyPawn->SetActorRotation(CurrentRotation);
	}
}

void ACaissonPlayerController::OnInteract()
{
	FVector WorldLocation;
	FVector WorldDirection;
	
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = TraceStart + (WorldDirection * 10000.f);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetPawn());

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

		if (bHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("[C++] 射线击中: %s"), *HitActor->GetName());
				
				UCaissonInteractComponent* InteractComp = HitActor->FindComponentByClass<UCaissonInteractComponent>();
				if (InteractComp)
				{
					InteractComp->OnInteractClicked.Broadcast();
				}
			}
		}
	}
}

// =============== UI 管理 ===============

UUserWidget* ACaissonPlayerController::OpenCaissonWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if (!WidgetClass) return nullptr;
	
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
