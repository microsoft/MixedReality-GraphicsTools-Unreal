// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "ComponentVisualizer.h"
#include "GTClippingConeComponent.h"

/**
 * Editor visualization for the UGTClippingConeComponent.
 */
class FGTClippingConeComponentVisualizer : public FComponentVisualizer
{
private:
	/** Draws widgets, such as the ClippingCone's bounds, within the editor. */
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
