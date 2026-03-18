// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CaissonGameMode.generated.h"

/**
 * 隆福寺藻井 基础游戏模式控制类
 */
UCLASS()
class CAISSONCEILING_API ACaissonGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACaissonGameMode();

protected:
	virtual void BeginPlay() override;
};
