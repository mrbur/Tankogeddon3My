// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKOGEDDON_API AProjectile : public AActor
{
	GENERATED_BODY()

private:
	FVector StartPosition;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "range")
	float FireRange = 2000.f;

	UFUNCTION()
	virtual void OnMeshHit(class UPrimitiveComponent* HittedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
	float Mass = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosives")
	float ExplosionRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosives")
	float ExplosionImpulse = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosives")
	bool isExplosive = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Explode();
	void MakeImpulse(AActor* HitActor);
	void MakeDamage(AActor* HitActor);

public:	
	AProjectile();

	float MoveSpeed = 100.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UScoreComponent* ScoreComponent;

	virtual void Start();
	virtual void Stop();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
