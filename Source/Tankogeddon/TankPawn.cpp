// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Math/UnrealMathUtility.h"
#include "Tankogeddon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Cannon.h"
#include "Ammo.h"
#include "AlterCannon.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/ForceFeedbackEffect.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
    RootComponent = BodyMesh;

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
    TurretMesh->SetupAttachment(BodyMesh);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
    SpringArm->SetupAttachment(BodyMesh);
    SpringArm->bDoCollisionTest = false;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritYaw = false;
    SpringArm->bInheritRoll = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

    CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
    CannonSpawnPoint->SetupAttachment(TurretMesh);

    HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
    HitCollider->SetupAttachment(BodyMesh);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
    HealthComponent->OnHealthChanged.AddDynamic(this, &ATankPawn::OnHealthChanged);
    HealthComponent->OnDie.AddDynamic(this, &ATankPawn::OnDie);

    HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Hit Effect"));
    HitEffect->SetupAttachment(CannonSpawnPoint);

    DieEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Die Effect"));
    DieEffect->SetupAttachment(CannonSpawnPoint);

    AudioHitEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Hit Effect"));
    AudioHitEffect->SetupAttachment(CannonSpawnPoint);

    AudioDieEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Die Effect"));
    AudioDieEffect->SetupAttachment(CannonSpawnPoint);
}

void ATankPawn::TakeDamage(const FDamageData& DamageData)
{
    HealthComponent->TakeDamage(DamageData);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
    SetupCannon();
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    CurrentMoveForwardAxis = FMath::FInterpTo(CurrentMoveForwardAxis, TargetMoveForwardAxis, DeltaTime * MoveSpeed, MovementSmootheness);
    FVector MoveVector = GetActorForwardVector() * CurrentMoveForwardAxis;
    FVector NewActorLocation = GetActorLocation() + MoveVector * MoveSpeed * DeltaTime;
    NewActorLocation += this->GetActorRightVector() * moveRight * MoveSpeedRight;
    SetActorLocation(NewActorLocation, true);
    
    CurrentRotateRightAxis = FMath::FInterpTo(CurrentRotateRightAxis, TargetRotateRightAxis, DeltaTime * MoveSpeed, RotationSmootheness);
    float Rotation = GetActorRotation().Yaw + CurrentRotateRightAxis * RotationSpeed * DeltaTime;
    SetActorRotation(FRotator(0.f, Rotation, 0.f));

    UE_LOG(LogTankogeddon, Verbose, TEXT("CurrentRotateRightAxis: %f"), CurrentRotateRightAxis);

    FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TurretTargetPosition);
    FRotator CurrentRotation = TurretMesh->GetComponentRotation();
    TargetRotation.Roll = CurrentRotation.Roll;
    TargetRotation.Pitch = CurrentRotation.Pitch;
    TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, MoveSpeed * DeltaTime, TurretRotationSmootheness));
}

void ATankPawn::MoveForward(float InAxisValue)
{
    TargetMoveForwardAxis = InAxisValue;
}

void ATankPawn::RotateRight(float InAxisValue)
{
    TargetRotateRightAxis = InAxisValue;
}

void ATankPawn::SetTurretTargetPosition(const FVector& TargetPosition)
{
    TurretTargetPosition = TargetPosition;
}

void ATankPawn::FireSpecial() {
    if (Cannon)
    {
        Cannon->FireSpecial();
    }
}
void ATankPawn::Fire()
{
    if (Cannon)
    {
        Cannon->Fire();
    }
}

void ATankPawn::ChangeCannon()
{
    if (Cannon)
    {
        Cannon->DetachRootComponentFromParent(true);
        
        if (bIsDefaultCannon) {
            AltCannon->SetActorHiddenInGame(false);
            DefaultCannon->SetActorHiddenInGame(true);
            bIsDefaultCannon = false;
            Cannon = AltCannon;
        }
        else {
            DefaultCannon->SetActorHiddenInGame(false);
            AltCannon->SetActorHiddenInGame(true);
            bIsDefaultCannon = true;
            Cannon = DefaultCannon;
        }
        Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }
}

void ATankPawn::OnHealthChanged_Implementation(float Damage)
{
    HitEffect->ActivateSystem();
    AudioHitEffect->Play();
    UE_LOG(LogTankogeddon, Log, TEXT("Tank %s taked damage:%f "), *GetName(), Damage);
}

void ATankPawn::OnDie_Implementation()
{
    DieEffect->ActivateSystem();
    AudioDieEffect->Play();
    GetWorld()->SpawnActor<AAmmo>(AAmmoClass, RootComponent->GetComponentLocation(), RootComponent->GetComponentRotation());
    Destroy();
}

void ATankPawn::AddScore_Implementation()
{
    score++;
}

void ATankPawn::AddAmmoToPool(int ammoCount)
{
    Cannon->AddAmmoToPool(ammoCount);
}

int ATankPawn::getMaxAmmo() const
{
    return Cannon->GetMaxAmmo();
}

FVector ATankPawn::GetTurretForwardVector()
{
    return TurretMesh->GetForwardVector();
}

int ATankPawn::IsPlayer()
{
    return isPlayer;
}

void ATankPawn::SetupCannon()
{
    if (Cannon)
    {
        Cannon->Destroy();
    }

    FActorSpawnParameters Params;
    Params.Instigator = this;
    Params.Owner = this;

    AltCannon = GetWorld()->SpawnActor<AAlterCannon>(AlterCannonClass, Params);
    DefaultCannon = GetWorld()->SpawnActor<ACannon>(DefaultCannonClass, Params);

    DefaultCannon->ScoreComponent->OnDestroySomeone.AddDynamic(this, &ATankPawn::AddScore);

    Cannon = AltCannon;
    Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
