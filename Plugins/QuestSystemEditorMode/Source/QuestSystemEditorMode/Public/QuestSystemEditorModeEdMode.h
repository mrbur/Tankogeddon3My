// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

class FQuestSystemEditorModeEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_QuestSystemEditorModeEdModeId;
public:
	FQuestSystemEditorModeEdMode();
	virtual ~FQuestSystemEditorModeEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual void ActorSelectionChangeNotify() override;
	void UpdateSelectedActors();
	bool UsesToolkits() const override;
	// End of FEdMode interface

private:
	TArray<AActor*> SelectedActors;
	TArray<AActor*> ObjectiveActors;

	bool checkActorForQuest(AActor* Actor);
};
