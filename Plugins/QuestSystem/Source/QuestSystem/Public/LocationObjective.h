// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objective.h"
#include "LocationObjective.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API ULocationObjective : public UObjective
{
	GENERATED_BODY()

	ULocationObjective();

	void ActivateObjective(AActor* Character) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Marker;
	
};
