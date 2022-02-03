// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "InventoryComponent.h"
#include "DrawDebugHelpers.h"

void UInventoryManagerComponent::Init(UInventoryComponent* InInventoryComponent)
{
    LocalInventoryComponent = InInventoryComponent;

    if (LocalInventoryComponent && InventoryItemsData)
    {
        FString ContextString;
        for (auto& Item : LocalInventoryComponent->GetInventorySlotsTable()->GetRowNames())
        {
            FInventorySlotInfo* ffg = LocalInventoryComponent->GetInventorySlotsTable()->FindRow<FInventorySlotInfo>(Item, ContextString);
            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald,
                ffg->Name.ToString());
        }
    }
}

FInventoryItemInfo* UInventoryManagerComponent::GetItemData(FName ItemID)
{
    return InventoryItemsData ?
        InventoryItemsData->FindRow<FInventoryItemInfo>(ItemID, "") : nullptr;
}