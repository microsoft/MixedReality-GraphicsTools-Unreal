// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTSceneComponent.h"

#include "GTLightComponent.generated.h"

class UMaterialParameterCollection;

/**
 * Abstract base class for all light actor components within Graphics Tools.
 */
UCLASS(Abstract, ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTLightComponent : public UGTSceneComponent
{
	GENERATED_BODY()

public:
	UGTLightComponent();
};
