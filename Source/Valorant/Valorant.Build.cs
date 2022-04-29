// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Valorant : ModuleRules
{
	public Valorant(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","PhysicsCore" });
		PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks"});
	}
}
