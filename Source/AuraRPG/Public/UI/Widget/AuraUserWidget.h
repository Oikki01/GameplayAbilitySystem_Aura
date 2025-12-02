#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

struct FUIWidgetRow;
/**
 * 
 */
UCLASS()
class AURARPG_API UAuraUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetWidgetController(UObject* InWidgetController);
public:
    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UObject>WidgetController;

protected:
    UFUNCTION(BlueprintImplementableEvent)//函数能在蓝图中实现
    void WidgetControllerSet();//设置控制层完成之后触发
};