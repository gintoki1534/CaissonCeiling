#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CaissonPawn.generated.h"

class UCameraComponent;
class USceneComponent;
class USpringArmComponent;

/**
 * 隆福寺藻井 漫游相机 Pawn
 */
UCLASS()
class CAISSONCEILING_API ACaissonPawn : public APawn
{
	GENERATED_BODY()

public:
	ACaissonPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 独立场景根节点。整个展示 Pawn 的旋转中心由它决定。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USceneComponent* SceneRootComp;

	// 模型旋转枢轴。展示模型应挂在这个节点下，鼠标拖拽时只旋转它。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USceneComponent* ModelPivotComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	// =============== 旋转控制参数 ===============

	// 旋转灵敏度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Control")
	float RotationSensitivity = 1.0f;

	// 接收鼠标输入的接口，供 Controller 调用
	UFUNCTION(BlueprintCallable, Category = "Camera|Control")
	void OnLookVectorReceived(const FVector2D& LookAxisVector);
};
