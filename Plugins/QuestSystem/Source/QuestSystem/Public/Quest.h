// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Objective.h"
#include "Quest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestStatusUpdated, AQuest*, Quest );

UCLASS()
class QUESTSYSTEM_API AQuest : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AQuest();

    FOnQuestStatusUpdated OnQuestStatusUpdated;

    UPROPERTY(EditAnywhere)
    FText Name;
    UPROPERTY(EditAnywhere)
    FText Descrition;
    UPROPERTY(EditAnywhere)
    TArray<UObjective*> Objectives;
    UPROPERTY(EditAnywhere)
    bool bIsStoryQuest = true;
    UPROPERTY(EditAnywhere)
    bool bKeepObjectivesOrder = true;
    UPROPERTY(EditAnywhere)
    AQuest* PrerquisedQuest;
    UPROPERTY(VisibleAnywhere)
    bool bIsTaken;

    UFUNCTION(BlueprintCallable, CallInEditor)
    void UpdateLocation(); 
    UFUNCTION(BlueprintCallable, CallInEditor)
    void AddLocationObjective();
    UFUNCTION(BlueprintCallable, CallInEditor)
    void AddInteractObjective();
    UFUNCTION(BlueprintCallable, CallInEditor)
    void TakeQuest(AActor* Character);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnObjectiveCompleted(UObjective* Objective);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};