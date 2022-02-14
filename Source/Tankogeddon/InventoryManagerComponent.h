// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.h"
#include "InventoryItemInfo.h"
#include <Tankogeddon/InventoryWidget.h>
#include "InventoryManagerComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void Init(UInventoryComponent* InInventoryComponent);

    UFUNCTION(BlueprintCallable)
    void HideInventory();

    FInventoryItemInfo* GetItemData(FName ItemID);

    UFUNCTION(BlueprintCallable)
    void LoadInventoryFromCSV();

    void InitEquipment(UInventoryComponent* InInventoryComponent);

    void SwapItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo);
    void ItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo);

protected:
    UPROPERTY()
    UInventoryComponent* LocalInventoryComponent;

    UPROPERTY(EditAnywhere)
    UDataTable* InventoryItemsData;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UInventoryWidget> InventoryWidgetClass;
    UPROPERTY(EditAnywhere)
    int32 MinInventorySize = 10;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UInventoryWidget> EquipInventoryWidgetClass;

    UPROPERTY()
    UInventoryWidget* InventoryWidget;

    UPROPERTY()
    UInventoryWidget* EquipInventoryWidget;
};
