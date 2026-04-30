#include "Level4PuzzlePieceActor.h"

#include "Components/ChildActorComponent.h"
#include "Components/MeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"

namespace
{
void GatherLevel4PieceActorHierarchy(AActor* RootActor, TArray<AActor*>& OutActors)
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

void GatherLevel4PiecePrimitiveComponents(AActor* RootActor, TArray<UPrimitiveComponent*>& OutComponents)
{
	TArray<AActor*> HierarchyActors;
	GatherLevel4PieceActorHierarchy(RootActor, HierarchyActors);

	TSet<UPrimitiveComponent*> VisitedComponents;
	for (AActor* Actor : HierarchyActors)
	{
		TArray<UPrimitiveComponent*> PrimitiveComponents;
		Actor->GetComponents<UPrimitiveComponent>(PrimitiveComponents);

		for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
		{
			if (!IsValid(PrimitiveComponent) || VisitedComponents.Contains(PrimitiveComponent))
			{
				continue;
			}

			VisitedComponents.Add(PrimitiveComponent);
			OutComponents.Add(PrimitiveComponent);
		}
	}
}

void GatherLevel4PieceMeshComponents(AActor* RootActor, TArray<UMeshComponent*>& OutComponents)
{
	TArray<AActor*> HierarchyActors;
	GatherLevel4PieceActorHierarchy(RootActor, HierarchyActors);

	TSet<UMeshComponent*> VisitedComponents;
	for (AActor* Actor : HierarchyActors)
	{
		TArray<UMeshComponent*> MeshComponents;
		Actor->GetComponents<UMeshComponent>(MeshComponents);

		for (UMeshComponent* MeshComponent : MeshComponents)
		{
			if (!IsValid(MeshComponent) || VisitedComponents.Contains(MeshComponent))
			{
				continue;
			}

			VisitedComponents.Add(MeshComponent);
			OutComponents.Add(MeshComponent);
		}
	}
}
}

ALevel4PuzzlePieceActor::ALevel4PuzzlePieceActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SceneRoot);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComponent->SetGenerateOverlapEvents(false);
}

void ALevel4PuzzlePieceActor::BeginPlay()
{
	Super::BeginPlay();
	EnsurePulseOverlayMaterials();
	EnsurePulseLights();
	RefreshVisualState();
}

void ALevel4PuzzlePieceActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RestorePulseOverlayMaterials();
	RestorePulseLightingChannels();
	Super::EndPlay(EndPlayReason);
}

void ALevel4PuzzlePieceActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	PulseTime += DeltaSeconds;
	UpdatePulseOverlayMaterials();
	UpdatePulseLights();
}

void ALevel4PuzzlePieceActor::InitializeLevel4Piece(int32 InPieceIndex)
{
	PieceIndex = InPieceIndex;
	bIsSpawnedInField = true;
}

void ALevel4PuzzlePieceActor::SetSelectedVisual(bool bNewSelected)
{
	bIsSelected = bNewSelected;
	RefreshVisualState();
	BP_OnSelectedVisualChanged(bIsSelected);
}

void ALevel4PuzzlePieceActor::SetCorrectVisual(bool bNewCorrect)
{
	bIsCorrect = bNewCorrect;
	RefreshVisualState();
	BP_OnCorrectVisualChanged(bIsCorrect);
}

UPrimitiveComponent* ALevel4PuzzlePieceActor::GetDragPrimitive() const
{
	return MeshComponent;
}

void ALevel4PuzzlePieceActor::GetInteractivePrimitiveComponents(TArray<UPrimitiveComponent*>& OutComponents) const
{
	CollectVisualPrimitiveComponents(OutComponents);
}

void ALevel4PuzzlePieceActor::RefreshVisualState()
{
	ApplyCustomDepthVisual();
	EnsurePulseOverlayMaterials();
	UpdatePulseOverlayMaterials();
	UpdatePulseLightingChannels();
	EnsurePulseLights();
	RepositionPulseLights();
	UpdatePulseLights();
}

void ALevel4PuzzlePieceActor::ApplyCustomDepthVisual()
{
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	CollectVisualPrimitiveComponents(PrimitiveComponents);

	const bool bShouldHighlight = bIsSelected || bIsCorrect;
	const int32 StencilValue = bIsSelected ? SelectedCustomDepthStencil : CorrectCustomDepthStencil;

	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		if (!IsValid(PrimitiveComponent))
		{
			continue;
		}

		PrimitiveComponent->SetRenderCustomDepth(bUseCustomDepthForSelection && bShouldHighlight);
		if (bUseCustomDepthForSelection && bShouldHighlight)
		{
			PrimitiveComponent->SetCustomDepthStencilValue(StencilValue);
		}
	}
}

