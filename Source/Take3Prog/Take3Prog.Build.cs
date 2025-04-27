// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Take3Prog : ModuleRules
{
	public Take3Prog(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
