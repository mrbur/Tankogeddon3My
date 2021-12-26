// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapWidgetStyle.h"

FMiniMapStyle::FMiniMapStyle()
{
	
}

FMiniMapStyle::~FMiniMapStyle()
{
}

const FName FMiniMapStyle::TypeName(TEXT("FMiniMapStyle"));

const FMiniMapStyle& FMiniMapStyle::GetDefault()
{
	static FMiniMapStyle Default;
	return Default;
}

void FMiniMapStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

