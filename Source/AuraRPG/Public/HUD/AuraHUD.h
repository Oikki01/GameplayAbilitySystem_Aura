

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeSet;
class UAuraUserWidget;
class UOverlayWidgetController;
class APlayerState;
class UAbilitySystemComponent;




struct FWidgetControllerParams;
/**
 * 主要是用来创建Widget和widgetController
 */
UCLASS()
class AURARPG_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:

	//用来存储空间类的变量
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	//检测OverlayWidegtController是否已创建，如果没有创建则生成，如果创建了则返回。我们始终只有一个控制器
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	//初始化Overlay：负责创建控件控制器、控件，设置控件的控制器并把控件添加到界面上
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
private:
	/**begin 具体的控件及控件的控制器 TSubclassOf在编辑器里指定一个可以生成的蓝图类*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	//存储OverlayWidgetController的指针
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	//在蓝图中设置OverlayWidgetController类
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	/**end*/
};
