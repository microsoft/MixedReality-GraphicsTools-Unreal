// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingSphereComponentVisualizer.h"

#include "GraphicsToolsEditor.h"

#include "Engine/Public/SceneManagement.h"

void FGTClippingSphereComponentVisualizer::DrawVisualization(
	const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (const UGTClippingSphereComponent* ClippingSphere = Cast<const UGTClippingSphereComponent>(Component))
	{
		DrawWireSphereAutoSides(PDI, ClippingSphere->GetComponentTransform(), FColor::White, 1, SDPG_World);
	}
}
