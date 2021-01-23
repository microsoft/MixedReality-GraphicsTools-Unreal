// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "ComponentVisualizer.h"
#include "GTClippingBoxComponent.h"

/**
 * Editor visualization for the UGTClippingBoxComponent.
 */
class FGTClippingBoxComponentVisualizer : public FComponentVisualizer
{
private:
	/** Draws widgets, such as the ClippingBox's bounds, within the editor. */
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
