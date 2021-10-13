// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UStaticMeshComponent* Mesh;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UArrowComponent* ProjectileSpawnPoint;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float FireRate = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float FireRange = 1000.f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float FireDamage = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float AlterFiredelay = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    int AlterFireCount = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    int currentAmmo = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    ECannonType Type = ECannonType::FireProjectile;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "Type == ECannonType::FireProjectile", EditConditionHides), Category = "Fire params")
    TSubclassOf<class AProjectile> ProjectileClass;

private:
    FTimerHandle ReloadTimerHandle;
    FTimerHandle FireHandle;
    bool bIsDuringFire = false;
    bool bIsReloading = false;

public:
    ACannon();

    void FireSpecial();
    void Shoot();
    void Fire();

    bool IsReadyToFire();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
    bool checkAmmo();
    void ReloadEnd();

};
