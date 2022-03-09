// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManager.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "TankPawn.h"
#include "Engine/World.h"
#include "Serialization/NameAsStringProxyArchive.h"
#include <Runtime/CoreUObject/Public/Serialization/ObjectAndNameAsStringProxyArchive.h>
#include "Kismet/GameplayStatics.h"

void USaveManager::Init()
{
    CurrentGameObject = Cast<UTestSaveGame>(UGameplayStatics::CreateSaveGameObject(
        UTestSaveGame::StaticClass()));

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    ATankPawn* TankPawn = Cast<ATankPawn>(PlayerPawn);
    OnGameLoadedFromSlot.AddDynamic(TankPawn, &ATankPawn::OnGameLoaded);


    ExistingSavedSlots.Empty();
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    FString FilePath = FPaths::Combine(FPaths::ProjectContentDir(), ExistingSavedSlotsFilePath);
    if (PlatformFile.FileExists(*FilePath))
    {
        FString ExistingSavingsArray;
        if (FFileHelper::LoadFileToString(ExistingSavingsArray, *FilePath))
        {
            ExistingSavingsArray.ParseIntoArray(ExistingSavedSlots, TEXT(","));
        }
    }
}

bool USaveManager::DoesSaveGameExist(const FString& SlotName)
{
    return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void USaveManager::LoadGame(const FString& SlotName)
{

    if (!GetExistingSavedSlots().Contains(SlotName)) {
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

    for (FVector TankSpawnPoint : CurrentGameObject->TankVector) {
        FTransform SpawnTransform(FRotator::ZeroRotator, TankSpawnPoint, FVector(1.f));
        AActor* MySpawnedActor = GetWorld()->SpawnActor<ATankPawn>(ATankPawn::StaticClass(), TankSpawnPoint, FRotator::ZeroRotator);
    }

    FString QuestFilePath = FPaths::Combine(FPaths::ProjectContentDir(),
        QuestCacheFilePath);
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    IFileHandle* FileHandle = PlatformFile.OpenRead(*QuestFilePath);

    int32 QuestCount = 0;

    TArray<uint8> Contents;
    Contents.AddUninitialized(FileHandle->Size());
    if (FileHandle)
    {
        int32* IntPointer = &QuestCount;
        uint8* ByteBuffer = reinterpret_cast<uint8*>(IntPointer);
        FileHandle->Read(ByteBuffer, sizeof(int32));
        
        FileHandle->Read(Contents.GetData(), Contents.Num());

        delete FileHandle;
    }

    if (QuestCount == 0)return;

    CurrentGameObject->Quests.Empty();

    FMemoryReader MemReader(Contents);
    FObjectAndNameAsStringProxyArchive Arr(MemReader, true);

    while (QuestCount > 0) {
        AQuest* Quest = NewObject<AQuest>(this);
        Arr.ArIsSaveGame = true;
        Quest->Serialize(Arr); 
        Arr << Quest;
        Arr.Flush();

        CurrentGameObject->Quests.Add(Quest);
        QuestCount--;
    }

    CurrentGameObject = Cast<UTestSaveGame>(SaveGame);
    if (OnGameLoadedFromSlot.IsBound())
    {
        OnGameLoadedFromSlot.Broadcast(SlotName, CurrentGameObject->InventorySlotsTable, CurrentGameObject->CurrentAmmo, CurrentGameObject->Health, CurrentGameObject->Quests);
    }
}

void USaveManager::OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
    if (OnGameSavedToSlot.IsBound())
    {
        //OnGameSavedToSlot.Broadcast(SlotName);
    }
    if (!ExistingSavedSlots.Contains(SlotName))
    {
        ExistingSavedSlots.Add(SlotName);
        CacheExistingSavedSlotsInfo();
    }
}

struct FWCSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
    FWCSaveGameArchive(FArchive& InInnerArchive)
        : FObjectAndNameAsStringProxyArchive(InInnerArchive, true)
    {
        ArIsSaveGame = true;
        ArNoDelta = true; // Optional, useful when saving/loading variables without resetting the level.
                          // Serialize variables even if weren't modified and mantain their default values.
    }
};

void USaveManager::SerializeSlotsInfo(TArray<uint8>* RawDerivedData) {
    if (RawDerivedData == nullptr || CurrentGameObject->Quests.Num() == 0)return;

    FMemoryWriter MemWriter(*RawDerivedData);
    FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
    Ar.ArIsSaveGame = true;

    int32 MyInteger = CurrentGameObject->Quests.Num();
    for (AQuest* Quest : CurrentGameObject->Quests) {
        Ar << MyInteger;

        Quest->Serialize(Ar);
        Ar << Quest;
    }
}

void USaveManager::CacheExistingSavedSlotsInfo()
{
    FString FilePath = FPaths::Combine(FPaths::ProjectContentDir(),
        ExistingSavedSlotsFilePath);

    FString ExistingSavingsArray = "";
    for (const FString& Slot : ExistingSavedSlots)
    {
        ExistingSavingsArray += Slot + ",";
    }

    FFileHelper::SaveStringToFile(ExistingSavingsArray, *FilePath,
        FFileHelper::EEncodingOptions::ForceUnicode, &IFileManager::Get(),
        FILEWRITE_EvenIfReadOnly);

    TArray<uint8> RawDerivedData;
    SerializeSlotsInfo(&RawDerivedData);

    FString QuestFilePath = FPaths::Combine(FPaths::ProjectContentDir(),
        QuestCacheFilePath);
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    IFileHandle* FileHandle = PlatformFile.OpenWrite(*QuestFilePath);
    if (FileHandle)
    {
        FileHandle->Write(RawDerivedData.GetData(), RawDerivedData.Num() * sizeof(uint8));
        delete FileHandle;
    }
}