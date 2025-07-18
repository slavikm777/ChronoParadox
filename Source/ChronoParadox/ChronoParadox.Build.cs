// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ChronoParadox : ModuleRules
{
	public ChronoParadox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AnimationCore", "AnimGraphRuntime" });
	}
}
