// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTProximityLightComponentVisualizer.h"

#include "GraphicsToolsEditor.h"

#include "Engine/Public/SceneManagement.h"

void FGTProximityLightComponentVisualizer::DrawVisualization(
	const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (const UGTProximityLightComponent* Light = Cast<const UGTProximityLightComponent>(Component))
	{
		FTransform LightTransform = Light->GetComponentTransform();
		LightTransform.RemoveScaling();

		// Draw the projected radius and center color.
		DrawWireSphereAutoSides(PDI, LightTransform, Light->GetCenterColor(), Light->GetProjectedRadius(), SDPG_World);

		// Draw the shrink distance as a white sphere.
		DrawWireSphereAutoSides(PDI, LightTransform, FColor::White, Light->GetShrinkDistance(), SDPG_World);

		// Draw the attenuation radius and outer color.
		DrawWireSphereAutoSides(PDI, LightTransform, Light->GetOuterColor(), Light->GetAttenuationRadius(), SDPG_World);
	}
}
