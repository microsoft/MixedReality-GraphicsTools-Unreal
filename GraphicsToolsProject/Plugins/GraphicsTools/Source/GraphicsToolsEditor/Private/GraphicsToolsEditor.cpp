// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GraphicsToolsEditor.h"

#include "GTClippingBoxComponentVisualizer.h"
#include "GTClippingConeComponentVisualizer.h"
#include "GTClippingPlaneComponentVisualizer.h"
#include "GTClippingSphereComponentVisualizer.h"
#include "GTMeshOutlineComponent.h"
#include "GTMeshOutlineComponentDetails.h"
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
		{
			TSharedPtr<FComponentVisualizer> Visualizer = MakeShareable(new FGTProximityLightComponentVisualizer());

			if (Visualizer.IsValid())
			{
				GUnrealEd->RegisterComponentVisualizer(UGTProximityLightComponent::StaticClass()->GetFName(), Visualizer);
				Visualizer->OnRegister();
			}
		}
		{
			TSharedPtr<FComponentVisualizer> Visualizer = MakeShareable(new FGTClippingPlaneComponentVisualizer());

			if (Visualizer.IsValid())
			{
				GUnrealEd->RegisterComponentVisualizer(UGTClippingPlaneComponent::StaticClass()->GetFName(), Visualizer);
				Visualizer->OnRegister();
			}
		}
		{
			TSharedPtr<FComponentVisualizer> Visualizer = MakeShareable(new FGTClippingSphereComponentVisualizer());

			if (Visualizer.IsValid())
			{
				GUnrealEd->RegisterComponentVisualizer(UGTClippingSphereComponent::StaticClass()->GetFName(), Visualizer);
				Visualizer->OnRegister();
			}
		}
		{
			TSharedPtr<FComponentVisualizer> Visualizer = MakeShareable(new FGTClippingBoxComponentVisualizer());

			if (Visualizer.IsValid())
			{
				GUnrealEd->RegisterComponentVisualizer(UGTClippingBoxComponent::StaticClass()->GetFName(), Visualizer);
				Visualizer->OnRegister();
			}
		}
		{
			TSharedPtr<FComponentVisualizer> Visualizer = MakeShareable(new FGTClippingConeComponentVisualizer());

			if (Visualizer.IsValid())
			{
				GUnrealEd->RegisterComponentVisualizer(UGTClippingConeComponent::StaticClass()->GetFName(), Visualizer);
				Visualizer->OnRegister();
			}
		}

		// Register customizations
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomClassLayout(
			UGTMeshOutlineComponent::StaticClass()->GetFName(),
			FOnGetDetailCustomizationInstance::CreateStatic(&FGTMeshOutlineComponentDetails::MakeInstance));
	}
}

void FGraphicsToolsEditorModule::ShutdownModule()
{
	if (UObjectInitialized() && FModuleManager::Get().IsModuleLoaded(TEXT("PropertyEditor")))
	{
		// Unregister customizations
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(UGTMeshOutlineComponent::StaticClass()->GetFName());
	}

	if (GUnrealEd)
	{
		// Unregister visualizers
		GUnrealEd->UnregisterComponentVisualizer(UGTClippingConeComponent::StaticClass()->GetFName());
		GUnrealEd->UnregisterComponentVisualizer(UGTClippingBoxComponent::StaticClass()->GetFName());
		GUnrealEd->UnregisterComponentVisualizer(UGTClippingSphereComponent::StaticClass()->GetFName());
		GUnrealEd->UnregisterComponentVisualizer(UGTClippingPlaneComponent::StaticClass()->GetFName());
		GUnrealEd->UnregisterComponentVisualizer(UGTProximityLightComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
