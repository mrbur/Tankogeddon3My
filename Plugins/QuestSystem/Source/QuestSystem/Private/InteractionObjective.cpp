// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObjective.h"
#include "InteractableObject.h"

UInteractionObjective::UInteractionObjective()
{
    Type = EObjectiveType::Interact;
}

void UInteractionObjective::ActivateObjective(AActor* Char)
{
    Character = Char;
    if (IInteractableObject* InteractableTarget = Cast<IInteractableObject>(Target))
    {
        InteractableTarget->OnInteractionFinished.AddDynamic(this, &UInteractionObjective::OnInteractionObjectiveComplete);
    }
}

void UInteractionObjective::OnInteractionObjectiveComplete(AActor* InteractableObject, AActor* ActorInteractedWithObject)
{
    if (bCanBeCompleted && Character == ActorInteractedWithObject)
    {
        bIsCompleted = true;
        if (OnObjectiveCompleted.IsBound())
        {
            OnObjectiveCompleted.Broadcast(this);
        }
    }
}
