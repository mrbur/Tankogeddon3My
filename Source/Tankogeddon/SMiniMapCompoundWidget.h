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
	SLATE_BEGIN_ARGS(SMiniMapCompoundWidget)
	{}
	SLATE_ARGUMENT(EHorizontalAlignment, HAlign)
	SLATE_ARGUMENT(FSlateBrush*, Background)
	SLATE_END_ARGS()
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	UPROPERTY()
	UUserWidget minimap;
};
