// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Damageable.h"
#include "ScoreComponent.h"
#include "InteractionComponent.h"
#include "Ammo.h"
#include "QuestListComponent.h"
#include "InventoryComponent.h"
#include "InventoryManagerComponent.h"
#include "EquipInventoryComponent.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    int isPlayer = 0;

    //INVENTORY
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UInventoryComponent* InventoryComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UInventoryManagerComponent* InventoryManagerComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UEquipInventoryComponent* EquipmentInventoryComponent;

    virtual void TakeDamage(const FDamageData& DamageData) override;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* BodyMesh;
    
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* TurretMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class USpringArmComponent* SpringArm;
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UCameraComponent* Camera;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UArrowComponent* CannonSpawnPoint;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UBoxComponent* HitCollider;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UBoxComponent* EnemyCollider;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UHealthComponent* HealthComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UInteractionComponent* InteractionComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UParticleSystemComponent* DieEffect;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UParticleSystemComponent* HitEffect;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UAudioComponent* AudioHitEffect;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UAudioComponent* AudioDieEffect;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UQuestListComponent* QuestListComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
    float MoveSpeed = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
    float MoveSpeedRight = 4.f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
    float RotationSpeed = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float MovementSmootheness = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float RotationSmootheness = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
    float TurretRotationSmootheness = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params", Meta = (MakeEditWidget = true))
    TArray<class ATargetPoint*> PatrollingPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params")
    float MovementAccuracy = 50.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
    TSubclassOf<class ACannon> DefaultCannonClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
    TSubclassOf<class AAlterCannon> AlterCannonClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
    TSubclassOf<class AAmmo> AAmmoClass;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveForward(float InAxisValue);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void RotateRight(float InAxisValue);

    UFUNCTION(BlueprintCallable, Category = "Turret")
    void SetTurretTargetPosition(const FVector& TargetPosition);
    
    UFUNCTION(BlueprintCallable, Category = "Turret")
    void FireSpecial();
    UFUNCTION(BlueprintCallable, Category = "Turret")
    void Fire();
    UFUNCTION(BlueprintCallable, Category = "Turret")
    void ChangeCannon();
    UFUNCTION(BlueprintCallable, Category = "Turret")
    void AddAmmoToPool(int ammoCount);
    UFUNCTION(BlueprintCallable, Category = "Turret")
    int getMaxAmmo() const;

    UFUNCTION(BlueprintPure, Category = "Turret")
    FVector GetTurretForwardVector();

    UFUNCTION(BlueprintNativeEvent, Category = "Health")
    void OnHealthChanged(float Damage);

    UFUNCTION(BlueprintNativeEvent, Category = "Health")
    void OnHealthLoad();

    UFUNCTION(BlueprintNativeEvent, Category = "Health")
    void OnDie();

    UFUNCTION(BlueprintNativeEvent, Category = "Score")
    void AddScore();

    UFUNCTION(BlueprintPure, Category = "AI|Move params")
    const TArray<class ATargetPoint*>& GetPatrollingPoints()
    {
        return PatrollingPoints;
    }

    UFUNCTION(BlueprintCallable, Category = "AI|Move params")
        void SetPatrollingPoints(const TArray<class ATargetPoint*>& InPoints)
    {
        PatrollingPoints = InPoints;
    }

    UFUNCTION(BlueprintPure, Category = "AI|Move params")
    float GetMovementAccuracy()
    {
        return MovementAccuracy;
    }

    UFUNCTION(BlueprintPure, Category = "AI|Move params")
    int IsPlayer();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    class ACannon* Cannon = nullptr;

    UFUNCTION(BlueprintCallable)
    void OnGameLoaded(const FString& SlotName, UDataTable* InventorySlotsTable, FString CurrentAmmo, FString Health);

private:
    void SetupCannon();

    class ACannon* DefaultCannon = nullptr;
    class ACannon* AltCannon = nullptr;
    bool bIsDefaultCannon = true;

    float CurrentMoveForwardAxis = 0.f;
    float TargetMoveForwardAxis = 0.f;
    float CurrentRotateRightAxis = 0.f;
    float TargetRotateRightAxis = 0.f;
    float moveRight = 0.f;

    int score = 0;

    FVector TurretTargetPosition;
};
