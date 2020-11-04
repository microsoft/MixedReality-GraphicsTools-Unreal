// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GraphicsToolsEditor.h"

#include "GTProximityLightComponentVisualizer.h"
#include "UnrealEdGlobals.h"

#include "Editor/UnrealEdEngine.h"

IMPLEMENT_GAME_MODULE(FGraphicsToolsEditorModule, GraphicsToolsEditor);

DEFINE_LOG_CATEGORY(GraphicsToolsEditor)

#define LOCTEXT_NAMESPACE "GraphicsToolsEditor"

void FGraphicsToolsEditorModule::StartupModule()
{
	if (GUnrealEd)
	{
		// Register visualizers
		TSharedPtr<FComponentVisualizer> Visualizer = MakeShareable(new FGTProximityLightComponentVisualizer());

		if (Visualizer.IsValid())
		{
			GUnrealEd->RegisterComponentVisualizer(UGTProximityLightComponent::StaticClass()->GetFName(), Visualizer);
			Visualizer->OnRegister();
		}
	}
}

void FGraphicsToolsEditorModule::ShutdownModule()
{
	if (GUnrealEd)
	{
		// Unregister visualizers
		GUnrealEd->UnregisterComponentVisualizer(UGTProximityLightComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
