#include "CaissonGameViewportClient.h"

#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "HAL/IConsoleManager.h"
#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/SLeafWidget.h"

namespace
{
static TAutoConsoleVariable<int32> CVarCaissonDisplay16x9Debug(
	TEXT("caisson.Display16x9.Debug"),
	0,
	TEXT("Draw and log 16:9 viewport debug information. 0=off, 1=on."),
	ECVF_Default);

static TAutoConsoleVariable<int32> CVarCaissonDisplay16x9WrapUI(
	TEXT("caisson.Display16x9.WrapUI"),
	1,
	TEXT("Wrap viewport UI into a fixed 1920x1080 16:9 safe area. 0=off, 1=on."),
	ECVF_Default);

static TAutoConsoleVariable<int32> CVarCaissonDisplay16x9DrawBars(
	TEXT("caisson.Display16x9.DrawBars"),
	1,
	TEXT("Draw black letterbox/pillarbox bars outside the 16:9 safe area. 0=off, 1=on."),
	ECVF_Default);

FVector2D Calculate16By9ActiveSize(const FVector2D& ViewportSize)
{
	constexpr float TargetAspect = 16.0f / 9.0f;
	if (ViewportSize.X <= 0.0 || ViewportSize.Y <= 0.0)
	{
		return FVector2D::ZeroVector;
	}

	const float CurrentAspect = ViewportSize.X / ViewportSize.Y;
	if (CurrentAspect > TargetAspect)
	{
		return FVector2D(ViewportSize.Y * TargetAspect, ViewportSize.Y);
	}

	if (CurrentAspect < TargetAspect)
	{
		return FVector2D(ViewportSize.X, ViewportSize.X / TargetAspect);
	}

	return ViewportSize;
}

class SCaissonLetterboxOverlay final : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SCaissonLetterboxOverlay) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		SetVisibility(EVisibility::HitTestInvisible);
	}

	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override
	{
		return FVector2D::ZeroVector;
	}

	virtual int32 OnPaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override
	{
		const FVector2D ViewportSize = AllottedGeometry.GetLocalSize();
		if (ViewportSize.X <= 0.0 || ViewportSize.Y <= 0.0)
		{
			return LayerId;
		}

		const FVector2D ActiveSize = Calculate16By9ActiveSize(ViewportSize);
		const FVector2D ActiveOffset = (ViewportSize - ActiveSize) * 0.5f;

		if (CVarCaissonDisplay16x9Debug.GetValueOnGameThread() != 0)
		{
			UE_LOG(
				LogTemp,
				Log,
				TEXT("[Display16x9] Viewport=%s Active=%s Offset=%s DrawBars=%d WrapUI=%d"),
				*ViewportSize.ToString(),
				*ActiveSize.ToString(),
				*ActiveOffset.ToString(),
				CVarCaissonDisplay16x9DrawBars.GetValueOnGameThread(),
				CVarCaissonDisplay16x9WrapUI.GetValueOnGameThread());
		}

		if (CVarCaissonDisplay16x9DrawBars.GetValueOnGameThread() == 0)
		{
			return LayerId;
		}

		constexpr float TargetAspect = 16.0f / 9.0f;
		const float CurrentAspect = ViewportSize.X / ViewportSize.Y;
		const FSlateBrush* Brush = FCoreStyle::Get().GetBrush(TEXT("WhiteBrush"));
		const FLinearColor Black = FLinearColor::Black;
		int32 PaintLayer = LayerId;

		auto PaintRect = [&](const FVector2D& Position, const FVector2D& Size)
		{
			if (Size.X <= 0.0 || Size.Y <= 0.0)
			{
				return;
			}

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				PaintLayer,
				AllottedGeometry.ToPaintGeometry(Size, FSlateLayoutTransform(Position)),
				Brush,
				ESlateDrawEffect::None,
				Black);
		};

		if (CurrentAspect > TargetAspect)
		{
			const float ActiveWidth = ViewportSize.Y * TargetAspect;
			const float BarWidth = (ViewportSize.X - ActiveWidth) * 0.5f;
			PaintRect(FVector2D(0.0f, 0.0f), FVector2D(BarWidth, ViewportSize.Y));
			PaintRect(FVector2D(ViewportSize.X - BarWidth, 0.0f), FVector2D(BarWidth, ViewportSize.Y));
		}
		else if (CurrentAspect < TargetAspect)
		{
			const float ActiveHeight = ViewportSize.X / TargetAspect;
			const float BarHeight = (ViewportSize.Y - ActiveHeight) * 0.5f;
			PaintRect(FVector2D(0.0f, 0.0f), FVector2D(ViewportSize.X, BarHeight));
			PaintRect(FVector2D(0.0f, ViewportSize.Y - BarHeight), FVector2D(ViewportSize.X, BarHeight));
		}

		return PaintLayer + 1;
	}
};
}

