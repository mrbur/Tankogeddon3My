// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStructs.h"
#include "Turret.h"
#include <Runtime/UMG/Public/Components/TextBlock.h>
#include "TurretListItem.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UTurretListItem : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* NameText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
    TSubclassOf<class ATurret> Turret;

    void NativePreConstruct() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
    class UWidget* DragableWidget;


protected:
    FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;
    //FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    void NativeOnDragDetected(const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent,
        UDragDropOperation*& OutOperation) override;
    /*bool NativeOnDragOver(const FGeometry& InGeometry,
        const FDragDropEvent& InDragDropEvent,
        UDragDropOperation* InOperation) override;*/
    bool NativeOnDrop(const FGeometry& InGeometry,
        const FDragDropEvent& InDragDropEvent,
        UDragDropOperation* InOperation) override;

    UFUNCTION()
    void OnDragCancelledHandle(UDragDropOperation* Operation);

	
};
