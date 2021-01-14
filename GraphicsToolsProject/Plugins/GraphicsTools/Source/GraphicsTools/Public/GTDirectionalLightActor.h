// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTLightActor.h"

#include "GTDirectionalLightActor.generated.h"

/**
 * Utility actor which automatically adds a UGTDirectionalLightComponent.
 */
UCLASS(ClassGroup = GraphicsTools)
class GRAPHICSTOOLS_API AGTDirectionalLightActor : public AGTLightActor
{
	GENERATED_BODY()

public:
	AGTDirectionalLightActor();
};
