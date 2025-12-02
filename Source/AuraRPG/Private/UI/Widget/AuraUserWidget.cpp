


#include "UI/Widget/AuraUserWidget.h"

#include "Engine/Engine.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
