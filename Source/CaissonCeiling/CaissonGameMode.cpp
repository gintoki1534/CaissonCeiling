// Fill out your copyright notice in the Description page of Project Settings.


#include "CaissonGameMode.h"
#include "CaissonPlayerController.h"

ACaissonGameMode::ACaissonGameMode()
{
	// 设置默认的 PlayerController
	PlayerControllerClass = ACaissonPlayerController::StaticClass();
}

void ACaissonGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}
