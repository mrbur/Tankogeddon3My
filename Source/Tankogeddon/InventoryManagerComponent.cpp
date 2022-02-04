// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"
#include "InventoryComponent.h"
#include "DrawDebugHelpers.h"

void UInventoryManagerComponent::Init(UInventoryComponent* InInventoryComponent)
{
    LocalInventoryComponent = InInventoryComponent;

    if (LocalInventoryComponent && InventoryItemsData)
    {
        ensure(InventoryWidgetClass);
        InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(),
            InventoryWidgetClass);
        InventoryWidget->AddToViewport();

        InventoryWidget->Init(
            FMath::Max(LocalInventoryComponent->GetItemsNum(), MinInventorySize));


        FString ContextString;
        for (auto& Item : LocalInventoryComponent->GetInventorySlotsTable()->GetRowNames())
        {
            FInventorySlotInfo* InventorySlotInfo = LocalInventoryComponent->GetInventorySlotsTable()->FindRow<FInventorySlotInfo>(Item, ContextString);
            GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald, InventorySlotInfo->Name.ToString());
        }
        int i = 0;
        for (auto& Item : LocalInventoryComponent->GetInventorySlotsTable()->GetRowNames())
        {
            FInventorySlotInfo* InventorySlotInfo = LocalInventoryComponent->GetInventorySlotsTable()->FindRow<FInventorySlotInfo>(Item, ContextString);
            FInventoryItemInfo* ItemData = GetItemData(InventorySlotInfo->ID);
            if (ItemData)
            {
                ItemData->Icon.LoadSynchronous();
                InventoryWidget->AddItem(*InventorySlotInfo, *ItemData, i++);
            }
        }
    }
}

FInventoryItemInfo* UInventoryManagerComponent::GetItemData(FName ItemID)
{
    return InventoryItemsData ?
        InventoryItemsData->FindRow<FInventoryItemInfo>(ItemID, "") : nullptr;
}