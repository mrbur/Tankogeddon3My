// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Runtime/UMG/Public/Components/VerticalBox.h>
#include "ObjectiveWidget.h"
#include "CurrentObjectives.generated.h"


/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UCurrentObjectives : public UUserWidget {
	GENERATED_BODY() 
public:
    UFUNCTION()
    void SetCurrentObjectives(AQuest* Quest);

protected:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UObjectiveWidget> ObjectiveWidgetClass;

    UPROPERTY(meta = (BindWidgetOptional))
    UVerticalBox* ObjectivesList;
};
