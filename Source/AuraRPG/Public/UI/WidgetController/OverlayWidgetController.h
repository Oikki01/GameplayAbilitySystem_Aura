#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Templates/SubclassOf.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


class UTexture2D;
/**
 * 新建结构体继承TableRowBase 可以创建数据行表格 用于检索数据查询需要用到的数据（也可以使用TMAP来让标签和结构体对应 但是建议使用数据表格）(对于数据表来说，最好的实现是用数据资产 )
 */
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	/**
	 * 消息标签
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MessageTag = FGameplayTag();

	/**
	 * 消息文本
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText MessageText = FText();

	/**
	 * 需要生成的UIWidget
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UAuraUserWidget> MessageUIWidget;

	/**
	 * 消息显示的图片
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* MessageTexture = nullptr;
};

class UAuraUserWidget;
struct FOnAttributeChangeData;

/**Begin 定义动态多播委托 方便在蓝图中使用*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSingnature, float, Health);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSingnature, float, MaxHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSingnature, float, Mana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSingnature, float, MaxMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageUIWidgetSingnature, FUIWidgetRow, Row);

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

	UPROPERTY(BlueprintAssignable, Category = "GAS|Message")
	FOnMessageUIWidgetSingnature OnMessageUIWidget;
	/**End*/

protected:

	/**Begin 回调函数*/
	void HealthChanged(const FOnAttributeChangeData& Data) const;

	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;

	void ManaChanged(const FOnAttributeChangeData& Data) const;

	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
	/**End*/

	/**
	 * 消息数据表格
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Widget Data")
	TObjectPtr<UDataTable> MessageDataTable;

	template<typename T>
	T* GetDataTableByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
