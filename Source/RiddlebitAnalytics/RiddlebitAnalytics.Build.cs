// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RiddlebitAnalytics : ModuleRules
{
	public RiddlebitAnalytics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] {});
		PrivateIncludePaths.AddRange(new string[] {});
		
		PublicDependencyModuleNames.AddRange(new string[]{"Core", "Engine", "Http", "Json", "JsonUtilities" });
		PrivateDependencyModuleNames.AddRange(new string[]{"CoreUObject", "Engine", "Slate", "SlateCore" });
		
		DynamicallyLoadedModuleNames.AddRange(new string[]{});
	}
}