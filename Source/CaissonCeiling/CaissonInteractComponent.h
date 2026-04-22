#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CaissonInteractComponent.generated.h"

class UPointLightComponent;
class UPrimitiveComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCaissonInteractSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCaissonHoverSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CAISSONCEILING_API UCaissonInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCaissonInteractComponent();

	// 交互对象显示名称，供 UI 或调试使用。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FString InteractName;

	// 交互对象唯一 ID，供关卡逻辑判断使用，例如 "TiangongTower"。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	FName InteractionId;

	// 当前是否允许被交互。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bInteractionEnabled;

	// 是否启用代码驱动的呼吸灯，不再依赖单个蓝图里手摆的高亮灯。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse")
	bool bEnablePulseLights;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse", meta=(ClampMin="1", ClampMax="6"))
	int32 PulseLightCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse", meta=(ClampMin="0.1"))
	float PulseSpeed;

	// 呼吸灯总亮度倍率。优先调这个值，1.0 为原始强度。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse", meta=(ClampMin="0.0", ClampMax="2.0"))
	float PulseIntensityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse", meta=(ClampMin="0.0"))
	float HoverPulseMaxIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse", meta=(ClampMin="0.0"))
	float ActivatedPulseMaxIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse", meta=(ClampMin="0.1"))
	float PulseMinBrightnessRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse", meta=(ClampMin="0.1"))
	float PulseRangeScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse", meta=(ClampMin="0.0"))
	float PulseBoundsPadding;

	// 代码呼吸灯使用的自定义光照通道。1/2 分别对应 UE 的 Custom Channel 1/2。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse", meta=(ClampMin="1", ClampMax="2"))
	int32 PulseLightingChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse")
	FLinearColor HoverPulseColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction|Pulse")
	FLinearColor ActivatedPulseColor;

	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnCaissonInteractSignature OnInteractClicked;

	// 鼠标移入该对象时触发，供蓝图做悬停高亮。
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnCaissonHoverSignature OnHoverBegin;

	// 鼠标移出该对象时触发，供蓝图取消悬停高亮。
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnCaissonHoverSignature OnHoverEnd;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 对外提供的点击入口，只有在可交互时才会真正触发事件。
	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool TryInteract();

	// 动态开关交互能力，便于完成后锁定目标。
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void SetInteractionEnabled(bool bEnabled);

	// 设置悬停高亮状态。
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void SetHoverHighlight(bool bEnabled);

	// 设置完成后的常亮状态。
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void SetPersistentHighlight(bool bEnabled);

	UFUNCTION(BlueprintPure, Category="Interaction")
	bool IsPersistentHighlightEnabled() const;

	// 控制高亮表现，通常用于悬停或已完成状态。
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void SetHighlightFocus(bool bIsFocused);

	// 将呼吸灯和高亮范围锁定到当前命中的 Actor，避免只亮局部小组件。
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void SetHighlightTarget(UPrimitiveComponent* HitComponent, AActor* HitActor);

	UFUNCTION(BlueprintCallable, Category="Interaction")
	void ClearHighlightTarget();

private:
	// 鼠标悬停时的临时高亮。
	bool bHoverHighlighted;

	// 目标完成后的常驻高亮。
	bool bPersistentHighlighted;

	float PulseTime;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UPointLightComponent>> PulseLightComponents;

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> HighlightTargetActor;

	struct FPrimitiveLightingChannelState
	{
		TWeakObjectPtr<UPrimitiveComponent> PrimitiveComponent;
		bool bChannel0 = true;
		bool bChannel1 = false;
		bool bChannel2 = false;
	};

	TArray<FPrimitiveLightingChannelState> OverriddenLightingChannelStates;

	void RefreshHighlightState();
	void EnsurePulseLights();
	void RepositionPulseLights();
	void UpdatePulseLights();
	void UpdatePulseLightingChannels();
	void RestorePulseLightingChannels();
	void GetPulseLightingChannelMask(bool& bOutChannel0, bool& bOutChannel1, bool& bOutChannel2) const;
	void CollectHighlightPrimitiveComponents(TArray<UPrimitiveComponent*>& OutComponents) const;

	void NotifyHoverStateChanged(bool bHovered);
};
