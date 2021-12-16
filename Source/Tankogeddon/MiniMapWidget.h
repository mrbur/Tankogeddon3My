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
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetTankPositionOnMiniMap(FVector position);

protected:
	SMiniMapCompoundWidget* miniMapCompoundWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional), Category = "Minimap")
	UNativeWidgetHost* MiniMap;
};
