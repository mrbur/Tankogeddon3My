// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapLoader.generated.h"

UCLASS()
class TANKOGEDDON_API AMapLoader : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMapLoader();

    UFUNCTION(BlueprintCallable, Category = "MapLoader")
    void SetIsActivated(bool NewIsActivated);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UStaticMeshComponent* GatesMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UBoxComponent* TriggerCollider;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UPointLightComponent* ActivatedLight;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UPointLightComponent* DeactivatedLight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
    FName LoadLevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
    bool bIsActivated = false;

private:
    void SetActiveLights();

    UFUNCTION()
    void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};