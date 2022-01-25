// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestSystemEditorCommands.h"

#define LOCTEXT_NAMESPACE "FQuestSystemEditorModule"

void FQuestSystemEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "QuestSystemEditor", "Bring up QuestSystemEditor window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
