// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CaissonUserWidget.generated.h"

/**
 * 隆福寺 UI 蓝图基类
 */
UCLASS()
class CAISSONCEILING_API UCaissonUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 供蓝图调用的通用初始化函数，例如绑定按钮等操作
	UFUNCTION(BlueprintCallable, Category = "CaissonUI")
	virtual void InitCaissonWidget();
	
protected:
	virtual void NativeConstruct() override;
};
