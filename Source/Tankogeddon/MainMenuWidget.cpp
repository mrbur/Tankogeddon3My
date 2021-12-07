// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include <Kismet/GameplayStatics.h>

void UMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (NewButton) {
		NewButton->OnPressed.AddDynamic(this, &ThisClass::OnNewGameClicked);
	}

	if (NewButton) {
		QuitButton->OnPressed.AddDynamic(this, &ThisClass::OnQuitGameClicked);
	}

	if (MenuAnimation) {
		PlayAnimation(MenuAnimation);
	}
}

void UMainMenuWidget::OnNewGameClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "/Game/Maps/TestLevel");
}

void UMainMenuWidget::OnQuitGameClicked()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}
