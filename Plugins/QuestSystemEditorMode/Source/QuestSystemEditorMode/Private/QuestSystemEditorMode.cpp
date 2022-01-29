// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestSystemEditorMode.h"
#include "QuestSystemEditorModeEdMode.h"

#define LOCTEXT_NAMESPACE "FQuestSystemEditorModeModule"

void FQuestSystemEditorModeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FQuestSystemEditorModeEdMode>(FQuestSystemEditorModeEdMode::EM_QuestSystemEditorModeEdModeId, LOCTEXT("QuestSystemEditorModeEdModeName", "QuestSystemEditorModeEdMode"), FSlateIcon(), true);
}

void FQuestSystemEditorModeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FQuestSystemEditorModeEdMode::EM_QuestSystemEditorModeEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestSystemEditorModeModule, QuestSystemEditorMode)