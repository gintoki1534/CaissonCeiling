#include "CaissonPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACaissonPawn::ACaissonPawn()
{
	// 漫游场景需要持续处理相机旋转，因此保留 Tick。
	PrimaryActorTick.bCanEverTick = true;

	// 创建独立根节点。这样展示模型和相机系统可以分层管理，
	// 鼠标拖拽旋转时会围绕 Pawn 原点旋转，而不是围绕 SpringArm 本身。
	SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComp"));
	RootComponent = SceneRootComp;

	// 创建模型旋转枢轴。展示模型统一挂在它下面，
	// 鼠标拖拽时只旋转这个节点，不让相机跟着一起转。
	ModelPivotComp = CreateDefaultSubobject<USceneComponent>(TEXT("ModelPivotComp"));
	ModelPivotComp->SetupAttachment(SceneRootComp);

	// 创建 SpringArm (用于控制 SceneCapture / Camera 的观察距离)
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(SceneRootComp);
	SpringArmComp->TargetArmLength = 1000.f;
	SpringArmComp->bDoCollisionTest = false; // 视需求开启/关闭碰撞
	
	// 创建 Camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
}

void ACaissonPawn::BeginPlay()
{
	Super::BeginPlay();

	if (ModelPivotComp)
	{
		InitialModelPivotRotation = ModelPivotComp->GetRelativeRotation();
	}
}

void ACaissonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateModelPivotRotationReset(DeltaTime);
}

void ACaissonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACaissonPawn::OnLookVectorReceived(const FVector2D& LookAxisVector)
{
	if (bModelPivotRotationResetting)
	{
		return;
	}

	// 确保有输入才计算，避免无意义的运算
	if (LookAxisVector.IsNearlyZero())
	{
		return;
	}

	// 计算旋转量：X轴控制 Yaw (偏航，左右转)，Y轴控制 Pitch (俯仰，上下转)
	// 因为是展示模型，通常我们希望鼠标操作感觉符合直觉（拖拽感），可根据需要调整正负号
	float YawOffset = LookAxisVector.X * RotationSensitivity;
	float PitchOffset = LookAxisVector.Y * RotationSensitivity;

	if (!ModelPivotComp)
	{
		return;
	}

	// 1. 获取当前模型枢轴的相对旋转
	FRotator CurrentRotation = ModelPivotComp->GetRelativeRotation();

	// 2. 加上鼠标拖拽带来的偏移量
	CurrentRotation.Yaw += YawOffset;

	// 限制俯仰角，防止模型直接翻转过去
	CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch + PitchOffset, -80.0f, 80.0f);

	// 不改变翻滚角 (Roll)
	CurrentRotation.Roll = InitialModelPivotRotation.Roll;

	// 3. 只把新的旋转应用到模型枢轴，不旋转相机。
	ModelPivotComp->SetRelativeRotation(CurrentRotation);
}

bool ACaissonPawn::BeginResetModelPivotRotation(float DurationSeconds)
{
	if (!ModelPivotComp)
	{
		return false;
	}

	const FRotator CurrentRotation = ModelPivotComp->GetRelativeRotation();
	if (CurrentRotation.Equals(InitialModelPivotRotation, 0.1f))
	{
		ModelPivotComp->SetRelativeRotation(InitialModelPivotRotation);
		bModelPivotRotationResetting = false;
		return false;
	}

	ModelPivotResetStartRotation = CurrentRotation;
	ModelPivotResetElapsedSeconds = 0.0f;
	ActiveModelPivotResetDuration = DurationSeconds > 0.0f ? DurationSeconds : ModelPivotResetDuration;

	if (ActiveModelPivotResetDuration <= KINDA_SMALL_NUMBER)
	{
		FinishModelPivotRotationReset();
		return false;
	}

	bModelPivotRotationResetting = true;
	return true;
}

bool ACaissonPawn::IsModelPivotRotationResetting() const
{
	return bModelPivotRotationResetting;
}

void ACaissonPawn::UpdateModelPivotRotationReset(float DeltaTime)
{
	if (!bModelPivotRotationResetting || !ModelPivotComp)
	{
		return;
	}

	ModelPivotResetElapsedSeconds += FMath::Max(0.0f, DeltaTime);
	const float Alpha = FMath::Clamp(ModelPivotResetElapsedSeconds / ActiveModelPivotResetDuration, 0.0f, 1.0f);
	const float EasedAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, Alpha, 2.0f);
	const FQuat StartQuat = ModelPivotResetStartRotation.Quaternion();
	const FQuat TargetQuat = InitialModelPivotRotation.Quaternion();
	ModelPivotComp->SetRelativeRotation(FQuat::Slerp(StartQuat, TargetQuat, EasedAlpha).Rotator());

	if (Alpha >= 1.0f)
	{
		FinishModelPivotRotationReset();
	}
}

void ACaissonPawn::FinishModelPivotRotationReset()
{
	if (ModelPivotComp)
	{
		ModelPivotComp->SetRelativeRotation(InitialModelPivotRotation);
	}

	const bool bWasResetting = bModelPivotRotationResetting;
	bModelPivotRotationResetting = false;
	ModelPivotResetElapsedSeconds = 0.0f;
	ActiveModelPivotResetDuration = 0.0f;
	ModelPivotResetStartRotation = InitialModelPivotRotation;

	if (bWasResetting)
	{
		OnModelPivotRotationResetFinished.Broadcast();
	}
}
