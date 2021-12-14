// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "GameOverUIHud.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API AGameOverUIHud : public AHUD
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Widget")
	UUserWidget* ShowWidget();

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void HideWidget();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> userWidgetClass;

	UPROPERTY()
	UUserWidget* deathWidget;

	
};
