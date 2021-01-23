// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTClippingPrimitiveActor.h"

#include "GTClippingBoxActor.generated.h"

/**
 * Utility actor which automatically adds a UGTClippingBoxComponent.
 */
UCLASS(ClassGroup = GraphicsTools)
class GRAPHICSTOOLS_API AGTClippingBoxActor : public AGTClippingPrimitiveActor
{
	GENERATED_BODY()

public:
	AGTClippingBoxActor();
};
