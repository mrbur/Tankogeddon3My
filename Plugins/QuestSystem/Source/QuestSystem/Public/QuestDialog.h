// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "QuestDescription.h"
#include "QuestDialog.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestDialog : public UUserWidget
{
	GENERATED_BODY()

public:
    void NativeConstruct() override;

    void Init(class AQuest* Quest);

    FSimpleDelegate OnQuestAccepted;
    FSimpleDelegate OnQuestQuited; 

protected:
    UPROPERTY(meta = (BindWidgetOptional))
    UQuestDescription* Description;
    UPROPERTY(meta = (BindWidgetOptional))
    UButton* RejectButton;
    UPROPERTY(meta = (BindWidgetOptional))
    UButton* AcceptButton;

    UFUNCTION()
    void RejectQuest();
    UFUNCTION()
    void AcceptQuest();

    void HideDialog();

	
};
