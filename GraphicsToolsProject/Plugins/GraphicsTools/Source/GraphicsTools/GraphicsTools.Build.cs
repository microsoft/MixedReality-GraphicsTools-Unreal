// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

using UnrealBuildTool;

public class GraphicsTools : ModuleRules
{
	public GraphicsTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"Projects",
			"RenderCore",
			"RHI"
		});
	}
}
