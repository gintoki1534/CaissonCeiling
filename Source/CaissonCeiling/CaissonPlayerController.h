// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CaissonPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

// ========== 步骤变更委托：当步骤推进时，向所有监听者广播新的步骤编号 ==========
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStepChanged, int32, NewStep);

/**
 * 隆福寺核心玩家控制器，处理增强输入、UI 流转与关卡步骤管理
 */
UCLASS()
class CAISSONCEILING_API ACaissonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACaissonPlayerController();

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	// =============== 增强输入配置 ===============

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Context")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* ClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Actions")
	UInputAction* RightClickAction;

	// 处理视角转动
	void Look(const FInputActionValue& Value);

	// 处理点击交互
	void OnInteract();

	// 处理右键 -> 自动推进步骤
	void OnRightClickSkip();

public:
	// =============== 关卡步骤管理系统 =================

	/** 当前步骤编号 (从 0 开始) */
	UPROPERTY(BlueprintReadOnly, Category="CaissonStep")
	int32 CurrentStep;

	/** 总步骤数 (在蓝图里设置，例如 3) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CaissonStep")
	int32 TotalSteps;

	/** 步骤变更事件：UI 蓝图绑定此委托，自动收到新步骤编号并刷新界面 */
	UPROPERTY(BlueprintAssignable, Category="CaissonStep")
	FOnStepChanged OnStepChanged;

	/** 推进到下一步（蓝图也可以手动调用） */
	UFUNCTION(BlueprintCallable, Category="CaissonStep")
	void AdvanceStep();

	/** 重置步骤到 0 */
	UFUNCTION(BlueprintCallable, Category="CaissonStep")
	void ResetSteps();

	// =============== 统管 UI 流转与焦点 =================
	
	/** 
	 * 傻瓜式打开 UI 蓝图接口：自动创建对象、添加至屏幕、设置游戏与UI模式并让其获得焦点 
	 */
	UFUNCTION(BlueprintCallable, Category="CaissonUI")
	UUserWidget* OpenCaissonWidget(TSubclassOf<UUserWidget> WidgetClass);

	/** 
	 * 傻瓜式关闭 UI 蓝图接口：从父级移除该 UI、复原 GameOnly 模式并强制夺回 3D 世界焦点 
	 */
	UFUNCTION(BlueprintCallable, Category="CaissonUI")
	void CloseCaissonWidget(UUserWidget* WidgetToClose);
};