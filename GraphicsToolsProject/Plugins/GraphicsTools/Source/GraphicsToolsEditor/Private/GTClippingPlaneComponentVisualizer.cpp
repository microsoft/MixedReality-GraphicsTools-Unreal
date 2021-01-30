// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingPlaneComponentVisualizer.h"

#include "GraphicsToolsEditor.h"

#include "Engine/Public/SceneManagement.h"

void FGTClippingPlaneComponentVisualizer::DrawVisualization(
	const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (const UGTClippingPlaneComponent* ClippingPlane = Cast<const UGTClippingPlaneComponent>(Component))
	{
		FMatrix LocalToWorld = ClippingPlane->GetComponentTransform().ToMatrixWithScale();

		DrawWireBox(PDI, LocalToWorld, FBox(FVector(0, -1, -1), FVector(0, 1, 1)), FColor::White, SDPG_World);

		DrawDirectionalArrow(
			PDI, LocalToWorld, FColor::White, 1 * (ClippingPlane->GetClippingSide() == EGTClippingSide::Inside) ? -1 : 1, 0.25, SDPG_World);
	}
}
