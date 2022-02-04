// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotInfo.h"
#include "InventoryItemInfo.h"
#include "Templates/SubclassOf.h"
#include "Components/UniformGridPanel.h"
#include "InventoryCellWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

    //virtual void NativeConstruct() override;

    void Init(int32 ItemsNum);
    bool AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo,
        int32 SlotPosition = -1);

protected:

    UPROPERTY(meta = (BindWidgetOptional))
    UUniformGridPanel* ItemCellsGrid;

    UPROPERTY(EditDefaultsOnly)
    int32 ItemsInPow = 5;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UInventoryCellWidget> CellWidgetClass;

    UPROPERTY()
    TArray<UInventoryCellWidget*> CellWidgets;

    UPROPERTY(meta = (BindWidgetOptional))
    UInventoryCellWidget* GoldCell;

    UInventoryCellWidget* CreateCellWidget();

};
