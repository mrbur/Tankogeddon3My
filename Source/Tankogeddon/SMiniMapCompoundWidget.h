// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Math/Color.h"
#include "MiniMapWidgetStyle.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class TANKOGEDDON_API SMiniMapCompoundWidget : public SCompoundWidget
{
public:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	SLATE_BEGIN_ARGS(SMiniMapCompoundWidget)
	{}
	SLATE_STYLE_ARGUMENT(FMiniMapStyle, style)
	SLATE_END_ARGS()
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void SetTankPosition(FVector TankPosition);

	FVector2D ConvertWorldToMiniMapPosition(FVector TankPosition);

protected:
	TArray<FVector2D> BorderLinePoints;
	TArray<FVector2D> TankLinePoints;

	float LineThickness = 4.0f;
	bool bUseAntialias = true;

	int MapXSize = 5000;
	int MapYSize = 5000;
	int MapXShift = 1500;
	int MapYShift = 2500;

	int MiniMapXSize = 500;
	int MiniMapYSize = 500;

	FVector TankCurrentPosition;

	const FTextBlockStyle* MiniMapTextStyle = nullptr;
	const FLinearColor* BorderLineColor = nullptr;
	const FLinearColor* TankColor = nullptr;

};
