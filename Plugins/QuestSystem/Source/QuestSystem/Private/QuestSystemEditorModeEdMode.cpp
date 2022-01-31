// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestSystemEditorModeEdMode.h"
#include "QuestSystemEditorModeEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "Engine/Selection.h"
#include "QuestSystemCharacter.h"
#include "Quest.h"
#include "EditorModeManager.h"
#include "InteractionObjective.h"
#include "LocationObjective.h"
#include <Engine.h>

const FEditorModeID FQuestSystemEditorModeEdMode::EM_QuestSystemEditorModeEdModeId = TEXT("EM_QuestSystemEditorModeEdMode");

FQuestSystemEditorModeEdMode::FQuestSystemEditorModeEdMode()
{

}

FQuestSystemEditorModeEdMode::~FQuestSystemEditorModeEdMode()
{

}

void FQuestSystemEditorModeEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FQuestSystemEditorModeEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FQuestSystemEditorModeEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

void FQuestSystemEditorModeEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	for (AActor* BoundedActor : SelectedActors)
	{
		DrawWireBox(
			PDI,
			BoundedActor->GetComponentsBoundingBox(true),
			FColor::Cyan,
			1);
	}

	for (AActor* ObjActor : ObjectiveActors)
	{
		DrawWireBox(
			PDI,
			ObjActor->GetComponentsBoundingBox(true),
			FColor::Blue,
			1);
	}
	
	FEdMode::Render(View, Viewport, PDI);
}

void FQuestSystemEditorModeEdMode::ActorSelectionChangeNotify()
{
	FEdMode::ActorSelectionChangeNotify();

	UpdateSelectedActors();
}

void FQuestSystemEditorModeEdMode::UpdateSelectedActors()
{
	SelectedActors.Empty();

	USelection* ActorsSelection = GEditor->GetSelectedActors();
	bool NoQuestSelected = true;
	for (FSelectionIterator Iter(*ActorsSelection); Iter; ++Iter)
	{
		AQuestSystemCharacter* LevelActor = Cast<AQuestSystemCharacter>(*Iter);
		
		if (LevelActor && !SelectedActors.Contains(LevelActor))
		{
			NoQuestSelected = !checkActorForQuest(LevelActor);
		}
	}

	if (NoQuestSelected) {
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuestSystemCharacter::StaticClass(), FoundActors);
		for (AActor* Actor : FoundActors)
		{
			checkActorForQuest(Actor);
		}
	}
	else {
		for (AActor* SelectedActor: SelectedActors) {
			TArray<AActor*> AttachedActors;
			ObjectiveActors.Reset();
			SelectedActor->GetAttachedActors(AttachedActors);
			for (AActor* Actor : AttachedActors)
			{
				AQuest* Quest = Cast<AQuest>(Actor);
				if (!Quest)continue;

				for (UObjective* Objective : Quest->Objectives)
				{
					if (!Objective)continue;

					UInteractionObjective* InterObjective = Cast<UInteractionObjective>(Objective);
					if (InterObjective && InterObjective->Target) {
						ObjectiveActors.Add(InterObjective->Target);
						continue;
					}

					ULocationObjective* LocObjective = Cast<ULocationObjective>(Objective);
					if (LocObjective && LocObjective->Marker) {
						ObjectiveActors.Add(LocObjective->Marker);
					}
				}
			}
		}
	}
}

bool FQuestSystemEditorModeEdMode::UsesToolkits() const
{
	return true;
}

bool FQuestSystemEditorModeEdMode::checkActorForQuest(AActor* Actor)
{
	TArray<AActor*> AttachedActors;
	Actor->GetAttachedActors(AttachedActors);
	for (AActor* AttachedQuest : AttachedActors)
	{
		AQuest* Quest = Cast<AQuest>(AttachedQuest);
		if (!Quest)continue;

		SelectedActors.Add(Actor);

		if (Quest->Objectives.Num() == 0)continue;
	}
	return AttachedActors.Num() > 0;
}




