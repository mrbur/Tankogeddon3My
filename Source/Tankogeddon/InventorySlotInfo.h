// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventorySlotInfo.generated.h"

UENUM()
enum class ESlotType : uint8
{
	SL_DEFAULT,
	SL_SPECIAL
};
USTRUCT(BlueprintType)
struct FInventorySlotInfo : public FTableRowBase
{
	GENERATED_BODY()
	
public:	
    // tech info
    UPROPERTY(EditAnywhere, Category = "General")
    FName ID;

    // text info
    UPROPERTY(EditAnywhere, Category = "General")
    FText Name;
    UPROPERTY(EditAnywhere, Category = "General")
    FText Description;

    UPROPERTY(EditAnywhere, Category = "General")
    int Count;

    // enums
    UPROPERTY(EditAnywhere, Category = "Typing")
    ESlotType Type;

};
