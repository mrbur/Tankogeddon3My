// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapWidget.h"
#include "FStyleSet.h"


void UMiniMapWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UMiniMapWidget::NativePreConstruct()
{
    WidgetStyle = FStyleSet::Get().GetWidgetStyle<FMiniMapStyle>("MiniMapStyle");

    TSharedRef<SMiniMapCompoundWidget> SlateWidget = SNew(SMiniMapCompoundWidget).style(&WidgetStyle);
    SMiniMapCompoundWidget& minimapSlateWidget = SlateWidget.Get();
    miniMapCompoundWidget = &minimapSlateWidget;

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
