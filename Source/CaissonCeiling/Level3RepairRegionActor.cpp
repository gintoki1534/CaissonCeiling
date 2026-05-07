#include "Level3RepairRegionActor.h"

#include "Components/PointLightComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Level3RepairAreaComponent.h"

ALevel3RepairRegionActor::ALevel3RepairRegionActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComp"));
	SetRootComponent(SceneRootComp);

	PreviewMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMeshComp"));
	PreviewMeshComp->SetupAttachment(SceneRootComp);
	PreviewMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PreviewMeshComp->SetCollisionResponseToAllChannels(ECR_Block);

	RepairAreaComp = CreateDefaultSubobject<ULevel3RepairAreaComponent>(TEXT("RepairAreaComp"));

	bEnableTransitionPulseLights = true;
	TransitionPulseLightCount = 3;
	TransitionPulseSpeed = 1.6f;
	TransitionPulseFadeSeconds = 0.5f;
	TransitionPulseIntensityScale = 0.00025f;
	TransitionPulseMaxIntensity = 2200.0f;
	TransitionPulseMinBrightnessRatio = 0.25f;
	TransitionPulseRangeScale = 2.0f;
	TransitionPulseBoundsPadding = 24.0f;
	TransitionPulseLightingChannel = 2;
	TransitionPulseColor = FLinearColor(0.8f, 0.68f, 0.42f, 1.0f);
	bTransitionPulseActive = false;
	TransitionPulseTime = 0.0f;
	TransitionPulseAlpha = 0.0f;
}

void ALevel3RepairRegionActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ConfigureRepairMeshCollision();
}

void ALevel3RepairRegionActor::BeginPlay()
{
	Super::BeginPlay();
	ConfigureRepairMeshCollision();
}

void ALevel3RepairRegionActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TransitionPulseTime += DeltaSeconds;
	UpdateTransitionPulseFade(DeltaSeconds);
	UpdateTransitionPulseLights();
}

void ALevel3RepairRegionActor::SetTransitionPulseActive(bool bActive)
{
	if (bTransitionPulseActive == bActive)
	{
		return;
	}

	bTransitionPulseActive = bActive;

	if (bTransitionPulseActive)
	{
		SetActorTickEnabled(true);
		EnsureTransitionPulseLights();
		UpdateTransitionPulseLightingChannels();
		RepositionTransitionPulseLights();
		UpdateTransitionPulseFade(0.0f);
		UpdateTransitionPulseLights();
	}
	else
	{
		SetActorTickEnabled(true);
		UpdateTransitionPulseFade(0.0f);
		UpdateTransitionPulseLights();
	}
}

bool ALevel3RepairRegionActor::IsTransitionPulseActive() const
{
	return bTransitionPulseActive || TransitionPulseAlpha > KINDA_SMALL_NUMBER;
}

void ALevel3RepairRegionActor::ConfigureRepairMeshCollision()
{
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);

	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		if (!PrimitiveComponent)
		{
			continue;
		}

		const bool bIsRepairMesh = PrimitiveComponent->GetFName() == TEXT("SM_RepairMesh")
			|| PrimitiveComponent == PreviewMeshComp;
		if (!bIsRepairMesh)
		{
			continue;
		}

		PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PrimitiveComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		PrimitiveComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}
}

void ALevel3RepairRegionActor::EnsureTransitionPulseLights()
{
	if (!bEnableTransitionPulseLights || TransitionPulseLightComponents.Num() > 0)
	{
		return;
	}

	if (!SceneRootComp)
	{
		return;
	}

	const int32 ClampedLightCount = FMath::Clamp(TransitionPulseLightCount, 1, 6);
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
		GetTransitionPulseLightingChannelMask(bChannel0, bChannel1, bChannel2);
		PulseLight->SetLightingChannels(bChannel0, bChannel1, bChannel2);
		PulseLight->SetupAttachment(SceneRootComp);

		AddInstanceComponent(PulseLight);
		PulseLight->RegisterComponent();
		TransitionPulseLightComponents.Add(PulseLight);
	}
}

void ALevel3RepairRegionActor::RepositionTransitionPulseLights()
{
	if (!bEnableTransitionPulseLights || TransitionPulseLightComponents.Num() == 0)
	{
		return;
	}

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	CollectTransitionPulsePrimitiveComponents(PrimitiveComponents);

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
	const float HorizontalRadius = FMath::Max(FMath::Max(Extent.X, Extent.Y), 20.0f) + TransitionPulseBoundsPadding;
	const float VerticalOffset = FMath::Max(Extent.Z * 0.45f, 18.0f) + (TransitionPulseBoundsPadding * 0.25f);
	const float AttenuationRadius = FMath::Max3(Extent.X, Extent.Y, Extent.Z) * TransitionPulseRangeScale + TransitionPulseBoundsPadding * 2.0f;

	for (int32 Index = 0; Index < TransitionPulseLightComponents.Num(); ++Index)
	{
		UPointLightComponent* PulseLight = TransitionPulseLightComponents[Index];
		if (!PulseLight)
		{
			continue;
		}

		FVector WorldLocation = Center + FVector(0.0f, 0.0f, VerticalOffset);
		if (TransitionPulseLightComponents.Num() > 1)
		{
			const float Angle = (static_cast<float>(Index) / static_cast<float>(TransitionPulseLightComponents.Num())) * (2.0f * PI);
			WorldLocation += FVector(FMath::Cos(Angle) * HorizontalRadius, FMath::Sin(Angle) * HorizontalRadius, 0.0f);
		}

		PulseLight->SetWorldLocation(WorldLocation);
		PulseLight->SetAttenuationRadius(AttenuationRadius);
	}
}

