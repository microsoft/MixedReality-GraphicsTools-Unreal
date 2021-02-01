// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTClippingPrimitiveActor.h"

#include "GTClippingConeActor.generated.h"

/**
 * Utility actor which automatically adds a UGTClippingConeComponent.
 */
UCLASS(ClassGroup = GraphicsTools)
class GRAPHICSTOOLS_API AGTClippingConeActor : public AGTClippingPrimitiveActor
{
	GENERATED_BODY()

public:
	AGTClippingConeActor();
};