float ALevel4PuzzlePieceActor::GetPulsePhaseSeconds() const
{
	const UWorld* World = GetWorld();
	return World ? World->GetTimeSeconds() : PulseTime;
}

void ALevel4PuzzlePieceActor::EnsurePulseOverlayMaterials()
{
	if (!bEnableOverlayPulse || !PulseOverlayMaterial || PulseOverlaySlots.Num() > 0)
	{
		return;
	}

	TArray<UMeshComponent*> MeshComponents;
	CollectVisualMeshComponents(MeshComponents);

	for (UMeshComponent* VisualMeshComponent : MeshComponents)
	{
		if (!IsValid(VisualMeshComponent))
		{
			continue;
		}

		UMaterialInstanceDynamic* DynamicOverlayMaterial = UMaterialInstanceDynamic::Create(PulseOverlayMaterial, this);
		if (!DynamicOverlayMaterial)
		{
			continue;
		}

		FLevel4PulseOverlaySlot& PulseOverlaySlot = PulseOverlaySlots.AddDefaulted_GetRef();
		PulseOverlaySlot.MeshComponent = VisualMeshComponent;
		PulseOverlaySlot.OriginalOverlayMaterial = VisualMeshComponent->GetOverlayMaterial();
		PulseOverlaySlot.DynamicOverlayMaterial = DynamicOverlayMaterial;
		PulseOverlayMaterialInstances.Add(DynamicOverlayMaterial);
	}
}

void ALevel4PuzzlePieceActor::UpdatePulseOverlayMaterials()
{
	if (PulseOverlaySlots.Num() == 0)
	{
		return;
	}

	const bool bShouldPulse = bEnableOverlayPulse && PulseOverlayMaterial && (bIsSelected || bIsCorrect);
	const bool bUseSelectedVisual = bIsSelected;
	const float BaseIntensity = bUseSelectedVisual ? SelectedPulseMaxIntensity : CorrectPulseMaxIntensity;
	const float MaxIntensity = BaseIntensity * PulseIntensityScale;
	const FLinearColor PulseColor = bUseSelectedVisual ? SelectedPulseColor : CorrectPulseColor;
	const float PulseAlpha = 0.5f + 0.5f * FMath::Sin(GetPulsePhaseSeconds() * PulseSpeed);
	const float BrightnessRatio = FMath::Lerp(PulseMinBrightnessRatio, 1.0f, PulseAlpha);
	const float PulseIntensity = bShouldPulse ? MaxIntensity * BrightnessRatio : 0.0f;
	const FLinearColor EmissiveColor = PulseColor * PulseIntensity;

	for (const FLevel4PulseOverlaySlot& PulseOverlaySlot : PulseOverlaySlots)
	{
		UMeshComponent* VisualMeshComponent = PulseOverlaySlot.MeshComponent.Get();
		UMaterialInstanceDynamic* DynamicOverlayMaterial = PulseOverlaySlot.DynamicOverlayMaterial.Get();
		if (!VisualMeshComponent || !DynamicOverlayMaterial)
		{
			continue;
		}

		DynamicOverlayMaterial->SetScalarParameterValue(TEXT("Level4PulseIntensity"), PulseIntensity);
		DynamicOverlayMaterial->SetScalarParameterValue(TEXT("PulseIntensity"), PulseIntensity);
		DynamicOverlayMaterial->SetScalarParameterValue(TEXT("GlowIntensity"), PulseIntensity);
		DynamicOverlayMaterial->SetScalarParameterValue(TEXT("HighlightIntensity"), PulseIntensity);
		DynamicOverlayMaterial->SetScalarParameterValue(TEXT("EmissiveStrength"), PulseIntensity);

		DynamicOverlayMaterial->SetVectorParameterValue(TEXT("Level4PulseColor"), PulseColor);
		DynamicOverlayMaterial->SetVectorParameterValue(TEXT("PulseColor"), PulseColor);
		DynamicOverlayMaterial->SetVectorParameterValue(TEXT("GlowColor"), PulseColor);
		DynamicOverlayMaterial->SetVectorParameterValue(TEXT("HighlightColor"), PulseColor);
		DynamicOverlayMaterial->SetVectorParameterValue(TEXT("EmissiveColor"), EmissiveColor);

		VisualMeshComponent->SetOverlayMaterial(bShouldPulse ? DynamicOverlayMaterial : PulseOverlaySlot.OriginalOverlayMaterial.Get());
	}
}

