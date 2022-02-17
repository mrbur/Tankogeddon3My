// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <Tankogeddon/SaveManager.h>
#include "TankogedonGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UTankogedonGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void Init() override;
	
	UPROPERTY(BlueprintReadOnly)
	USaveManager* SaveManager;

	UFUNCTION(BlueprintPure, meta = (FWorldContext = WorldContextObject))
	static USaveManager* GetSaveManager(UObject* WorldContextObject) {
		auto* GameInstance = Cast<UTankogedonGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		return GameInstance ? GameInstance->SaveManager : nullptr;
	}

};
