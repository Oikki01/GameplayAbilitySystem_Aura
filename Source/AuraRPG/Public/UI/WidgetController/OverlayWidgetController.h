#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;

/**Begin 定义动态多播委托 方便在蓝图中使用*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSingnature, float, Health);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSingnature, float, MaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSingnature, float, Mana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSingnature, float, MaxMana);

/**end*/

/**
 * overlaywidget所依赖的控制器
 * 设置蓝图类型意味着我们可以在事件图中把他当作一种类型使用，比如执行类型转换
 * 蓝图化意味着我们可以基于这个类创建蓝图
 */
UCLASS(BlueprintType, Blueprintable)
class AURARPG_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitailValues() override;

	virtual void BindCallBackToDependencies() override;

	/**Begin 定义委托对象*/
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnHealthChangedSingnature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxHealthChangedSingnature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnManaChangedSingnature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxManaChangedSingnature OnMaxManaChanged;
	/**End*/

protected:

	/**Begin 回调函数*/
	void HealthChanged(const FOnAttributeChangeData& Data) const;

	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;

	void ManaChanged(const FOnAttributeChangeData& Data) const;

	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
	/**End*/
};
