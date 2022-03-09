// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest.h"
#include "CurrentObjectives.h"
#include "QuestListComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUESTSYSTEM_API UQuestListComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestAction, AQuest*);

public:	
	void AddQuest(AQuest* Quest);

	UFUNCTION(BlueprintCallable)
	const TArray<AQuest*>& GetQuests();

	AQuest* GetActiveQuest() const;
	void SetActiveQuest(AQuest* Quest);

	UFUNCTION()
	void OnQuestAdd(AQuest* Quest);

	FOnQuestAction OnActiveQuestChanged;

	UPROPERTY()
	TArray<AQuest*> AcceptedQuests;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AQuest* ActiveQuest;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCurrentObjectives> CurrentObjectivesWidgetClass;
};
