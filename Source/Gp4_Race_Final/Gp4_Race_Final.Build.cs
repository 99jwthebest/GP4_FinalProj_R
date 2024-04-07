// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Gp4_Race_Final : ModuleRules
{
	public Gp4_Race_Final(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ChaosVehicles", "PhysicsCore" });
	}
}
