// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTClippingPrimitiveComponent.h"

#include "GTClippingBoxComponent.generated.h"

/**
 * Sub class of a ClippingPrimitive that represents an analytic box used to pass state to materials for per pixel clipping. A box's
 * transformation is described by 4x4 matrix that represents the box's location, rotation, and scale. A sphere can be non-uniformally
 * scaled along the x, y, or z axis. The box's dimension along the x, y, or z axis is determined by the magnitude of the
 * scale along each axis. A scale of one represents a unit box.
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
