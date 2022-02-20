// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManager.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "TankPawn.h"

void USaveManager::Init()
{
    CurrentGameObject = Cast<UTestSaveGame>(UGameplayStatics::CreateSaveGameObject(
        UTestSaveGame::StaticClass()));

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    ATankPawn* TankPawn = Cast<ATankPawn>(PlayerPawn);
    OnGameLoadedFromSlot.AddDynamic(TankPawn, &ATankPawn::OnGameLoaded);
}

bool USaveManager::DoesSaveGameExist(const FString& SlotName)
{
    return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void USaveManager::LoadGame(const FString& SlotName)
{
    if (!DoesSaveGameExist(SlotName)) {
        return;
    }

    UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0,
        FAsyncLoadGameFromSlotDelegate::CreateUObject(this,
            &USaveManager::OnGameLoadedFromSlotHandle));
}

void USaveManager::SaveCurrentGame(const FString& SlotName)
{
    UGameplayStatics::AsyncSaveGameToSlot(CurrentGameObject, SlotName, 0,
        FAsyncSaveGameToSlotDelegate::CreateUObject(this,
            &USaveManager::OnGameSavedToSlotHandle));
}

void USaveManager::OnGameLoadedFromSlotHandle(const FString& SlotName,
    const int32 UserIndex, USaveGame* SaveGame)
{
    CurrentGameObject = Cast<UTestSaveGame>(SaveGame);
    if (OnGameLoadedFromSlot.IsBound())
    {
        OnGameLoadedFromSlot.Broadcast(SlotName, CurrentGameObject->InventorySlotsTable);
    }
}

void USaveManager::OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
    if (OnGameSavedToSlot.IsBound())
    {
        //OnGameSavedToSlot.Broadcast(SlotName);
    }
}