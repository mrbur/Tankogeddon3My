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
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
    SetupCannon();
}

void ATankPawn::SmoothMove() {
    CurrentRotateRightAxis = FMath::Lerp(CurrentRotateRightAxis, TargetRotateRightAxis, RotationSmootheness);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector MoveVector = GetActorForwardVector() * CurrentMoveForwardAxis;
    FVector NewActorLocation = GetActorLocation() + MoveVector * MoveSpeed * DeltaTime;
    NewActorLocation += this->GetActorRightVector() * moveRight * MoveSpeedRight;
    SetActorLocation(NewActorLocation);
    
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
    SmoothMove();
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

void ATankPawn::SetupCannon()
{
    if (Cannon)
    {
        Cannon->Destroy();
    }

    FActorSpawnParameters Params;
    Params.Instigator = this;
    Params.Owner = this;
    Cannon = GetWorld()->SpawnActor<ACannon>(DefaultCannonClass, Params);
    Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
