#include "CaissonInteractComponent.h"

#include "Components/PrimitiveComponent.h"

UCaissonInteractComponent::UCaissonInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractName = TEXT("UnnamedInteractable");
	InteractionId = NAME_None;
	bInteractionEnabled = true;
	bHoverHighlighted = false;
	bPersistentHighlighted = false;
}

void UCaissonInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCaissonInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UCaissonInteractComponent::TryInteract()
{
	if (!bInteractionEnabled)
	{
		return false;
	}

	// 只有允许交互时才广播点击事件，避免重复完成。
	OnInteractClicked.Broadcast();
	return true;
}

void UCaissonInteractComponent::SetInteractionEnabled(bool bEnabled)
{
	bInteractionEnabled = bEnabled;
}

void UCaissonInteractComponent::SetHoverHighlight(bool bEnabled)
{
	if (bHoverHighlighted == bEnabled)
	{
		return;
	}

	bHoverHighlighted = bEnabled;
	NotifyHoverStateChanged(bEnabled);
	RefreshHighlightState();
}

void UCaissonInteractComponent::SetPersistentHighlight(bool bEnabled)
{
	if (bPersistentHighlighted == bEnabled)
	{
		return;
	}

	bPersistentHighlighted = bEnabled;
	RefreshHighlightState();
}

void UCaissonInteractComponent::SetHighlightFocus(bool bIsFocused)
{
	SetHoverHighlight(bIsFocused);
}

void UCaissonInteractComponent::RefreshHighlightState()
{
	const bool bShouldHighlight = bHoverHighlighted || bPersistentHighlighted;

	if (AActor* Owner = GetOwner())
	{
		TArray<UPrimitiveComponent*> PrimitiveComponents;
		Owner->GetComponents<UPrimitiveComponent>(PrimitiveComponents);

		for (UPrimitiveComponent* Component : PrimitiveComponents)
		{
			// 统一使用 Custom Depth，蓝图或材质里可以继续扩展描边/发光效果。
			Component->SetRenderCustomDepth(bShouldHighlight);
		}
	}
}

void UCaissonInteractComponent::NotifyHoverStateChanged(bool bHovered)
{
	if (bHovered)
	{
		OnHoverBegin.Broadcast();
	}
	else
	{
		OnHoverEnd.Broadcast();
	}
}
