// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Engine/DataTable.h"
#include "TankPawn.h"
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
	FString CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataTable* InventorySlotsTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FVector> TankVector;
	
};
