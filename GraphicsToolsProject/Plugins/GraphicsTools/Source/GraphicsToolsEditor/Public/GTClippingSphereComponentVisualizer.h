// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "ComponentVisualizer.h"
#include "GTClippingSphereComponent.h"

/**
 * Editor visualization for the UGTClippingSphereComponent.
 */
class FGTClippingSphereComponentVisualizer : public FComponentVisualizer
{
private:
	/** Draws widgets, such as the ClippingSphere's radii, within the editor. */
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
