#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySlotInfo.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UDataTable* InventorySlotsTable;

	UDataTable* GetInventorySlotsTable() const {
		return InventorySlotsTable;
	}

	FInventorySlotInfo* GetItem(FName RowName);
	const TMap<FName, uint8*>& GetItems();
	int32 GetItemsNum();
		
};
