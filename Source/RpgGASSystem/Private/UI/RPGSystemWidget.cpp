// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPGSystemWidget.h"

void URPGSystemWidget::SetWidgetController(UWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
