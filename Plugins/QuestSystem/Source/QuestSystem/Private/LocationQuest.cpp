// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationQuest.h"
#include "QuestSystemCharacter.h"
#include <Engine.h>

ALocationQuest::ALocationQuest() {
	TArray<AActor*> Characters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuestSystemCharacter::StaticClass(), Characters);
	
	if (Characters.Num() == 0)return;

	ULocationObjective* LocationObjective = CreateDefaultSubobject<ULocationObjective>(TEXT("Location objective"));
	LocationObjective->Descrition = FText::FromString(FString("Good location objective"));
	LocationObjective->Marker = Characters[0];
	Objectives.Add(LocationObjective);
}