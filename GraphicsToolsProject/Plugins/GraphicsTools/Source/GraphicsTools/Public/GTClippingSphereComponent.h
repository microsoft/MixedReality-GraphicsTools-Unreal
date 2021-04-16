// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTClippingPrimitiveComponent.h"

#include "GTClippingSphereComponent.generated.h"

/**
 * Sub class of a ClippingPrimitive that represents an analytic sphere used to pass state to materials for per pixel clipping. A sphere's
 * transformation is described by 4x4 matrix that represents the sphere's location, rotation, and scale. A sphere can be non-uniformally
 * scaled along the x, y, or z axis to become an ellipsoid. The sphere's dimension along the x, y, or z axis is determined by the magnitude
 * of the scale along each axis. A scale of one represents a unit sphere.
 */
UCLASS(ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTClippingSphereComponent : public UGTClippingPrimitiveComponent
{
	GENERATED_BODY()
public:
	UGTClippingSphereComponent();

protected:
	//
	// UGTSceneComponent interface

	/** Accessor to all UGTClippingSphereComponent components within a world writing to the WorldParameterCollection. */
	virtual TArray<UGTSceneComponent*>& GetWorldComponents() override;
};
