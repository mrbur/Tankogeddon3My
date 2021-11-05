// Fill out your copyright notice in the Description page of Project Settings.


#include "WindComponent.h"

// Sets default values for this component's properties
UWindComponent::UWindComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UWindComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
}


void UWindComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Mesh->AddForce(FVector(-100.f, 200.f, 0.f));
}

