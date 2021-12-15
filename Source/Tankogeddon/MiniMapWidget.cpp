// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapWidget.h"


void UMiniMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (MiniMap)
    {
        MiniMap->SetContent(
            SNew(SMiniMapCompoundWidget)
        );
    }
}