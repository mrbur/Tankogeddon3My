// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damageable.h"
#include "Components/ArrowComponent.h"
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
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<class ACannon> CannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accuracy = 10.f;

	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();
	void Fire();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnHealthChanged(float Damage);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDie();

private:
	UPROPERTY()
	class ACannon* Cannon;

	UPROPERTY()
	class APawn* PlayerPawn;

};
