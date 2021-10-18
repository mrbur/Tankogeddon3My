// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/ArrowComponent.h"
#include "Cannon.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/NoExportTypes.h"
#include "Tankogeddon.h"
#include "HealthComponent.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005f;

    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret body"));
    RootComponent = BodyMesh;

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret turret"));
    TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

    CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
    CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

    HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
    HitCollider->SetupAttachment(BodyMesh);

    UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
    if (TurretMeshTemp)
    {
        TurretMesh->SetStaticMesh(TurretMeshTemp);
    }

    UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
    if (BodyMeshTemp)
    {
        BodyMesh->SetStaticMesh(BodyMeshTemp);
    }

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
    HealthComponent->OnHealthChanged.AddDynamic(this, &ATurret::OnHealthChanged);
    HealthComponent->OnDie.AddDynamic(this, &ATurret::OnDie);

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

    FActorSpawnParameters Params;
    Params.Owner = this;
    Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
    Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (PlayerPawn)
    {
        Targeting();
    }
}

void ATurret::TakeDamage(const FDamageData& DamageData)
{
    //TODO REALIZE METHOD
}

bool ATurret::IsPlayerInRange()
{
    return FVector::DistSquared(PlayerPawn->GetActorLocation(), GetActorLocation()) <= FMath::Square(TargetingRange);
}

bool ATurret::CanFire()
{
    FVector TargetingDir = TurretMesh->GetForwardVector();
    FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
    DirToPlayer.Normalize();
    float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDir, DirToPlayer)));
    return AimAngle <= Accuracy;
}

void ATurret::Fire()
{
    if (Cannon)
    {
        Cannon->Fire();
    }
}

void ATurret::OnHealthChanged_Implementation(float Damage)
{
    UE_LOG(LogTankogeddon, Log, TEXT("Turret %s taked damage:%f "), *GetName(), Damage);
}

void ATurret::OnDie_Implementation()
{
    Destroy();
}


void ATurret::Targeting()
{
    if (IsPlayerInRange())
    {
        RotateToPlayer();
    }

    if (CanFire() && Cannon && Cannon->IsReadyToFire())
    {
        Fire();
    }
}

void ATurret::RotateToPlayer()
{
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
    FRotator CurrRotation = TurretMesh->GetComponentRotation();
    TargetRotation.Pitch = CurrRotation.Pitch;
    TargetRotation.Roll = CurrRotation.Roll;
    TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
}

void ATurret::Destroyed()
{
    Super::Destroyed();

    if (Cannon)
    {
        Cannon->Destroy();
    }
}