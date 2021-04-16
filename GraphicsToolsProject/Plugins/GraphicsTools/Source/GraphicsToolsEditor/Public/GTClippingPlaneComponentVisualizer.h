// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "ComponentVisualizer.h"
#include "GTClippingPlaneComponent.h"

/**
 * Editor visualization for the UGTClippingPlaneComponent.
 */
class FGTClippingPlaneComponentVisualizer : public FComponentVisualizer
{
private:
	/** Draws widgets, such as the ClippingPlane's bounds, within the editor. */
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
