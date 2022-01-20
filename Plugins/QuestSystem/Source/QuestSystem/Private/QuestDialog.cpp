// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDialog.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UQuestDialog::NativeConstruct()
{
    Super::NativeConstruct();

    if (RejectButton)
    {
        RejectButton->OnReleased.AddDynamic(this, &UQuestDialog::RejectQuest);
    }
    if (AcceptButton)
    {
        AcceptButton->OnReleased.AddDynamic(this, &UQuestDialog::AcceptQuest);
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    UWidgetBlueprintLibrary::SetInputMode_UIOnly(PC);
    PC->bShowMouseCursor = true;
}

void UQuestDialog::Init(AQuest* Quest)
{
    if (Description)
    {
        Description->Init(Quest);
    }
}

void UQuestDialog::RejectQuest()
{
    HideDialog();
}

void UQuestDialog::AcceptQuest()
{
    HideDialog();
    OnQuestAccepted.ExecuteIfBound();
}

void UQuestDialog::HideDialog()
{
    RemoveFromViewport();

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
    PC->bShowMouseCursor = false;

    OnQuestQuited.ExecuteIfBound();
}