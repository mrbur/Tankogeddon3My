#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableObject : public UInterface
{
    GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionFinished,
AActor*, InteractableObject, AActor*, ActorInteractedWithObject);

/**
 *
 */
class QUESTSYSTEM_API IInteractableObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact(AActor* ActorInteractedWithObject);
    UFUNCTION()
    virtual void Interact_Implementation(AActor* ActorInteractedWithObject) {}

    virtual void NotifyInteractionFinished(AActor* InteractableObject,
        AActor* ActorInteractedWithObject)
    {
        if (OnInteractionFinished.IsBound())
        {
            OnInteractionFinished.Broadcast(InteractableObject,
                ActorInteractedWithObject);
        }
    }

    FOnInteractionFinished OnInteractionFinished;
};