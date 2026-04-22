#include "CaissonInteractComponent.h"

#include "Components/ChildActorComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/PrimitiveComponent.h"

namespace
{
void GatherActorHierarchy(AActor* RootActor, TArray<AActor*>& OutActors)
{
	if (!RootActor)
	{
		return;
	}

	TSet<AActor*> VisitedActors;
	TArray<AActor*> PendingActors;
	PendingActors.Add(RootActor);

	while (PendingActors.Num() > 0)
	{
		AActor* CurrentActor = PendingActors.Pop(EAllowShrinking::No);
		if (!CurrentActor || VisitedActors.Contains(CurrentActor))
		{
			continue;
		}

		VisitedActors.Add(CurrentActor);
		OutActors.Add(CurrentActor);

		TArray<AActor*> AttachedActors;
		CurrentActor->GetAttachedActors(AttachedActors);
		for (AActor* AttachedActor : AttachedActors)
		{
			PendingActors.Add(AttachedActor);
		}

		TArray<UChildActorComponent*> ChildActorComponents;
		CurrentActor->GetComponents<UChildActorComponent>(ChildActorComponents);
		for (UChildActorComponent* ChildActorComponent : ChildActorComponents)
		{
			if (ChildActorComponent && ChildActorComponent->GetChildActor())
			{
				PendingActors.Add(ChildActorComponent->GetChildActor());
			}
		}
	}
}

void GatherPrimitiveComponents(AActor* RootActor, TArray<UPrimitiveComponent*>& OutComponents)
{
	TArray<AActor*> HierarchyActors;
	GatherActorHierarchy(RootActor, HierarchyActors);

	for (AActor* Actor : HierarchyActors)
	{
		TArray<UPrimitiveComponent*> PrimitiveComponents;
		Actor->GetComponents<UPrimitiveComponent>(PrimitiveComponents);

		for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
		{
			if (!IsValid(PrimitiveComponent))
			{
				continue;
			}

			OutComponents.Add(PrimitiveComponent);
		}
	}
}

}

UCaissonInteractComponent::UCaissonInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractName = TEXT("UnnamedInteractable");
	InteractionId = NAME_None;
	bInteractionEnabled = true;
	bEnablePulseLights = true;
	PulseLightCount = 3;
	PulseSpeed = 2.2f;
	PulseIntensityScale = 0.0005f;
	HoverPulseMaxIntensity = 2200.0f;
	ActivatedPulseMaxIntensity = 4200.0f;
	PulseMinBrightnessRatio = 0.35f;
	PulseRangeScale = 2.2f;
	PulseBoundsPadding = 36.0f;
	PulseLightingChannel = 2;
	HoverPulseColor = FLinearColor(1.0f, 0.82f, 0.42f, 1.0f);
	ActivatedPulseColor = FLinearColor(1.0f, 0.72f, 0.22f, 1.0f);
	bHoverHighlighted = false;
	bPersistentHighlighted = false;
	PulseTime = 0.0f;
}

void UCaissonInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	EnsurePulseLights();
	RepositionPulseLights();
	UpdatePulseLights();
}

void UCaissonInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PulseTime += DeltaTime;
	UpdatePulseLights();
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

bool UCaissonInteractComponent::IsPersistentHighlightEnabled() const
{
	return bPersistentHighlighted;
}

void UCaissonInteractComponent::SetHighlightFocus(bool bIsFocused)
{
	SetHoverHighlight(bIsFocused);
}

void UCaissonInteractComponent::SetHighlightTarget(UPrimitiveComponent* HitComponent, AActor* HitActor)
{
	HighlightTargetActor = nullptr;

	if (IsValid(HitActor))
	{
		HighlightTargetActor = HitActor;
	}
	else if (IsValid(HitComponent))
	{
		HighlightTargetActor = HitComponent->GetOwner();
	}

	if (bHoverHighlighted || bPersistentHighlighted)
	{
		UpdatePulseLightingChannels();
		RepositionPulseLights();
		UpdatePulseLights();
	}
}

void UCaissonInteractComponent::ClearHighlightTarget()
{
	HighlightTargetActor = nullptr;

	if (bHoverHighlighted || bPersistentHighlighted)
	{
		UpdatePulseLightingChannels();
		RepositionPulseLights();
		UpdatePulseLights();
	}
}

