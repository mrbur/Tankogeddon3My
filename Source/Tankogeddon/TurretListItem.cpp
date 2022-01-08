// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretListItem.h"
#include "TurretDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UTurretListItem::NativePreConstruct() {
    Super::NativePreConstruct();

    if (NameText)
    {
        NameText->SetText(FText::FromString(Turret.Get()->ClassGeneratedBy->GetName()));
    }
}

FReply UTurretListItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this,
            EKeys::LeftMouseButton).NativeReply;
    }
    return FReply::Handled();
}

void UTurretListItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(
        UTurretDragDropOperation::StaticClass());
    if (OutOperation)
    {
        UTurretDragDropOperation* TurretDragDropOperation = Cast<UTurretDragDropOperation>(OutOperation);
        TurretDragDropOperation->TurretName = FText::FromString(Turret.Get()->ClassGeneratedBy->GetName());
        TurretDragDropOperation->DefaultDragVisual = DragableWidget;

        SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.7f));
        SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    else
    {
        Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    }
}

bool UTurretListItem::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void UTurretListItem::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
}