void ALevel4PuzzlePieceActor::RestorePulseOverlayMaterials()
{
	for (const FLevel4PulseOverlaySlot& PulseOverlaySlot : PulseOverlaySlots)
	{
		UMeshComponent* VisualMeshComponent = PulseOverlaySlot.MeshComponent.Get();
		if (!VisualMeshComponent)
		{
			continue;
		}

		VisualMeshComponent->SetOverlayMaterial(PulseOverlaySlot.OriginalOverlayMaterial.Get());
	}

	PulseOverlaySlots.Reset();
	PulseOverlayMaterialInstances.Reset();
}

void ALevel4PuzzlePieceActor::EnsurePulseLights()
{
	if (!bEnablePulseLights || PulseLightComponents.Num() > 0)
	{
		return;
	}

	USceneComponent* AttachRoot = GetRootComponent();
	if (!AttachRoot)
	{
		return;
	}

	const int32 ClampedLightCount = FMath::Clamp(PulseLightCount, 1, 6);
	for (int32 Index = 0; Index < ClampedLightCount; ++Index)
	{
		UPointLightComponent* PulseLight = NewObject<UPointLightComponent>(this);
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

		AddInstanceComponent(PulseLight);
		PulseLight->RegisterComponent();
		PulseLightComponents.Add(PulseLight);
	}
}

void ALevel4PuzzlePieceActor::RepositionPulseLights()
{
	if (!bEnablePulseLights || PulseLightComponents.Num() == 0)
	{
		return;
	}

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	CollectVisualPrimitiveComponents(PrimitiveComponents);

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
		CombinedBounds = GetComponentsBoundingBox(true);
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

void ALevel4PuzzlePieceActor::UpdatePulseLights()
{
	if (PulseLightComponents.Num() == 0)
	{
		return;
	}

	const bool bShouldPulse = bIsSelected || bIsCorrect;
	const bool bUseSelectedVisual = bIsSelected;
	const float BaseIntensity = bUseSelectedVisual ? SelectedPulseMaxIntensity : CorrectPulseMaxIntensity;
	const float MaxIntensity = BaseIntensity * PulseIntensityScale;
	const FLinearColor LightColor = bUseSelectedVisual ? SelectedPulseColor : CorrectPulseColor;

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
		const float PulseAlpha = 0.5f + 0.5f * FMath::Sin(GetPulsePhaseSeconds() * PulseSpeed + PhaseOffset);
		const float BrightnessRatio = FMath::Lerp(PulseMinBrightnessRatio, 1.0f, PulseAlpha);

		PulseLight->SetLightColor(LightColor);
		PulseLight->SetIntensity(MaxIntensity * BrightnessRatio);
	}
}

void ALevel4PuzzlePieceActor::UpdatePulseLightingChannels()
{
	RestorePulseLightingChannels();

	if (!bEnablePulseLights || !(bIsSelected || bIsCorrect))
	{
		return;
	}

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	CollectVisualPrimitiveComponents(PrimitiveComponents);
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

void ALevel4PuzzlePieceActor::RestorePulseLightingChannels()
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

void ALevel4PuzzlePieceActor::GetPulseLightingChannelMask(bool& bOutChannel0, bool& bOutChannel1, bool& bOutChannel2) const
{
	bOutChannel0 = false;
	bOutChannel1 = (PulseLightingChannel == 1);
	bOutChannel2 = (PulseLightingChannel == 2);
}

void ALevel4PuzzlePieceActor::CollectVisualPrimitiveComponents(TArray<UPrimitiveComponent*>& OutComponents) const
{
	GatherLevel4PiecePrimitiveComponents(const_cast<ALevel4PuzzlePieceActor*>(this), OutComponents);
}

void ALevel4PuzzlePieceActor::CollectVisualMeshComponents(TArray<UMeshComponent*>& OutComponents) const
{
	GatherLevel4PieceMeshComponents(const_cast<ALevel4PuzzlePieceActor*>(this), OutComponents);
}
