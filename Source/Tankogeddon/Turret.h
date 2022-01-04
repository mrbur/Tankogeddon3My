// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damageable.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Turret.generated.h"

UCLASS()
class TANKOGEDDON_API ATurret : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamage(const FDamageData& DamageData) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonSetupPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* EnemyCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* DieEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* HitEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* AudioHitEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* AudioDieEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<class ACannon> CannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accuracy = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	class AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	bool IsEnemy = true;

	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";

	UPROPERTY()
	class ACannon* Cannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	class APawn* PlayerPawn;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();
	void Fire();
	bool IsTargetHiddenBehind();

	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnHealthChanged(float Damage);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDie();

private:
	float TargetRotateRightAxis = 45.f;
	float TurretRotationSmootheness = 0.5f;
	float RotateSpeed = 30.f;

};
