// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapWidget.h"


void UMiniMapWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UMiniMapWidget::NativePreConstruct()
{
    TSharedRef<SMiniMapCompoundWidget> 
        SlateWidget = SNew(SMiniMapCompoundWidget);
    SMiniMapCompoundWidget& f = SlateWidget.Get();
    miniMapCompoundWidget = &f;

    if (MiniMap)
    {
        MiniMap->SetContent(
            SlateWidget
        );
    }
}

void UMiniMapWidget::SetTankPositionOnMiniMap(FVector position)
{
    miniMapCompoundWidget->SetTankPosition(position);
}
