#include "PhysicsProjectile.h"
#include "PhysicsMovementComponent.h"
#include "WindComponent.h"
#include "Damageable.h"

APhysicsProjectile::APhysicsProjectile()
{
    MovementComponent = CreateDefaultSubobject<UPhysicsMovementComponent>(TEXT("Movement Component"));
    WindComponent = CreateDefaultSubobject<UWindComponent>(TEXT("Wind Component"));
}

void APhysicsProjectile::Start()
{
    Super::Start();

    MovementComponent->Velocity = GetActorForwardVector() * MoveSpeed;
    MovementComponent->SetComponentTickEnabled(true);
    WindComponent->SetComponentTickEnabled(true);
}

void APhysicsProjectile::Stop()
{
    MovementComponent->Velocity = FVector::ZeroVector;
    MovementComponent->SetComponentTickEnabled(false);

    Super::Stop();
}

void APhysicsProjectile::Tick(float DeltaSeconds)
{
    if (GetActorLocation().Z < -10000.f)
    {
        Stop();
    }
}

void APhysicsProjectile::OnMeshHit(class UPrimitiveComponent* HittedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
    if (Super::isExplosive) {
        Super::Explode();
    }
    Destroy();
}