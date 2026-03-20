#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CaissonInteractComponent.generated.h"

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

	// 控制高亮表现，通常用于悬停或已完成状态。
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void SetHighlightFocus(bool bIsFocused);

private:
	// 鼠标悬停时的临时高亮。
	bool bHoverHighlighted;

	// 目标完成后的常驻高亮。
	bool bPersistentHighlighted;

	void RefreshHighlightState();

	void NotifyHoverStateChanged(bool bHovered);
};