void UCaissonInteractComponent::RefreshHighlightState()
{
	const bool bShouldHighlight = bHoverHighlighted || bPersistentHighlighted;

	if (AActor* Owner = GetOwner())
	{
		TArray<UPrimitiveComponent*> PrimitiveComponents;
		CollectHighlightPrimitiveComponents(PrimitiveComponents);

		for (UPrimitiveComponent* Component : PrimitiveComponents)
		{
			// 统一使用 Custom Depth，蓝图或材质里可以继续扩展描边/发光效果。
			Component->SetRenderCustomDepth(bShouldHighlight);
		}
	}

	UpdatePulseLightingChannels();
	EnsurePulseLights();
	RepositionPulseLights();
	UpdatePulseLights();
}

void UCaissonInteractComponent::EnsurePulseLights()
{
	if (!bEnablePulseLights || PulseLightComponents.Num() > 0)
	{
		return;
	}

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	USceneComponent* AttachRoot = Owner->GetRootComponent();
	if (!AttachRoot)
	{
		return;
	}

	const int32 ClampedLightCount = FMath::Clamp(PulseLightCount, 1, 6);

	for (int32 Index = 0; Index < ClampedLightCount; ++Index)
	{
		UPointLightComponent* PulseLight = NewObject<UPointLightComponent>(Owner);
		if (!PulseLight)
		{
			continue;
		}

		PulseLight->SetMobility(EComponentMobility::Movable);
		PulseLight->SetCastShadows(false);
		PulseLight->SetUseInverseSquaredFalloff(false);
		PulseLight->SetIntensity(0.0f);
		PulseLight->SetVisibility(true);
		PulseLight->SetHiddenInGame(false);
		bool bChannel0 = false;
		bool bChannel1 = false;
		bool bChannel2 = false;
		GetPulseLightingChannelMask(bChannel0, bChannel1, bChannel2);
		PulseLight->SetLightingChannels(bChannel0, bChannel1, bChannel2);
		PulseLight->SetupAttachment(AttachRoot);

		Owner->AddInstanceComponent(PulseLight);
		PulseLight->RegisterComponent();
		PulseLightComponents.Add(PulseLight);
	}
}

void UCaissonInteractComponent::UpdatePulseLightingChannels()
{
	RestorePulseLightingChannels();

	if (!bEnablePulseLights || !(bHoverHighlighted || bPersistentHighlighted))
	{
		return;
	}

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	CollectHighlightPrimitiveComponents(PrimitiveComponents);
	if (PrimitiveComponents.Num() == 0)
	{
		return;
	}

	bool bPulseChannel0 = false;
	bool bPulseChannel1 = false;
	bool bPulseChannel2 = false;
	GetPulseLightingChannelMask(bPulseChannel0, bPulseChannel1, bPulseChannel2);

	for (UPointLightComponent* PulseLight : PulseLightComponents)
	{
		if (IsValid(PulseLight))
		{
			PulseLight->SetLightingChannels(bPulseChannel0, bPulseChannel1, bPulseChannel2);
		}
	}

	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		if (!IsValid(PrimitiveComponent))
		{
			continue;
		}

		FPrimitiveLightingChannelState& SavedState = OverriddenLightingChannelStates.AddDefaulted_GetRef();
		SavedState.PrimitiveComponent = PrimitiveComponent;
		SavedState.bChannel0 = PrimitiveComponent->LightingChannels.bChannel0;
		SavedState.bChannel1 = PrimitiveComponent->LightingChannels.bChannel1;
		SavedState.bChannel2 = PrimitiveComponent->LightingChannels.bChannel2;

		PrimitiveComponent->SetLightingChannels(
			SavedState.bChannel0 || bPulseChannel0,
			SavedState.bChannel1 || bPulseChannel1,
			SavedState.bChannel2 || bPulseChannel2);
	}
}

void UCaissonInteractComponent::RestorePulseLightingChannels()
{
	for (const FPrimitiveLightingChannelState& SavedState : OverriddenLightingChannelStates)
	{
		if (!SavedState.PrimitiveComponent.IsValid())
		{
			continue;
		}

		SavedState.PrimitiveComponent->SetLightingChannels(
			SavedState.bChannel0,
			SavedState.bChannel1,
			SavedState.bChannel2);
	}

	OverriddenLightingChannelStates.Reset();
}

