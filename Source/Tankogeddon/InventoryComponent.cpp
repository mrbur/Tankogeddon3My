// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "InventorySlotInfo.h"


FInventorySlotInfo* UInventoryComponent::GetItem(FName RowName)
{
    FString ContextString;
    return InventorySlotsTable->FindRow<FInventorySlotInfo>(RowName, ContextString);
}

const TMap<FName, uint8*>& UInventoryComponent::GetItems()
{
    return InventorySlotsTable->GetRowMap();
}

int32 UInventoryComponent::GetItemsNum()
{
    return InventorySlotsTable->GetRowMap().Num();
}