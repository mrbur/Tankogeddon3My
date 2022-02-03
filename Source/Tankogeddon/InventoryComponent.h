// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySlotInfo.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TMap<int32, FInventorySlotInfo> Items;

	UPROPERTY(EditAnywhere)
	UDataTable* InventorySlotsTable;

public:
	FInventorySlotInfo* GetItem(int32 SlotIndex);
	void SetItem(int32 SlotIndex, const FInventorySlotInfo& Item);
	void ClearItem(int32 SlotIndex);
	const TMap<int32, FInventorySlotInfo>& GetItems();
	int32 GetItemsNum();
		
};
