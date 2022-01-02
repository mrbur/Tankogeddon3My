// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Objective.generated.h"

UENUM()
enum class EObjectiveType : uint8
{
    Location,
    Interact,
    Collect,
    Kill,
    Loot,
    EnemyKill
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, UObjective* /*Objective*/);
UCLASS(Abstract)
class QUESTSYSTEM_API UObjective : public UObject
{
	GENERATED_BODY()
	
public:
    virtual void ActivateObjective(AActor* Character) {}

    UPROPERTY(EditAnywhere)
    FText Descrition;
    UPROPERTY(VisibleAnywhere)
    EObjectiveType Type;

    FOnObjectiveCompleted OnObjectiveCompleted;

    UPROPERTY(VisibleAnywhere)
    bool bIsCompleted;
    UPROPERTY(VisibleAnywhere)
    bool bCanBeCompleted;
};
