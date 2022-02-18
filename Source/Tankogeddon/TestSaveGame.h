// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Engine/DataTable.h"
#include "TestSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UTestSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int CurrentAmmo = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CurrentHealth = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataTable* InventorySlotsTable;
	
};
