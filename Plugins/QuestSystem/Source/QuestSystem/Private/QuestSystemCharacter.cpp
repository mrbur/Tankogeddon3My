// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemCharacter.h"
#include "QuestListComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Sets default values
AQuestSystemCharacter::AQuestSystemCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AQuestSystemCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuestSystemCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AQuestSystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AQuestSystemCharacter::Interact_Implementation(
    AActor* ActorInteractedWithObject)
{
    if (ActorInteractedWithObject)
    {
        // check if actor has QuestList and can accept quests
        UActorComponent* ActorQuestListComp =
            ActorInteractedWithObject->GetComponentByClass(
                UQuestListComponent::StaticClass());
        if (ActorQuestListComp)
        {
            UQuestListComponent* ActorQuestList =
                Cast<UQuestListComponent>(ActorQuestListComp);

            // past any limitations and quest choosing logic
            TArray<AActor*> AttachedActors;
            GetAttachedActors(AttachedActors);
            bool HadQuestsAvailable = false;
            for (AActor* Actor : AttachedActors)
            {
                if (1)
                    /*(Quest->IsAlreadyTaken() ||
                    (Quest->GetPrerquisedQuest() &&
                        !Quest->GetPrerquisedQuest()->IsCompleted()))*/
                {
                    continue;
                }

                if (QuestDialogClass)
                {
                    UQuestDialog* QuestDialog =
                        CreateWidget<UQuestDialog>(GetWorld(),
                            QuestDialogClass);
                    QuestDialog->Init(Cast<AQuest>(Actor));
                    QuestDialog->OnQuestAccepted.BindUObject(
                        ActorQuestList,
                        &UQuestListComponent::AddQuest, Cast<AQuest>(Actor));
                    QuestDialog->OnQuestQuited.BindLambda(
                        [this, ActorInteractedWithObject]()
                        {
                            NotifyInteractionFinished(this,
                                ActorInteractedWithObject);
                        });
                    QuestDialog->AddToViewport();
                }

                HadQuestsAvailable = true;
            }

            if (!HadQuestsAvailable)
            {
                NotifyInteractionFinished(this, ActorInteractedWithObject);
            }
        }
    }
}

void AQuestSystemCharacter::ToggleQuestListVisibility()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (QuestList)
    {
        QuestList->RemoveFromParent();
        QuestList = nullptr;
        UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
        PC->bShowMouseCursor = false;
    }
    else
    {
        if (QuestListClass)
        {
            QuestList = CreateWidget<UQuestList>(GetWorld(), QuestListClass);
            QuestList->Init(QuestListComp);
            QuestList->AddToViewport();
            UWidgetBlueprintLibrary::SetInputMode_GameAndUI(PC);
            PC->bShowMouseCursor = true;
        }
    }
}
