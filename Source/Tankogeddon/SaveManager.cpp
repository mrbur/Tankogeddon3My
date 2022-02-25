// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManager.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "TankPawn.h"
#include "Engine/World.h"

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
    CurrentGameObject->TankVector.Empty();
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATankPawn::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors) {
        ATankPawn* TankPawn = Cast<ATankPawn>(Actor);
        if (TankPawn && !TankPawn->IsPlayer()) {
            CurrentGameObject->TankVector.Add(TankPawn->GetActorLocation());
        }
    }

    UGameplayStatics::AsyncSaveGameToSlot(CurrentGameObject, SlotName, 0,
        FAsyncSaveGameToSlotDelegate::CreateUObject(this,
            &USaveManager::OnGameSavedToSlotHandle));
}

void USaveManager::OnGameLoadedFromSlotHandle(const FString& SlotName,
    const int32 UserIndex, USaveGame* SaveGame)
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATankPawn::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors) {
        ATankPawn* TankPawn = Cast<ATankPawn>(Actor);
        if (TankPawn && !TankPawn->IsPlayer()) {
            TankPawn->Destroy();
        }
    }

    CurrentGameObject = Cast<UTestSaveGame>(SaveGame);
    if (OnGameLoadedFromSlot.IsBound())
    {
        OnGameLoadedFromSlot.Broadcast(SlotName, CurrentGameObject->InventorySlotsTable, CurrentGameObject->CurrentAmmo, CurrentGameObject->Health);
    }

    for (FVector TankSpawnPoint : CurrentGameObject->TankVector) {
        FTransform SpawnTransform(FRotator::ZeroRotator, TankSpawnPoint, FVector(1.f));
        AActor* MySpawnedActor = GetWorld()->SpawnActor<ATankPawn>(ATankPawn::StaticClass(), TankSpawnPoint, FRotator::ZeroRotator);
    }
}

void USaveManager::OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
    if (OnGameSavedToSlot.IsBound())
    {
        //OnGameSavedToSlot.Broadcast(SlotName);
    }
}