void UCaissonInteractComponent::GetPulseLightingChannelMask(bool& bOutChannel0, bool& bOutChannel1, bool& bOutChannel2) const
{
	bOutChannel0 = false;
	bOutChannel1 = (PulseLightingChannel == 1);
	bOutChannel2 = (PulseLightingChannel == 2);
}

void UCaissonInteractComponent::RepositionPulseLights()
{
	if (!bEnablePulseLights || PulseLightComponents.Num() == 0)
	{
		return;
	}

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	CollectHighlightPrimitiveComponents(PrimitiveComponents);

	FBox CombinedBounds(ForceInit);
	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		if (PrimitiveComponent && PrimitiveComponent->IsRegistered())
		{
			CombinedBounds += PrimitiveComponent->Bounds.GetBox();
		}
	}

	if (!CombinedBounds.IsValid)
	{
		CombinedBounds = Owner->GetComponentsBoundingBox(true);
	}

	if (!CombinedBounds.IsValid)
	{
		return;
	}

	const FVector Center = CombinedBounds.GetCenter();
	const FVector Extent = CombinedBounds.GetExtent();
	const float HorizontalRadius = FMath::Max(FMath::Max(Extent.X, Extent.Y), 20.0f) + PulseBoundsPadding;
	const float VerticalOffset = FMath::Max(Extent.Z * 0.45f, 18.0f) + (PulseBoundsPadding * 0.25f);
	const float AttenuationRadius = FMath::Max3(Extent.X, Extent.Y, Extent.Z) * PulseRangeScale + PulseBoundsPadding * 2.0f;

	for (int32 Index = 0; Index < PulseLightComponents.Num(); ++Index)
	{
		UPointLightComponent* PulseLight = PulseLightComponents[Index];
		if (!PulseLight)
		{
			continue;
		}

		FVector WorldLocation = Center + FVector(0.0f, 0.0f, VerticalOffset);
		if (PulseLightComponents.Num() > 1)
		{
			const float Angle = (static_cast<float>(Index) / static_cast<float>(PulseLightComponents.Num())) * (2.0f * PI);
			WorldLocation += FVector(FMath::Cos(Angle) * HorizontalRadius, FMath::Sin(Angle) * HorizontalRadius, 0.0f);
		}

		PulseLight->SetWorldLocation(WorldLocation);
		PulseLight->SetAttenuationRadius(AttenuationRadius);
	}
}

void UCaissonInteractComponent::UpdatePulseLights()
{
	if (PulseLightComponents.Num() == 0)
	{
		return;
	}

	const bool bShouldPulse = bHoverHighlighted || bPersistentHighlighted;
	const float BaseIntensity = bPersistentHighlighted ? ActivatedPulseMaxIntensity : HoverPulseMaxIntensity;
	const float MaxIntensity = BaseIntensity * PulseIntensityScale;
	const FLinearColor LightColor = bPersistentHighlighted ? ActivatedPulseColor : HoverPulseColor;

	for (int32 Index = 0; Index < PulseLightComponents.Num(); ++Index)
	{
		UPointLightComponent* PulseLight = PulseLightComponents[Index];
		if (!PulseLight)
		{
			continue;
		}

		if (!bEnablePulseLights || !bShouldPulse || MaxIntensity <= 0.0f)
		{
			PulseLight->SetIntensity(0.0f);
			continue;
		}

		const float PhaseOffset = (static_cast<float>(Index) / FMath::Max(1, PulseLightComponents.Num())) * (2.0f * PI);
		const float PulseAlpha = 0.5f + 0.5f * FMath::Sin(PulseTime * PulseSpeed + PhaseOffset);
		const float BrightnessRatio = FMath::Lerp(PulseMinBrightnessRatio, 1.0f, PulseAlpha);

		PulseLight->SetLightColor(LightColor);
		PulseLight->SetIntensity(MaxIntensity * BrightnessRatio);
	}
}

void UCaissonInteractComponent::CollectHighlightPrimitiveComponents(TArray<UPrimitiveComponent*>& OutComponents) const
{
	if (HighlightTargetActor.IsValid())
	{
		GatherPrimitiveComponents(HighlightTargetActor.Get(), OutComponents);
	}

	if (OutComponents.Num() > 0)
	{
		return;
	}

	if (AActor* Owner = GetOwner())
	{
		GatherPrimitiveComponents(Owner, OutComponents);
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
