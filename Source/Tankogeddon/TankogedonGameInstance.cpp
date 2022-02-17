// Fill out your copyright notice in the Description page of Project Settings.


#include "TankogedonGameInstance.h"

void UTankogedonGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Warning, TEXT("UTankogedonGameInstance::Init()"));

    SaveManager = NewObject<USaveManager>(this);
    SaveManager->Init();
}