


#include "HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		//NewObject用来创建UObeject的 Outer
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		//为控制器设置需要获取数据和属性的核心变量
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		//绑定回调到依赖项
		OverlayWidgetController->BindCallBackToDependencies();

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("OverlayWidgetClass未初始化，请在BP_AuraHUD中设置"))
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass未初始化，请在BP_AuraHUD中设置"))

	//通过HUD把控件添加
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	//需要将Widget转换为OverlayWidget
	UAuraUserWidget* Overlay_Widget = Cast<UAuraUserWidget>(Widget);
	
	//FWidgetControllerParams WCParams = FWidgetControllerParams(PC, PS, ASC, AS);
	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController =  GetOverlayWidgetController(WCParams);
	//为控件添加控制器
	Overlay_Widget->SetWidgetController(WidgetController);

	//广播为控件初始化属性
	WidgetController->BroadcastInitailValues();
	
	//显示到视口上
	Widget->AddToViewport();
	
	
	
}
