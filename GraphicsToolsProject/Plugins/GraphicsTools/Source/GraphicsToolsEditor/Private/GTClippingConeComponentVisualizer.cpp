// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingConeComponentVisualizer.h"

#include "GraphicsToolsEditor.h"

#include "Engine/Public/SceneManagement.h"

void FGTClippingConeComponentVisualizer::DrawVisualization(
	const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (const UGTClippingConeComponent* ClippingCone = Cast<const UGTClippingConeComponent>(Component))
	{
		const FTransform& Transform = ClippingCone->GetComponentTransform();
		FVector HalfHeight = Transform.GetScaledAxis(EAxis::X);
		FVector Scale = Transform.GetScale3D() * 0.5f;

		DrawWireChoppedCone(
			PDI, Transform.GetLocation(), Transform.GetUnitAxis(EAxis::Z), Transform.GetUnitAxis(EAxis::Y), Transform.GetUnitAxis(EAxis::X),
			FColor::White, Scale.Y, Scale.Z, Scale.X, 16, SDPG_World);
	}
}
