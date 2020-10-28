// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

using UnrealBuildTool;
using System.Collections.Generic;

public class GraphicsToolsProjectEditorTarget : TargetRules
{
	public GraphicsToolsProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "GraphicsToolsProject" });
	}
}
