// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	SLATE_ATTRIBUTE(FSlateColor, MyRadioButtons)
	SLATE_END_ARGS()
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void SetTankPosition(FVector TankPosition);

	FVector2D ConvertWorldToMiniMapPosition(FVector TankPosition);

protected:
	TArray<FVector2D> BorderLinePoints;
	TArray<FVector2D> TankLinePoints;
	FLinearColor BorderLineColor = FLinearColor::Black;
	FLinearColor TankColor = FLinearColor::Red;
	float LineThickness = 4.0f;
	bool bUseAntialias = true;

	int MapXSize = 5000;
	int MapYSize = 5000;
	int MapXShift = 1500;
	int MapYShift = 2500;

	int MiniMapXSize = 500;
	int MiniMapYSize = 500;

	FVector TankCurrentPosition;
};
