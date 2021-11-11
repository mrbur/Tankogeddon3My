// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "AlterCannon.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API AAlterCannon : public ACannon
{
	GENERATED_BODY()

public:
	AAlterCannon();
	virtual void Tick(float DeltaTime) override;

};
