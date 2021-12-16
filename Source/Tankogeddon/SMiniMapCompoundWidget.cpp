// Fill out your copyright notice in the Description page of Project Settings.


#include "SMiniMapCompoundWidget.h"
#include "Widgets/SCanvas.h"
#include "Rendering/DrawElements.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
int32 SMiniMapCompoundWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    FSlateDrawElement::MakeLines(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(),
        LinePoints,
        ESlateDrawEffect::None,
        LineColor,
        bUseAntialias,
        LineThickness
    );
    return int32();
}

void SMiniMapCompoundWidget::Construct(const FArguments& InArgs)
{
    LinePoints.Add(FVector2D(0.0f, 0.0f));
    LinePoints.Add(FVector2D(0.0f, 300.0f));
    LinePoints.Add(FVector2D(300.0f, 300.0f));
    LinePoints.Add(FVector2D(300.0f, 0.0f));
    LinePoints.Add(FVector2D(0.0f, 0.0f));

    ChildSlot
        [
            SNew(SCanvas)
        ];
}

void SMiniMapCompoundWidget::SetTankPosition(FVector TankPosition)
{
    TankCurrentPosition = TankPosition;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION