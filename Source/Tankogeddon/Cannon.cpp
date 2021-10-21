// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "Damageable.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/ForceFeedbackEffect.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    USceneComponent* SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceeneCpm;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
    Mesh->SetupAttachment(RootComponent);

    ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
    ProjectileSpawnPoint->SetupAttachment(Mesh);

    ScoreComponent = CreateDefaultSubobject<UScoreComponent>(TEXT("Score component"));


    ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot Effect"));
    ShootEffect->SetupAttachment(ProjectileSpawnPoint);

    AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Effect"));
    AudioEffect->SetupAttachment(ProjectileSpawnPoint);
}

bool ACannon::checkAmmo() {
    if (CurrentAmmo == 0) {
        if (AmmoPool == 0) {
            GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("NO AMMO"));
            GetWorld()->GetTimerManager().ClearTimer(FireHandle);
            bIsDuringFire = false;
            return false;
        }
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Reloading..."));
        bIsReloading = true;
        GetWorld()->GetTimerManager().ClearTimer(FireHandle);
        bIsDuringFire = false;
        GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::ReloadEnd, 3.f / FireRate, false);
        return false;
    }
    return true;
}

void ACannon::ReloadEnd()
{
    bIsReloading = false;
    if (AmmoPool == 0) {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("No ammo in pool"));
        return;
    }
    if (AmmoPool >= MaxAmmoInSet) {
        AmmoPool -= MaxAmmoInSet;
        CurrentAmmo = MaxAmmoInSet;
    }
    else {
        CurrentAmmo = AmmoPool;
        AmmoPool = 0;
    }
}

void ACannon::FireSpecial() {
    if (!IsReadyToFire()) {
        return;
    }
    if (!checkAmmo())return;
    GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow, TEXT("Special fire"));
    CurrentAmmo--;
}

void ACannon::Shoot()
{
    if (!checkAmmo())
    {
        return;
    }
    CurrentAmmo--;


    ShootEffect->ActivateSystem();
    AudioEffect->Play();

    if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
    {
        if (ShootForceEffect)
        {
            FForceFeedbackParameters Params;
            Params.bLooping = false;
            Params.Tag = TEXT("ShootFFParams");
            GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect);
        }

        if (ShootShake)
        {
            GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(ShootShake);
        }
    }



    if (Type == ECannonType::FireProjectile)
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire - projectile"));

        AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
        Projectile->ScoreComponent = ScoreComponent;
        if (Projectile)
        {
            Projectile->SetInstigator(GetInstigator());
            Projectile->Start();
        }
    }
    else if (Type == ECannonType::FireTrace)
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire - trace"));
      
        FHitResult HitResult; 
        FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
        FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
        FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
        TraceParams.bReturnPhysicalMaterial = false;
        if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
        {
            TraceEnd = HitResult.Location;

            if (IDamageable* Damageable = Cast<IDamageable>(HitResult.GetActor()))
            {
                FDamageData DamageData;
                DamageData.DamageValue = 1;
                DamageData.Instigator = GetInstigator();
                DamageData.DamageMaker = this;
                Damageable->TakeDamage(DamageData);
            }
        }
        DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);

        
    }
}

void ACannon::Fire()
{
    if (!IsReadyToFire()) {
        return;
    }
    GetWorld()->GetTimerManager().SetTimer(FireHandle, this, &ACannon::Shoot, AlterFiredelay, true);
    bIsDuringFire = true;
}

bool ACannon::IsReadyToFire()
{
    return !bIsReloading && !bIsDuringFire;
}

int ACannon::GetMaxAmmo() const
{
    return MaxAmmoInSet;
}

void ACannon::AddAmmoToPool(int ammoCount)
{
    AmmoPool += ammoCount;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}
