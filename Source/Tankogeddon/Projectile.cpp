// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Tankogeddon.h"
#include "Damageable.h"
#include "ScoreComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.TickInterval = 0.005f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnMeshHit);
}

void AProjectile::Start()
{
	StartPosition = GetActorLocation();
}

void AProjectile::Stop()
{
}

void AProjectile::OnMeshHit(UPrimitiveComponent* HittedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	UE_LOG(LogTankogeddon, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

	if (OtherActor == GetInstigator())
	{
		Destroy();
		return;
	}

	if (OtherComp->IsSimulatingPhysics())
	{
		FVector Impulse = Mass * MoveSpeed * GetActorForwardVector();
		OtherComp->AddImpulseAtLocation(Impulse, SweepResult.ImpactPoint);
	}

	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	else if (IDamageable* Damageable = Cast<IDamageable>(OtherActor))
	{
		FDamageData DamageData;
		DamageData.DamageValue = Damage;
		DamageData.Instigator = GetInstigator();
		DamageData.DamageMaker = this;
		Damageable->TakeDamage(DamageData);

		if (OtherActor->IsActorBeingDestroyed()) {
			ScoreComponent->OnDestroySomeone.Broadcast();
		}
	}
	Destroy();
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile::Explode()
{
    FVector StartPos = GetActorLocation();
    FVector EndPos = StartPos + FVector(0.1f);

    FCollisionShape Shape = FCollisionShape::MakeSphere(ExplosionRange);
    FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
    Params.AddIgnoredActor(this);
    Params.bTraceComplex = true;
    Params.TraceTag = "Explode Trace";
    TArray<FHitResult> AttackHit;

    FQuat Rotation = FQuat::Identity;

    GetWorld()->DebugDrawTraceTag = "Explode Trace";

    bool bSweepResult = GetWorld()->SweepMultiByChannel
    (
        AttackHit,
        StartPos,
        EndPos,
        Rotation,
        ECollisionChannel::ECC_Visibility,
        Shape,
        Params
    );

    if (bSweepResult)
    {
        for (FHitResult HitResult : AttackHit)
        {
            AActor* HitActor = HitResult.GetActor();
            if (!HitActor)
                continue;

            MakeDamage(HitActor);
            MakeImpulse(HitActor);
        }
    }

    Stop();
}

void AProjectile::MakeImpulse(AActor* HitActor)
{
    UPrimitiveComponent* HitMesh = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
    if (HitMesh)
    {
        if (HitMesh->IsSimulatingPhysics())
        {
            FVector ForceVector = HitActor->GetActorLocation() - GetActorLocation();
            ForceVector.Normalize();
            HitMesh->AddImpulse(ForceVector * ExplosionImpulse, NAME_None, true);
        }
    }
}

void AProjectile::MakeDamage(AActor* HitActor)
{
    IDamageable* DamageableActor = Cast<IDamageable>(HitActor);
    if (DamageableActor)
    {
        FDamageData DamageData;
        DamageData.DamageValue = Damage;
        DamageData.Instigator = GetOwner();
        DamageData.DamageMaker = this;

        DamageableActor->TakeDamage(DamageData);
    }
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(NextPosition, true);

	if (FVector::Dist(GetActorLocation(), StartPosition) > FireRange)
	{
		Destroy();
	}
}