void UCaissonGameViewportClient::Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);
	UE_LOG(LogTemp, Log, TEXT("[Display16x9] CaissonGameViewportClient initialized."));
	EnsureLetterboxOverlay();
}

void UCaissonGameViewportClient::AddViewportWidgetContent(TSharedRef<SWidget> ViewportContent, const int32 ZOrder)
{
	if (ViewportContent == LetterboxOverlayWidget)
	{
		Super::AddViewportWidgetContent(ViewportContent, ZOrder);
		return;
	}

	TSharedRef<SWidget> WrappedContent = WrapWidgetIn16By9SafeArea(ViewportContent);
	ViewportWidgetWrappers.Add(&ViewportContent.Get(), WrappedContent);
	UE_LOG(LogTemp, Verbose, TEXT("[Display16x9] AddViewportWidgetContent wrapped. ZOrder=%d"), ZOrder);
	Super::AddViewportWidgetContent(WrappedContent, ZOrder);
}

void UCaissonGameViewportClient::RemoveViewportWidgetContent(TSharedRef<SWidget> ViewportContent)
{
	if (TSharedPtr<SWidget> WrappedContent = ViewportWidgetWrappers.FindRef(&ViewportContent.Get()))
	{
		Super::RemoveViewportWidgetContent(WrappedContent.ToSharedRef());
		ViewportWidgetWrappers.Remove(&ViewportContent.Get());
		return;
	}

	Super::RemoveViewportWidgetContent(ViewportContent);
}

void UCaissonGameViewportClient::AddViewportWidgetForPlayer(ULocalPlayer* Player, TSharedRef<SWidget> ViewportContent, const int32 ZOrder)
{
	TSharedRef<SWidget> WrappedContent = WrapWidgetIn16By9SafeArea(ViewportContent);
	PlayerWidgetWrappers.Add(&ViewportContent.Get(), WrappedContent);
	UE_LOG(LogTemp, Verbose, TEXT("[Display16x9] AddViewportWidgetForPlayer wrapped. ZOrder=%d Player=%s"), ZOrder, *GetNameSafe(Player));
	Super::AddViewportWidgetForPlayer(Player, WrappedContent, ZOrder);
}

void UCaissonGameViewportClient::RemoveViewportWidgetForPlayer(ULocalPlayer* Player, TSharedRef<SWidget> ViewportContent)
{
	if (TSharedPtr<SWidget> WrappedContent = PlayerWidgetWrappers.FindRef(&ViewportContent.Get()))
	{
		Super::RemoveViewportWidgetForPlayer(Player, WrappedContent.ToSharedRef());
		PlayerWidgetWrappers.Remove(&ViewportContent.Get());
		return;
	}

	Super::RemoveViewportWidgetForPlayer(Player, ViewportContent);
}

TSharedRef<SWidget> UCaissonGameViewportClient::WrapWidgetIn16By9SafeArea(TSharedRef<SWidget> ViewportContent)
{
	if (CVarCaissonDisplay16x9WrapUI.GetValueOnGameThread() == 0)
	{
		return ViewportContent;
	}

	return SNew(SBox)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SScaleBox)
			.Stretch(EStretch::ScaleToFit)
			.StretchDirection(EStretchDirection::Both)
			[
				SNew(SBox)
				.WidthOverride(1920.0f)
				.HeightOverride(1080.0f)
				[
					ViewportContent
				]
			]
		];
}

void UCaissonGameViewportClient::EnsureLetterboxOverlay()
{
	if (LetterboxOverlayWidget.IsValid())
	{
		return;
	}

	LetterboxOverlayWidget = SNew(SCaissonLetterboxOverlay);
	Super::AddViewportWidgetContent(LetterboxOverlayWidget.ToSharedRef(), TNumericLimits<int32>::Max());
}
