// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MediaPlayer.h"
#include "Components/Widget.h"
#include "AVideoPlane.generated.h"

UCLASS()
class TANKOGEDDON_API AVideoPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MediaPlayer")
	UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MediaPlayer")
	UWidget* Menu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MediaPlayer")
	FVector PlaneSize;
	// Sets default values for this actor's properties
	AVideoPlane();

	UFUNCTION()
	void CatchMediaOpened(FString OpenedUrl);

	UFUNCTION(BlueprintCallable)
	void OpenMedia();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
