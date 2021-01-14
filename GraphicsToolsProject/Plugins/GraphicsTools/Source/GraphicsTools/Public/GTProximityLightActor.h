// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTLightActor.h"

#include "GTProximityLightActor.generated.h"

/**
 * Utility actor which automatically adds a UGTProximityLightComponent.
 */
UCLASS(ClassGroup = GraphicsTools)
class GRAPHICSTOOLS_API AGTProximityLightActor : public AGTLightActor
{
	GENERATED_BODY()

public:
	AGTProximityLightActor();
};
