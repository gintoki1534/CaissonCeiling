// Fill out your copyright notice in the Description page of Project Settings.


#include "CaissonInteractComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UCaissonInteractComponent::UCaissonInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractName = TEXT("未命名部件");
}

// Called when the game starts
void UCaissonInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UCaissonInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCaissonInteractComponent::SetHighlightFocus(bool bIsFocused)
{
	// 尝试获取所属 Actor 的所有的渲染组件并开启 Custom Depth (例如用于描边高亮)
	AActor* Owner = GetOwner();
	if (Owner)
	{
		TArray<UPrimitiveComponent*> PrimComps;
		Owner->GetComponents<UPrimitiveComponent>(PrimComps);
		for (UPrimitiveComponent* Comp : PrimComps)
		{
			Comp->SetRenderCustomDepth(bIsFocused);
			// 默认 CustomDepthStencilValue 可在蓝图设置，例如为 1
		}
	}
}
