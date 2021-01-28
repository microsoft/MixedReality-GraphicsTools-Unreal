// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTClippingPrimitiveComponent.h"

#include "GTClippingBoxComponent.generated.h"

/**
 * Sub class of a ClippingPrimitive that represents an analytic box used to pass state to materials for per pixel clipping.
 */
UCLASS(ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTClippingBoxComponent : public UGTClippingPrimitiveComponent
{
	GENERATED_BODY()
public:
	UGTClippingBoxComponent();

protected:
	//
	// UGTSceneComponent interface

	/** Accessor to all UGTClippingBoxComponent components within a world writing to the WorldParameterCollection. */
	virtual TArray<UGTSceneComponent*>& GetWorldComponents() override;
};
