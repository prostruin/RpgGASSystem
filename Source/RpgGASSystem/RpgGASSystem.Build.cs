// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RpgGASSystem : ModuleRules
{
	public RpgGASSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "GameplayTasks", "GameplayTags" });
	}
}
