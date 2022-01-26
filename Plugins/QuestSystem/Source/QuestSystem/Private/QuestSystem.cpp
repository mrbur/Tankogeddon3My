// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestSystem.h"
#include "QuestSystemEditorStyle.h"
#include "QuestSystemEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Engine/Selection.h"
#include "Quest.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "ToolMenus.h"

static const FName QuestSystemEditorTabName("QuestSystemEditor");

#define LOCTEXT_NAMESPACE "FQuestSystemModule"

void FQuestSystemModule::StartupModule()
{
	FQuestSystemEditorStyle::Initialize();
	FQuestSystemEditorStyle::ReloadTextures();

	FQuestSystemEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FQuestSystemEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FQuestSystemModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After,
			PluginCommands, FMenuExtensionDelegate::CreateRaw(this,
				&FQuestSystemModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After,
			PluginCommands, FToolBarExtensionDelegate::CreateRaw(this,
				&FQuestSystemModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(QuestSystemEditorTabName, FOnSpawnTab::CreateRaw(this, &FQuestSystemModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FStandaloneWindowTestTabTitle", "StandaloneWindowTest"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FQuestSystemModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FQuestSystemEditorCommands::Get().OpenPluginWindow);
}

void FQuestSystemModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FQuestSystemEditorCommands::Get().OpenPluginWindow);
}

void FQuestSystemModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FQuestSystemEditorStyle::Shutdown();

	FQuestSystemEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(QuestSystemEditorTabName);
}

TSharedRef<SDockTab> FQuestSystemModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::FromString("Show selected actors quest");
	VerticalBox = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.MaxHeight(25)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.MaxWidth(120)
		[
			SNew(SButton)
			.OnClicked_Lambda([this]()
				{
					if (GEditor)
					{
						for (FSelectionIterator SelectedActorsIter((GEditor->GetSelectedActorIterator())); SelectedActorsIter; ++SelectedActorsIter)
						{
							AActor* SelectedActor = Cast<AActor>(*SelectedActorsIter);

							TArray<AActor*> AttachedActors;
							SelectedActor->GetAttachedActors(AttachedActors);

							for (AActor* Actor : AttachedActors)
							{
								AQuest* Quest = Cast<AQuest>(Actor);
								if (Quest)AddRow(Quest);
							}
						}
					}
					return FReply::Handled();
				})
		[
			SNew(STextBlock)
			.Text(WidgetText)
		]
		]
		];

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab);
	SpawnedTab->SetContent(VerticalBox.ToSharedRef());
	return SpawnedTab;
}

void FQuestSystemModule::AddRow(AQuest* Quest)
{
	VerticalBox->AddSlot()
		.MaxHeight(50)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			SNew(SEditableTextBox)
			.Text(Quest->Name)
		]
	+ SHorizontalBox::Slot()
		[
			SNew(SEditableTextBox)
			.Text(Quest->Description)
		]
		];
}

void FQuestSystemModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(QuestSystemEditorTabName);
}

void FQuestSystemModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FQuestSystemEditorCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FQuestSystemEditorCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestSystemModule, QuestSystem)