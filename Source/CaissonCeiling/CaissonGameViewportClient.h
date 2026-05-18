#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "CaissonGameViewportClient.generated.h"

/**
 * Project-level viewport policy: keep the playable/rendered area at 16:9 and
 * cover the remaining area with black letterbox bars.
 */
UCLASS(Transient)
class CAISSONCEILING_API UCaissonGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

public:
	virtual void Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
	virtual void AddViewportWidgetContent(TSharedRef<SWidget> ViewportContent, const int32 ZOrder = 0) override;
	virtual void RemoveViewportWidgetContent(TSharedRef<SWidget> ViewportContent) override;
	virtual void AddViewportWidgetForPlayer(ULocalPlayer* Player, TSharedRef<SWidget> ViewportContent, const int32 ZOrder) override;
	virtual void RemoveViewportWidgetForPlayer(ULocalPlayer* Player, TSharedRef<SWidget> ViewportContent) override;

private:
	TSharedPtr<SWidget> LetterboxOverlayWidget;
	TMap<const SWidget*, TSharedPtr<SWidget>> ViewportWidgetWrappers;
	TMap<const SWidget*, TSharedPtr<SWidget>> PlayerWidgetWrappers;

	static TSharedRef<SWidget> WrapWidgetIn16By9SafeArea(TSharedRef<SWidget> ViewportContent);
	void EnsureLetterboxOverlay();
};
