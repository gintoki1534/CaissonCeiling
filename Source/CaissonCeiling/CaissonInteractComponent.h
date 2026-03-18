// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CaissonInteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCaissonInteractSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAISSONCEILING_API UCaissonInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCaissonInteractComponent();

	// 交互名称 (例如：斗拱、天花)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FString InteractName;

	// 当被点击时触发
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnCaissonInteractSignature OnInteractClicked;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 供外部射线检测等调用的交互高亮接口
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void SetHighlightFocus(bool bIsFocused);
};
