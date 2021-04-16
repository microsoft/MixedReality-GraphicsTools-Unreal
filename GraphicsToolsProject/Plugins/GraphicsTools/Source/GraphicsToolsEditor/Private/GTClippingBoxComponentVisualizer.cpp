// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingBoxComponentVisualizer.h"

#include "GraphicsToolsEditor.h"

#include "Engine/Public/SceneManagement.h"

void FGTClippingBoxComponentVisualizer::DrawVisualization(
	const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (const UGTClippingBoxComponent* ClippingBox = Cast<const UGTClippingBoxComponent>(Component))
	{
		DrawWireBox(
			PDI, ClippingBox->GetComponentTransform().ToMatrixWithScale(), FBox(FVector(-1, -1, -1), FVector(1, 1, 1)), FColor::White,
			SDPG_World);
	}
}
