

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 方便设置控件控制器Base中的核心变量
 * 声明为BlueprintType：如果需要在蓝图中用到这个结构体也没问题
 */
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,UAttributeSet* AS) : PlayerController(PC), PlayerState(PS),AbilitySystemComponent(ASC),AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};
/**
 * 负责从系统和Model中获取数据，并将它们广播到所有关联的Widget。这个类是个基类，这样所有WidgetController都能继承那四个核心属性。所以每一个控件都有一个对应的控制器或者多个控制器对应一个控件。
 */
UCLASS()
class AURARPG_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	//设置控件控制器中的参数
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

    //广播初始值
	virtual void BroadcastInitailValues();

    //绑定回调到依赖项
	virtual void BindCallBackToDependencies();
	
protected:
	//这些变量是用来获取所有信息和数据的
	//WidgetContorller的子类需要获取数据，或者订阅它们广播的委托
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	//HUD需要了解PlayerState上的重要信息
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
