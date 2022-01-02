// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuestSystem : ModuleRules
{
	public QuestSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"Core", "CoreUObject", "Engine", "InputCore", "UMG", "MediaAssets", "DesktopPlatform", "TestPlugin"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "MediaAssets", "DesktopPlatform", "TestPlugin"});


		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
