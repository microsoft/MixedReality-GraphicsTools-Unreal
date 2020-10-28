// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

using UnrealBuildTool;
using System.Collections.Generic;

public class GraphicsToolsProjectTarget : TargetRules
{
	public GraphicsToolsProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "GraphicsToolsProject" });
	}
}
