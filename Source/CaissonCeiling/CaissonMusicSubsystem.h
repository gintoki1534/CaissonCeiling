#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CaissonMusicSubsystem.generated.h"

class UAudioComponent;
class USoundBase;

UCLASS()
class CAISSONCEILING_API UCaissonMusicSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category="Audio|BGM")
	void PlayGlobalBgm(const UObject* WorldContextObject, USoundBase* Sound, float Volume = 0.6f, float FadeInSeconds = 2.0f);

	UFUNCTION(BlueprintCallable, Category="Audio|BGM")
	void StopGlobalBgm(float FadeOutSeconds = 1.0f);

	UFUNCTION(BlueprintCallable, Category="Audio|BGM")
	void SetGlobalBgmVolume(float Volume);

	UFUNCTION(BlueprintPure, Category="Audio|BGM")
	bool IsGlobalBgmPlaying() const;

private:
	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> GlobalBgmComponent;

	UPROPERTY(Transient)
	TObjectPtr<USoundBase> CurrentGlobalBgm;
};
