// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Tankogeddon/TestSaveGame.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "SaveManager.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnGameFromSlotAction, const FString&, SlotName, UDataTable*, InventorySlotsTable, FString, CurrentAmmo, FString, Health, TArray<AQuest*>, Quests);

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API USaveManager : public UObject
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void Init();
    UFUNCTION(BlueprintCallable)
    bool DoesSaveGameExist(const FString& SlotName);
    UFUNCTION(BlueprintCallable)
    void LoadGame(const FString& SlotName);
    UFUNCTION(BlueprintCallable)
    void SaveCurrentGame(const FString& SlotName);

    UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
    FOnGameFromSlotAction OnGameLoadedFromSlot;
    UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
    FOnGameFromSlotAction OnGameSavedToSlot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
    TSubclassOf<class ATankPawn> SpawnTankClass;

    UFUNCTION(BlueprintCallable)
    const TArray<FString>& GetExistingSavedSlots() const {
        return ExistingSavedSlots;
    }

protected:
    UPROPERTY(BlueprintReadWrite)
    UTestSaveGame* CurrentGameObject;

    void OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);
    void OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess);

    UPROPERTY()
    TArray<FString> ExistingSavedSlots;
    const FString ExistingSavedSlotsFilePath = "existing_slots.txt";
    const FString QuestCacheFilePath = "quest_cache.txt";

    void CacheExistingSavedSlotsInfo();
    void SerializeSlotsInfo(TArray<uint8>* RawDerivedData);
};