void ALevel3RepairRegionActor::UpdateTransitionPulseLights()
{
	if (TransitionPulseLightComponents.Num() == 0)
	{
		return;
	}

	const float MaxIntensity = TransitionPulseMaxIntensity * TransitionPulseIntensityScale * TransitionPulseAlpha;
	for (int32 Index = 0; Index < TransitionPulseLightComponents.Num(); ++Index)
	{
		UPointLightComponent* PulseLight = TransitionPulseLightComponents[Index];
		if (!PulseLight)
		{
			continue;
		}

		if (!bEnableTransitionPulseLights || MaxIntensity <= 0.0f)
		{
			PulseLight->SetIntensity(0.0f);
			continue;
		}

		const float PhaseOffset = (static_cast<float>(Index) / FMath::Max(1, TransitionPulseLightComponents.Num())) * (2.0f * PI);
		const float PulseAlpha = 0.5f + 0.5f * FMath::Sin(TransitionPulseTime * TransitionPulseSpeed + PhaseOffset);
		const float BrightnessRatio = FMath::Lerp(TransitionPulseMinBrightnessRatio, 1.0f, PulseAlpha);

		PulseLight->SetLightColor(TransitionPulseColor);
		PulseLight->SetIntensity(MaxIntensity * BrightnessRatio);
	}
}

void ALevel3RepairRegionActor::UpdateTransitionPulseLightingChannels()
{
	RestoreTransitionPulseLightingChannels();

	if (!bEnableTransitionPulseLights || !bTransitionPulseActive)
	{
		return;
	}

	TArray<UPrimitiveComponent*> PrimitiveComponents;
	CollectTransitionPulsePrimitiveComponents(PrimitiveComponents);
	if (PrimitiveComponents.Num() == 0)
	{
		return;
	}

	bool bPulseChannel0 = false;
	bool bPulseChannel1 = false;
	bool bPulseChannel2 = false;
	GetTransitionPulseLightingChannelMask(bPulseChannel0, bPulseChannel1, bPulseChannel2);

	for (UPointLightComponent* PulseLight : TransitionPulseLightComponents)
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

void ALevel3RepairRegionActor::RestoreTransitionPulseLightingChannels()
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

void ALevel3RepairRegionActor::GetTransitionPulseLightingChannelMask(bool& bOutChannel0, bool& bOutChannel1, bool& bOutChannel2) const
{
	bOutChannel0 = false;
	bOutChannel1 = (TransitionPulseLightingChannel == 1);
	bOutChannel2 = (TransitionPulseLightingChannel == 2);
}

void ALevel3RepairRegionActor::CollectTransitionPulsePrimitiveComponents(TArray<UPrimitiveComponent*>& OutComponents) const
{
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);

	for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
	{
		if (!IsValid(PrimitiveComponent))
		{
			continue;
		}

		const bool bIsRepairMesh = PrimitiveComponent->GetFName() == TEXT("SM_RepairMesh")
			|| PrimitiveComponent == PreviewMeshComp;
		if (bIsRepairMesh)
		{
			OutComponents.Add(PrimitiveComponent);
		}
	}
}

void ALevel3RepairRegionActor::UpdateTransitionPulseFade(float DeltaSeconds)
{
	const float TargetAlpha = bTransitionPulseActive ? 1.0f : 0.0f;
	const float FadeSeconds = FMath::Max(0.0f, TransitionPulseFadeSeconds);

	if (FadeSeconds <= KINDA_SMALL_NUMBER)
	{
		TransitionPulseAlpha = TargetAlpha;
	}
	else
	{
		TransitionPulseAlpha = FMath::FInterpConstantTo(
			TransitionPulseAlpha,
			TargetAlpha,
			DeltaSeconds,
			1.0f / FadeSeconds);
	}

	if (!bTransitionPulseActive && TransitionPulseAlpha <= KINDA_SMALL_NUMBER)
	{
		TransitionPulseAlpha = 0.0f;
		RestoreTransitionPulseLightingChannels();
		UpdateTransitionPulseLights();
		SetActorTickEnabled(false);
	}
}
