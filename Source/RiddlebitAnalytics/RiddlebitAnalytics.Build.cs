// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	public class RiddlebitAnalytics : ModuleRules
	{
		public RiddlebitAnalytics(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

			//PublicIncludePaths.AddRange(new string[] {});
			PrivateIncludePaths.AddRange(new string[] { "RiddlebitAnalytics/Private" });

			PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Http", "Json" });
			//PrivateDependencyModuleNames.AddRange(new string[]{"CoreUObject", "Engine", "Slate", "SlateCore" });

			//DynamicallyLoadedModuleNames.AddRange(new string[]{});
		}
	}
}