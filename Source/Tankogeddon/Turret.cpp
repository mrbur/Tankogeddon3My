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
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/ForceFeedbackEffect.h"

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

    EnemyCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Enemy Collider"));
    EnemyCollider->SetupAttachment(BodyMesh);
    BodyMesh->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnMeshOverlapBegin);

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


    HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Hit Effect"));
    HitEffect->SetupAttachment(CannonSetupPoint);

    DieEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Die Effect"));
    DieEffect->SetupAttachment(CannonSetupPoint);

    AudioHitEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Hit Effect"));
    AudioHitEffect->SetupAttachment(CannonSetupPoint);

    AudioDieEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Die Effect"));
    AudioDieEffect->SetupAttachment(CannonSetupPoint);
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

    if (IsEnemy) {
        TargetActor = PlayerPawn;
    }
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
    HealthComponent->TakeDamage(DamageData);
}

bool ATurret::IsPlayerInRange()
{
    if (TargetActor == nullptr) return false;
    return FVector::DistSquared(TargetActor->GetActorLocation(), GetActorLocation()) <= FMath::Square(TargetingRange);
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

bool ATurret::IsTargetHiddenBehind()
{
    FHitResult HitResult;
    FVector TraceStart = GetActorLocation();
    FVector TraceEnd = TargetActor->GetActorLocation();
    FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("AI Vission Trace")), true, this);
    TraceParams.bReturnPhysicalMaterial = false;
    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
    {
        DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.1f, 0, 5);
        if (HitResult.Actor != PlayerPawn)
        {
            return true;
        }
    }
    return false;
}

void ATurret::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (IsEnemy)return;
    if (OtherActor == PlayerPawn) return;

    TargetActor = OtherActor;
}

void ATurret::OnHealthChanged_Implementation(float Damage)
{
    HitEffect->ActivateSystem();
    AudioHitEffect->Play();
    UE_LOG(LogTankogeddon, Log, TEXT("Turret %s taked damage:%f "), *GetName(), Damage);
}

void ATurret::OnDie_Implementation()
{
    DieEffect->ActivateSystem();
    AudioDieEffect->Play();
    Destroy();
}


void ATurret::Targeting()
{
    if (IsPlayerInRange())
    {
        RotateToPlayer();
    }
    if (!CanFire()) {
        return;
    }
    if (TargetActor != nullptr && IsTargetHiddenBehind()) {
        return;
    }

    if (Cannon && Cannon->IsReadyToFire())
    {
        Fire();
    }
}

void ATurret::RotateToPlayer()
{
    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
    FRotator CurrentRotation = TurretMesh->GetComponentRotation();
    TargetRotation.Roll = CurrentRotation.Roll;
    TargetRotation.Pitch = CurrentRotation.Pitch;
    TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, RotateSpeed * GetWorld()->GetDeltaSeconds(), TurretRotationSmootheness));
}

void ATurret::Destroyed()
{
    Super::Destroyed();

    if (Cannon)
    {
        Cannon->Destroy();
    }
}