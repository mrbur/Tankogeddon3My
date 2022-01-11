// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretListItem.h"
#include "TurretDragDropOperation.h"
#include "Turret.h"
#include "Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UTurretListItem::NativePreConstruct() {
    Super::NativePreConstruct();

    if (NameText)
    {
        NameText->SetText(FText::FromString(Turret.Get()->ClassGeneratedBy->GetName()));
    }
}

void UTurretListItem::CreateTurret(FVector WhereToSpawn)
{
    ATurret* SpawnedTurret = GetWorld()->SpawnActor<ATurret>(Turret, WhereToSpawn, FRotator(0, 0, 0));
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
    const FVector2D& screenPos = InDragDropEvent.GetScreenSpacePosition();
    FVector WorldLocation;
    FVector WorldDirection;
    APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    controller->DeprojectScreenPositionToWorld(screenPos.X - 350, screenPos.Y - 120, WorldLocation, WorldDirection);

    FHitResult Hit(ForceInit);
    
    if (GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, WorldLocation + WorldDirection * 10000, ECC_Visibility, 1)) {
        CreateTurret(Hit.Location);
    }
    
    SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
}
