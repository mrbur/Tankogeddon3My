// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

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
}

bool ACannon::checkAmmo() {
    if (!bIsReadyToFire) {
        return false;
    }
    if (currentAmmo == 0) {
        bIsReadyToFire = false;
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("Reloading..."));
        GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 3.f / FireRate, false);
        return false;
    }
    return true;
}

void ACannon::Reload()
{
    bIsReadyToFire = true;
    currentAmmo = 5;
}

void ACannon::FireSpecial() {
    if (!checkAmmo())
    {
        return;
    }
    currentAmmo--;
    
    GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Special fire"));
}

void ACannon::Fire()
{
    if (!checkAmmo())
    {
        return;
    }
    currentAmmo--;

    if (Type == ECannonType::FireProjectile)
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire - projectile"));
    }
    else if (Type == ECannonType::FireTrace)
    {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire - trace"));
    }
}

bool ACannon::IsReadyToFire()
{
    return bIsReadyToFire;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
    bIsReadyToFire = true;
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}
