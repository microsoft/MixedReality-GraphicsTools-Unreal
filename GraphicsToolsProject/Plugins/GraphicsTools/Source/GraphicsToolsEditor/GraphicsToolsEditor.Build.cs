// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

using UnrealBuildTool;

public class GraphicsToolsEditor : ModuleRules
{
	public GraphicsToolsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "GraphicsTools" });

		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
	}
}

