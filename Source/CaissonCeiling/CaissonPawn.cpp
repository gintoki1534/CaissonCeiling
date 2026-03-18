// Fill out your copyright notice in the Description page of Project Settings.


#include "CaissonPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACaissonPawn::ACaissonPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建 SpringArm (用于围绕目标旋转和缩放相机)
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	RootComponent = SpringArmComp;
	SpringArmComp->TargetArmLength = 1000.f;
	SpringArmComp->bDoCollisionTest = false; // 视需求开启/关闭碰撞
	
	// 创建 Camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ACaissonPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACaissonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACaissonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACaissonPawn::OnLookVectorReceived(const FVector2D& LookAxisVector)
{
	// 确保有输入才计算，避免无意义的运算
	if (LookAxisVector.IsNearlyZero())
	{
		return;
	}

	// 计算旋转量：X轴控制 Yaw (偏航，左右转)，Y轴控制 Pitch (俯仰，上下转)
	// 因为是展示模型，通常我们希望鼠标操作感觉符合直觉（拖拽感），可根据需要调整正负号
	float YawOffset = LookAxisVector.X * RotationSensitivity;
	float PitchOffset = LookAxisVector.Y * RotationSensitivity;

	// 1. 获取当前的旋转
	FRotator CurrentRotation = GetActorRotation();

	// 2. 加上我们计算出的偏移量
	CurrentRotation.Yaw += YawOffset;

	// 限制俯仰角，防止模型直接翻转过去 (比如限制在 -80 到 80 度之间)
	CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch + PitchOffset, -80.0f, 80.0f);

	// 不改变翻滚角 (Roll)
	CurrentRotation.Roll = 0.0f;

	// 3. 应用新的旋转到由 Pawn 整体 (或者 SpringArm)
	SetActorRotation(CurrentRotation);
}

