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

		// Rotate the arrow's x-axis depending on the clipping side.
		LocalToWorld = (ClippingPlane->GetClippingSide() == EGTClippingSide::Inside) ? FRotationMatrix(FRotator(180, 0, 0)) * LocalToWorld
																					 : LocalToWorld;
		DrawDirectionalArrow(PDI, LocalToWorld, FColor::White, 1, 0.25, SDPG_World);
	}
}
