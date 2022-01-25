// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestSystemEditor.h"
#include "QuestSystemEditorStyle.h"
#include "QuestSystemEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Engine/Selection.h"
#include "ToolMenus.h"

static const FName QuestSystemEditorTabName("QuestSystemEditor");

#define LOCTEXT_NAMESPACE "FQuestSystemEditorModule"

void FQuestSystemEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FQuestSystemEditorStyle::Initialize();
	FQuestSystemEditorStyle::ReloadTextures();

	FQuestSystemEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FQuestSystemEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FQuestSystemEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After,
			PluginCommands, FMenuExtensionDelegate::CreateRaw(this,
				&FQuestSystemEditorModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After,
			PluginCommands, FToolBarExtensionDelegate::CreateRaw(this,
				&FQuestSystemEditorModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(QuestSystemEditorTabName, FOnSpawnTab::CreateRaw(this, &FQuestSystemEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FStandaloneWindowTestTabTitle", "StandaloneWindowTest"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FQuestSystemEditorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FQuestSystemEditorCommands::Get().OpenPluginWindow);
}

void FQuestSystemEditorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FQuestSystemEditorCommands::Get().OpenPluginWindow);
}

void FQuestSystemEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FQuestSystemEditorStyle::Shutdown();

	FQuestSystemEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(QuestSystemEditorTabName);
}

TSharedRef<SDockTab> FQuestSystemEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::FromString("Move selected actors");

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			.OnClicked_Lambda([]()
				{
					if (GEditor)
					{
						for (FSelectionIterator Iter((GEditor->GetSelectedActorIterator())); Iter; ++Iter)
						{
							AActor* Actor = Cast<AActor>(*Iter);
							if (Actor)
							{
								Actor->AddActorLocalOffset(FVector(50.f));
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
}

void FQuestSystemEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(QuestSystemEditorTabName);
}

void FQuestSystemEditorModule::RegisterMenus()
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
	
IMPLEMENT_MODULE(FQuestSystemEditorModule, QuestSystemEditor)