// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "InventoryComponent.h"
#include "DrawDebugHelpers.h"

void UInventoryManagerComponent::Init(UInventoryComponent* InInventoryComponent)
{
    LocalInventoryComponent = InInventoryComponent;

    if (LocalInventoryComponent && InventoryItemsData)
    {
        for (auto& Item : LocalInventoryComponent->GetItems())
        {
            FInventoryItemInfo* ItemData = GetItemData(Item.Value.ID);
            if (ItemData)
            {
                // TODO manage item 
                FString ItemDataStr = ItemData->Name.ToString() + ": " +
                    FString::FromInt(Item.Value.Count);
                GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald,
                    ItemDataStr);
            }
        }
    }
}

FInventoryItemInfo* UInventoryManagerComponent::GetItemData(FName ItemID)
{
    return InventoryItemsData ?
        InventoryItemsData->FindRow<FInventoryItemInfo>(ItemID, "") : nullptr;
}