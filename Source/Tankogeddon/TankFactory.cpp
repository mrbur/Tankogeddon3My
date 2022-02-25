// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "Tankogeddon.h"
#include "TankPawn.h"
#include "Kismet/GameplayStatics.h"
#include "MapLoader.h"
#include "Serialization/Archive.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/ForceFeedbackEffect.h"

// Sets default values
ATankFactory::ATankFactory()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneComp;

    BuildingDestroyedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Destroyed Mesh"));
    BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
    
    BuildingMesh->SetVisibility(bIsFactoryActive);
    BuildingDestroyedMesh->SetVisibility(!bIsFactoryActive);
    BuildingMesh->SetupAttachment(SceneComp);
    BuildingDestroyedMesh->SetupAttachment(SceneComp);
    
    TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
    TankSpawnPoint->SetupAttachment(SceneComp);

    HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
    HitCollider->SetupAttachment(SceneComp);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
    HealthComponent->OnDie.AddDynamic(this, &ATankFactory::Die);
    HealthComponent->OnHealthChanged.AddDynamic(this, &ATankFactory::DamageTaked);

    TankCreationEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Tank Creation Effect"));
    TankCreationEffect->SetupAttachment(SceneComp);
    
    TankCreationAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Tank Creation Audio Effect"));
    TankCreationAudioEffect->SetupAttachment(TankSpawnPoint);
}

void ATankFactory::TakeDamage(const FDamageData& DamageData)
{
    HealthComponent->TakeDamage(DamageData);
}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(SpawnTankTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
}

void ATankFactory::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorld()->GetTimerManager().ClearTimer(SpawnTankTimerHandle);
}

void ATankFactory::SpawnNewTank()
{
    FTransform SpawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1.f));
    ATankPawn* NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    NewTank->SetPatrollingPoints(TankWayPoints);
    NewTank->FinishSpawning(SpawnTransform);

    TankCreationEffect->ActivateSystem();
    TankCreationAudioEffect->Play();
}

void ATankFactory::Die()
{
    if (MapLoader)
    {
        MapLoader->SetIsActivated(true);
    }
    bIsFactoryActive = false;
    BuildingMesh->SetVisibility(false);
    BuildingDestroyedMesh->SetVisibility(true);

    GetWorld()->GetTimerManager().ClearTimer(SpawnTankTimerHandle);
}

void ATankFactory::DamageTaked(float DamageValue)
{
    UE_LOG(LogTankogeddon, Log, TEXT("Factory %s taked damage:%f "), *GetName(), DamageValue);
}