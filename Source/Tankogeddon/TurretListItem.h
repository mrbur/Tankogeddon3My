// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStructs.h"
#include "Turret.h"
#include <Runtime/UMG/Public/Components/TextBlock.h>
#include "TurretListItem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurretCreate, FVector, pos);

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

    UFUNCTION(BlueprintCallable, Category = "Turret")
    void CreateTurret(FVector WhereToSpawn);
protected:
    FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent) override;
    void NativeOnDragDetected(const FGeometry& InGeometry,
        const FPointerEvent& InMouseEvent,
        UDragDropOperation*& OutOperation) override;
    bool NativeOnDrop(const FGeometry& InGeometry,
        const FDragDropEvent& InDragDropEvent,
        UDragDropOperation* InOperation) override;
    void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
