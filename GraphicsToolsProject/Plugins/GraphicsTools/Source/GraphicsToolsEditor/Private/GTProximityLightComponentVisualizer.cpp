// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTProximityLightComponentVisualizer.h"

#include "GraphicsToolsEditor.h"

#include <SceneManagement.h>

void FGTProximityLightComponentVisualizer::DrawVisualization(
	const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (const UGTProximityLightComponent* Light = Cast<const UGTProximityLightComponent>(Component))
	{
		FTransform LightTransform = Light->GetComponentTransform();
		LightTransform.RemoveScaling();

		// Draw the near radius and color.
		DrawWireSphereAutoSides(PDI, LightTransform, Light->GetCenterColor(), Light->GetNearRadius(), SDPG_World);

		// Draw the far radius and color.
		DrawWireSphereAutoSides(PDI, LightTransform, Light->GetOuterColor(), Light->GetFarRadius(), SDPG_World);
	}
}
