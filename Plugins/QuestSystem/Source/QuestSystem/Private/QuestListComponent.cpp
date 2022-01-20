// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestListComponent.h"

void UQuestListComponent::AddQuest(AQuest* Quest)
{
    if (Quest)
    {
        AcceptedQuests.AddUnique(Quest);
        Quest->TakeQuest(GetOwner());
        Quest->OnQuestStatusUpdated.AddDynamic(this, &UQuestListComponent::OnQuestAdd);
    }
}

const TArray<AQuest*>& UQuestListComponent::GetQuests()
{
    return AcceptedQuests;
}

AQuest* UQuestListComponent::GetActiveQuest() const
{
    return ActiveQuest;
}

void UQuestListComponent::SetActiveQuest(AQuest* Quest)
{
    if (AcceptedQuests.Contains(Quest))
    {
        ActiveQuest = Quest;
        if (OnActiveQuestChanged.IsBound())
        {
            OnActiveQuestChanged.Broadcast(Quest);
        }
    }
}

void UQuestListComponent::OnQuestAdd(AQuest* Quest)
{
    if (ActiveQuest == Quest)
    {
        if (OnActiveQuestChanged.IsBound())
        {
            OnActiveQuestChanged.Broadcast(Quest);
        }
    }
}

void UQuestListComponent::BeginPlay()
{
    Super::BeginPlay();

    if (CurrentObjectivesWidgetClass)
    {
        UCurrentObjectives* CurrentObjectivesWidget =
            CreateWidget<UCurrentObjectives>(GetWorld(),
                CurrentObjectivesWidgetClass);
        OnActiveQuestChanged.AddUObject(CurrentObjectivesWidget,
            &UCurrentObjectives::SetCurrentObjectives);
        CurrentObjectivesWidget->AddToViewport();
    }

}


