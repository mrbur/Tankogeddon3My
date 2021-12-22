// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WindComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPLUGIN_API UWindComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWindComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wind params")
	FVector WindVector = FVector(-10.f, 20.f, 0.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wind params")
	int WindVelocity = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wind params")
	UStaticMeshComponent* Mesh;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
