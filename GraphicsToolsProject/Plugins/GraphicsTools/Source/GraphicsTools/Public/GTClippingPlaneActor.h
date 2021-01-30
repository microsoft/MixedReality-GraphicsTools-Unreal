// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTClippingPrimitiveActor.h"

#include "GTClippingPlaneActor.generated.h"

/**
 * Utility actor which automatically adds a UGTClippingPlaneComponent.
 */
UCLASS(ClassGroup = GraphicsTools)
class GRAPHICSTOOLS_API AGTClippingPlaneActor : public AGTClippingPrimitiveActor
{
	GENERATED_BODY()

public:
	AGTClippingPlaneActor();
};
