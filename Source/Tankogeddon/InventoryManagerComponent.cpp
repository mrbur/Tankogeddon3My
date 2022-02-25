// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManagerComponent.h"
#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "DrawDebugHelpers.h"
#include "Misc/CString.h"

class FString;

void UInventoryManagerComponent::LoadInventoryFromCSV()
{
    FString csvFile = FPaths::ProjectContentDir() + "default_items.csv";

    if (FPaths::FileExists(csvFile)) {
        FString FileContent;

        FFileHelper::LoadFileToString(FileContent, *csvFile);

        LocalInventoryComponent->InventorySlotsTable->EmptyTable();
        TArray<FString> problems = LocalInventoryComponent->InventorySlotsTable->CreateTableFromCSVString(FileContent);

        if (InventoryWidget) {
            InventoryWidget->GoldCell->Clear();
            InventoryWidget->CellWidgets.Empty();
            if (InventoryWidget->ItemCellsGrid) {
                InventoryWidget->ItemCellsGrid->ClearChildren();
            }
        }

        Init(LocalInventoryComponent);
    }
}

void UInventoryManagerComponent::Init(UInventoryComponent* InInventoryComponent)
{
    LocalInventoryComponent = InInventoryComponent;

    if (InventoryWidget) {
        InventoryWidget->GoldCell->Clear();
        InventoryWidget->CellWidgets.Empty();
        if (InventoryWidget->ItemCellsGrid) {
            InventoryWidget->ItemCellsGrid->ClearChildren();
        }
    }

    if (LocalInventoryComponent && InventoryItemsData)
    {
        ensure(InventoryWidgetClass);
        InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
        InventoryWidget->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::ItemDropped);

        InventoryWidget->AddToViewport();

        InventoryWidget->Init(FMath::Max(LocalInventoryComponent->GetItemsNum(), MinInventorySize));

        FString ContextString;
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

void UInventoryManagerComponent::InitEquipment(UInventoryComponent* InInventoryComponent)
{
    if (!EquipInventoryWidgetClass)return;
    EquipInventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(),
        EquipInventoryWidgetClass);
    EquipInventoryWidget->OnItemDrop.AddUObject(InventoryWidget,
        &UInventoryWidget::OnItemDropped);

    EquipInventoryWidget->AddToViewport();
}

void UInventoryManagerComponent::SwapItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo)
{
    FText FromCount = DraggedFrom->CountText->GetText();
    DraggedFrom->CountText->SetText(DroppedTo->CountText->GetText());
    DroppedTo->CountText->SetText(FromCount);

    FSlateBrush SlateBrush = DraggedFrom->ItemImage->Brush;
    DraggedFrom->ItemImage->SetBrush(DroppedTo->ItemImage->Brush);
    DroppedTo->ItemImage->SetBrush(SlateBrush);

    FInventorySlotInfo FromItem = DraggedFrom->StoredItem;
    DraggedFrom->StoredItem = DroppedTo->StoredItem;
    DroppedTo->StoredItem = FromItem;
}

void UInventoryManagerComponent::ItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo)
{
    if (!DraggedFrom->bHasItem)return;

    if (DroppedTo->bHasItem) {
        if (DraggedFrom->StoredItem.ID == DroppedTo->StoredItem.ID) {
            int32 Sum = FCString::Atoi(*DroppedTo->CountText->Text.ToString()) + FCString::Atoi(*DraggedFrom->CountText->Text.ToString());
            DroppedTo->CountText->SetText(FText::FromString(FString::FromInt(Sum)));
            DraggedFrom->Clear();
            return;
        }
        else {
            SwapItemDropped(DraggedFrom, DroppedTo);
            return;
        }
    }
    
    int32 count = FCString::Atoi(*DraggedFrom->CountText->Text.ToString());
    if(count > 1) {
        DroppedTo->StoredItem = DraggedFrom->StoredItem;
        DroppedTo->ItemImage->SetBrush(DraggedFrom->ItemImage->Brush);
        DroppedTo->CountText->SetText(FText::FromString("1"));
        DraggedFrom->CountText->SetText(FText::FromString(FString::FromInt(--count)));
    }
    else {
        SwapItemDropped(DraggedFrom, DroppedTo);
    }
}

void UInventoryManagerComponent::HideInventory()
{
    if (InventoryWidget) {
        InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    if (EquipInventoryWidget) {
        EquipInventoryWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}