#include "PhysicsProjectile.h"
#include "PhysicsMovementComponent.h"
#include "WindComponent.h"
#include "Damageable.h"
#include "DrawDebugHelpers.h"

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

    float vx = 0.5 * MoveSpeed;
    float vy = MovementComponent->Velocity.Z;

    float t = 2 * vy / (MovementComponent->Gravity.Z * -1);

    float s = vx * t;
    endp = GetActorForwardVector();

    endp.Z = 0;
    endp.Normalize();
    endp = s * endp;
    endp = endp + GetActorLocation();
}

void APhysicsProjectile::Stop()
{
    MovementComponent->Velocity = FVector::ZeroVector;
    MovementComponent->SetComponentTickEnabled(false);

    Super::Stop();
}

void APhysicsProjectile::Tick(float DeltaSeconds)
{
    DrawDebugLine(GetWorld(), GetActorLocation(), endp, FColor::Green, false, 0.5f, 0, 5.f);

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