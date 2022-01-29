// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractableObject.h"
#include "QuestDialog.h"
#include "QuestList.h"
#include "QuestSystemCharacter.generated.h"

UCLASS()
class QUESTSYSTEM_API AQuestSystemCharacter : public ACharacter, public IInteractableObject {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ToggleQuestListVisibility();

	UFUNCTION(BlueprintCallable)
	void OnQuestAccept(AQuest* Quest, UQuestListComponent* ActorQuestList);

	UPROPERTY()
	AActor* ActorInteractedWith;

	UPROPERTY()
	UQuestList* QuestList;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UQuestList> QuestListClass;

	UPROPERTY()
	UQuestListComponent* QuestListComp;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Quest Mark")
	class UStaticMeshComponent* QuestMark;

	AQuestSystemCharacter();

	void Interact_Implementation(AActor* ActorInteractedWithObject) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestDialog> QuestDialogClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool HasQuests();

};
