// Fill out your copyright notice in the Description page of Project Settings.


#include "AlterCannon.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"

AAlterCannon::AAlterCannon() {
    PrimaryActorTick.bCanEverTick = true;
}

void AAlterCannon::Tick(float DeltaTime)
{
    float vx = 0.5 * ProjectileMoveSpeed;
    FVector Velocity = ProjectileMoveSpeed * ProjectileSpawnPoint->GetForwardVector();
    float vy = Velocity.Z;

    float t = 2 * vy / GravityConst;

    float s = vx * t;
    FVector endp = GetActorForwardVector();

    endp.Z = 0;
    endp.Normalize();
    endp = s * endp;
    endp = endp + GetActorLocation();

    DrawDebugLine(GetWorld(), GetActorLocation(), endp, FColor::Emerald, false, 0.5f, 0, 5.f);
}
