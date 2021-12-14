// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUIHud.h"

UUserWidget* AGameOverUIHud::ShowWidget()
{
    if (userWidgetClass) {
        HideWidget();
        deathWidget = CreateWidget(GetWorld(), userWidgetClass);
    }
    return deathWidget;
}

void AGameOverUIHud::HideWidget()
{
    if (deathWidget) {
        deathWidget->RemoveFromParent();
        deathWidget = nullptr;
    }
}
