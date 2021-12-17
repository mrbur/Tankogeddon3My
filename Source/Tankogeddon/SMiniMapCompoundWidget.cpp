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
        BorderLinePoints,
        ESlateDrawEffect::None,
        BorderLineColor,
        bUseAntialias,
        LineThickness
    );

    FSlateDrawElement::MakeLines(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(),
        TankLinePoints,
        ESlateDrawEffect::None,
        TankColor,
        bUseAntialias,
        LineThickness
    );
    return int32();
}

void SMiniMapCompoundWidget::Construct(const FArguments& InArgs)
{
    BorderLinePoints.Add(FVector2D(0.0f, 0.0f));
    BorderLinePoints.Add(FVector2D(500.0f, 0.0f));
    BorderLinePoints.Add(FVector2D(500.0f, 500.0f));
    BorderLinePoints.Add(FVector2D(0.0f, 500.0f));
    BorderLinePoints.Add(FVector2D(0.0f, 0.0f));

    ChildSlot
        [
            SNew(SCanvas)
        ];
}

void SMiniMapCompoundWidget::SetTankPosition(FVector TankPosition)
{
    TankLinePoints.Empty();

    FVector2D Tank2DPosition = ConvertWorldToMiniMapPosition(TankPosition);

    TankLinePoints.Add(FVector2D(Tank2DPosition.X, Tank2DPosition.Y));
    TankLinePoints.Add(FVector2D(Tank2DPosition.X, Tank2DPosition.Y - 4));
    TankLinePoints.Add(FVector2D(Tank2DPosition.X - 4, Tank2DPosition.Y - 4));
    TankLinePoints.Add(FVector2D(Tank2DPosition.X - 4, Tank2DPosition.Y));
    TankLinePoints.Add(FVector2D(Tank2DPosition.X, Tank2DPosition.Y));
}

FVector2D SMiniMapCompoundWidget::ConvertWorldToMiniMapPosition(FVector TankPosition)
{
    FVector2D Tank2DPosition(TankPosition.X + MapXShift, TankPosition.Y + MapYShift);
    Tank2DPosition.X = (Tank2DPosition.X / MapXSize) * MiniMapXSize;
    Tank2DPosition.Y = (Tank2DPosition.Y / MapYSize) * MiniMapYSize;

    return FVector2D(Tank2DPosition.Y, MiniMapYSize - Tank2DPosition.X);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION