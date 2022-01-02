// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationObjective.h"
#include "LocationMarker.h"

ULocationObjective::ULocationObjective()
{
    Type = EObjectiveType::Location;
}

void ULocationObjective::ActivateObjective(AActor* Character)
{
    if (ILocationMarker* LocMarker = Cast<ILocationMarker>(Marker))
    {
        LocMarker->OnLocationReached.AddLambda([this, Character](
            AActor* LocationMarker, AActor* OverlappedActor)
            {
                if (bCanBeCompleted && Character == OverlappedActor)
                {
                    bIsCompleted = true;
                    if (OnObjectiveCompleted.IsBound())
                    {
                        OnObjectiveCompleted.Broadcast(this);
                    }
                }
            });
    }
}