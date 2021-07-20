// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

using UnrealBuildTool;

public class GraphicsToolsEditor : ModuleRules
{
	public GraphicsToolsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Slate",
			"SlateCore",
			"Engine",
			"UnrealEd",
			"PropertyEditor",
			"RenderCore",
			"RHI",
			"ProceduralMeshComponent",
			"MeshDescription",
			"StaticMeshDescription",
			"AssetTools",
			"AssetRegistry",
			"GraphicsTools"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"UnrealEd",
			"ProceduralMeshComponent"
		});
	}
}

