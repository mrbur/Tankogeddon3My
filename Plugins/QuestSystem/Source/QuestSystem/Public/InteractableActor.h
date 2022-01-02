// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.h"
#include "InteractableActor.generated.h"

UCLASS()
class QUESTSYSTEM_API AInteractableActor : public AActor, public IInteractableObject
{
    GENERATED_BODY()


        UFUNCTION(BlueprintCallable)
        void OnInteractionFinished(AActor* ActorInteractedWithObject)
    {
        NotifyInteractionFinished(this, ActorInteractedWithObject);
    }
};