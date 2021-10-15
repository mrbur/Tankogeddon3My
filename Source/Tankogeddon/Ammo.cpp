// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"
#include "Tankogeddon.h"

// Sets default values
AAmmo::AAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh1;
	Mesh1->OnComponentBeginOverlap.AddDynamic(this, &AAmmo::OnMeshOverlapBegin);
	//Mesh->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	Mesh1->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh1->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmo::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTankogeddon, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
}