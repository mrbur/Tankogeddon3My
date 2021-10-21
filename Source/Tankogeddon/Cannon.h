// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include "ScoreComponent.h"
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

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UParticleSystemComponent* ShootEffect;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UAudioComponent* AudioEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    class UForceFeedbackEffect* ShootForceEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
    TSubclassOf<class UCameraShakeBase> ShootShake;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float FireRate = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float FireRange = 10000.f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float FireDamage = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float AlterFiredelay = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    int AlterFireCount = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    int AmmoPool = 15;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    int MaxAmmoInSet = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
    int CurrentAmmo = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
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

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UScoreComponent* ScoreComponent;

    void FireSpecial();
    void Shoot();
    void Fire();
    bool IsReadyToFire();
    int GetMaxAmmo() const;
    void AddAmmoToPool(int ammoCount);

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
    bool checkAmmo();
    void ReloadEnd();

};
