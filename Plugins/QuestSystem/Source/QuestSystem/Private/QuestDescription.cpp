// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDescription.h"
#include "Objective.h"
#include "Quest.h"
#include "ObjectiveWidget.h"

void UQuestDescription::Init(AQuest* Quest)
{
    if (Quest)
    {
        if (NameText)
        {
            NameText->SetText(Quest->Name);
        }
        if (DescriptionText)
        {
            DescriptionText->SetText(Quest->Description);
        }
        if (ObjectiveWidgetClass && ObjectivesList)
        {
            ObjectivesList->ClearChildren();
            for (UObjective* Objective : Quest->Objectives)
            {
                if (!Objective) continue;
                UObjectiveWidget* ObjectiveWidget =
                    CreateWidget<UObjectiveWidget>(this, ObjectiveWidgetClass);
                ObjectiveWidget->Init(Objective);
                ObjectivesList->AddChild(ObjectiveWidget);
            }
        }
    }
}