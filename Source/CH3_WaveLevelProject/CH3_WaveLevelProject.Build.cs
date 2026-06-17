// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CH3_WaveLevelProject : ModuleRules
{
	public CH3_WaveLevelProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
