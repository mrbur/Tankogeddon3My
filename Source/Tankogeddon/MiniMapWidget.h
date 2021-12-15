// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/NativeWidgetHost.h>
#include "SMiniMapCompoundWidget.h"
#include "MiniMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UNativeWidgetHost* MiniMap;
	
};
