// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CaissonPawn.generated.h"

class UCameraComponent;
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
