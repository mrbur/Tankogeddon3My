// Fill out your copyright notice in the Description page of Project Settings.


#include "AVideoPlane.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include <Runtime/MediaAssets/Public/FileMediaSource.h>

// Sets default values
AVideoPlane::AVideoPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AVideoPlane::CatchMediaOpened(FString OpenedUrl)
{
    auto dim = MediaPlayer->GetVideoTrackDimensions(0, 0);
    PlaneSize = FVector(10 * dim.X / dim.Y, 10, 0);
}

void AVideoPlane::OpenMedia()
{
    TArray<FString> OutFileNames;
    FString Path;
    IDesktopPlatform* DesktopPlatform =
        FDesktopPlatformModule::Get();

    if (MediaPlayer)
    {
        FString FileTypes = "All Files(*.PNG;*.JPG;*.MP4)|*.PNG;*.JPG;*.MP4|"
            "Image Files(*.PNG;*.JPG;)|*.PNG;*.JPG;|"
            "Video Files(*.MP4)|*.MP4";

        uint32 SelectionFlag = 0;
        DesktopPlatform->OpenFileDialog(
            FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
            "Choose content", "", "", FileTypes, SelectionFlag, OutFileNames);

        if (OutFileNames.Num() > 0)
        {
            Path = OutFileNames[0];
        }

        UFileMediaSource* MediaSource = NewObject<UFileMediaSource>();
        MediaSource->FilePath = Path;
        MediaPlayer->OpenSource(MediaSource);
    }
}

// Called when the game starts or when spawned
void AVideoPlane::BeginPlay()
{
	Super::BeginPlay();
	
    if (MediaPlayer)
    {
        MediaPlayer->OnMediaOpened.AddDynamic(this, &AVideoPlane::CatchMediaOpened);
    }
}

// Called every frame
void AVideoPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

