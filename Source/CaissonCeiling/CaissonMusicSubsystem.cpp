#include "CaissonMusicSubsystem.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

void UCaissonMusicSubsystem::Deinitialize()
{
	StopGlobalBgm(0.0f);
	Super::Deinitialize();
}

void UCaissonMusicSubsystem::PlayGlobalBgm(const UObject* WorldContextObject, USoundBase* Sound, float Volume, float FadeInSeconds)
{
	if (!WorldContextObject || !Sound)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BGM] PlayGlobalBgm failed: WorldContextObject=%s, Sound=%s"), *GetNameSafe(WorldContextObject), *GetNameSafe(Sound));
		return;
	}

	const float ClampedVolume = FMath::Max(0.0f, Volume);
	const float ClampedFadeInSeconds = FMath::Max(0.0f, FadeInSeconds);

	if (GlobalBgmComponent && CurrentGlobalBgm == Sound)
	{
		UE_LOG(LogTemp, Log, TEXT("[BGM] Reusing existing global BGM: %s"), *GetNameSafe(Sound));
		GlobalBgmComponent->SetVolumeMultiplier(ClampedVolume);
		if (!GlobalBgmComponent->IsPlaying())
		{
			if (ClampedFadeInSeconds > 0.0f)
			{
				GlobalBgmComponent->FadeIn(ClampedFadeInSeconds, ClampedVolume);
			}
			else
			{
				GlobalBgmComponent->Play();
			}
		}
		return;
	}

	if (GlobalBgmComponent)
	{
		StopGlobalBgm(0.0f);
	}

	GlobalBgmComponent = UGameplayStatics::CreateSound2D(
		WorldContextObject,
		Sound,
		ClampedVolume,
		1.0f,
		0.0f,
		nullptr,
		true,
		false);

	CurrentGlobalBgm = Sound;

	if (GlobalBgmComponent && ClampedFadeInSeconds > 0.0f)
	{
		GlobalBgmComponent->FadeIn(ClampedFadeInSeconds, ClampedVolume);
	}
	else if (GlobalBgmComponent)
	{
		GlobalBgmComponent->Play();
	}

	UE_LOG(LogTemp, Log, TEXT("[BGM] Spawn global BGM: Sound=%s, Component=%s, Volume=%.2f, FadeIn=%.2f"),
		*GetNameSafe(Sound),
		*GetNameSafe(GlobalBgmComponent),
		ClampedVolume,
		ClampedFadeInSeconds);
}

void UCaissonMusicSubsystem::StopGlobalBgm(float FadeOutSeconds)
{
	if (!GlobalBgmComponent)
	{
		CurrentGlobalBgm = nullptr;
		return;
	}

	const float ClampedFadeOutSeconds = FMath::Max(0.0f, FadeOutSeconds);
	if (ClampedFadeOutSeconds > 0.0f && GlobalBgmComponent->IsPlaying())
	{
		GlobalBgmComponent->FadeOut(ClampedFadeOutSeconds, 0.0f);
	}
	else
	{
		GlobalBgmComponent->Stop();
	}

	GlobalBgmComponent = nullptr;
	CurrentGlobalBgm = nullptr;
}

void UCaissonMusicSubsystem::SetGlobalBgmVolume(float Volume)
{
	if (GlobalBgmComponent)
	{
		GlobalBgmComponent->SetVolumeMultiplier(FMath::Max(0.0f, Volume));
	}
}

bool UCaissonMusicSubsystem::IsGlobalBgmPlaying() const
{
	return GlobalBgmComponent && GlobalBgmComponent->IsPlaying();
}
