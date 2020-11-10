// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "ComponentVisualizer.h"
#include "GTProximityLightComponent.h"

/**
 * Editor visualization for the UGTProximityLightComponent.
 */
class FGTProximityLightComponentVisualizer : public FComponentVisualizer
{
private:
	/** Draws widgets, such as the light's radii, within the editor. */
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
