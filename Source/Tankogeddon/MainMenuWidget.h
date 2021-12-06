// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "MainMenuWidget.generated.h"


/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
	FName LoadLevelName;

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	UButton* NewButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* QuitButton;

	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnQuitGameClicked();
	
};
