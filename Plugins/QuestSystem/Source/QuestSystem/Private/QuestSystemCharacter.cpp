// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemCharacter.h"
#include "QuestListComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Sets default values
AQuestSystemCharacter::AQuestSystemCharacter()
{
    QuestMark = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Quest mark mesh"));
    QuestMark->SetupAttachment(RootComponent);

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
    ActorInteractedWith = ActorInteractedWithObject;
    if (ActorInteractedWithObject)
    {
        UActorComponent* ActorQuestListComp =
            ActorInteractedWithObject->GetComponentByClass(
                UQuestListComponent::StaticClass());
        if (ActorQuestListComp)
        {
            UQuestListComponent* ActorQuestList =
                Cast<UQuestListComponent>(ActorQuestListComp);

            TArray<AActor*> AttachedActors;
            GetAttachedActors(AttachedActors);
            bool HadQuestsAvailable = false;
            for (AActor* Actor : AttachedActors)
            {
                AQuest* Quest = Cast<AQuest>(Actor);
                if (!Quest) continue;

                if (Quest->bIsTaken ||
                    (Quest->PrerquisedQuest &&
                        !Quest->PrerquisedQuest->bIsComplete))
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
                        this,
                        &AQuestSystemCharacter::OnQuestAccept, Quest, ActorQuestList);

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

void AQuestSystemCharacter::OnQuestAccept(AQuest* Quest, UQuestListComponent* ActorQuestList)
{
    ActorQuestList->AddQuest(Quest);
    ToggleQuestListVisibility();
    QuestMark->SetHiddenInGame(true);
}

void AQuestSystemCharacter::ToggleQuestListVisibility()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    UActorComponent* ActorQuestListComp =
        ActorInteractedWith->GetComponentByClass(
            UQuestListComponent::StaticClass());
    UQuestListComponent* ActorQuestList =
        Cast<UQuestListComponent>(ActorQuestListComp);


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
            QuestList->Init(ActorQuestList);
            QuestList->AddToViewport();
            UWidgetBlueprintLibrary::SetInputMode_GameAndUI(PC);
            PC->bShowMouseCursor = true;
        }
    }
}
