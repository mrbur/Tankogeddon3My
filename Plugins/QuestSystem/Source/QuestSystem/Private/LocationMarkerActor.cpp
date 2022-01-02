// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationMarkerActor.h"

// Sets default values
ALocationMarkerActor::ALocationMarkerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALocationMarkerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALocationMarkerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

