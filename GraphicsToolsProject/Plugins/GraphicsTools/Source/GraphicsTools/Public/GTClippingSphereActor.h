// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTClippingPrimitiveActor.h"

#include "GTClippingSphereActor.generated.h"

/**
 * Utility actor which automatically adds a UGTClippingSphereComponent.
 */
UCLASS(ClassGroup = GraphicsTools)
class GRAPHICSTOOLS_API AGTClippingSphereActor : public AGTClippingPrimitiveActor
{
	GENERATED_BODY()

public:
	AGTClippingSphereActor();
};
