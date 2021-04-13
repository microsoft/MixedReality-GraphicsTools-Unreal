// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTClippingPrimitiveComponent.h"

#include "GTClippingConeComponent.generated.h"

/**
 * Sub class of a ClippingPrimitive that represents an analytic cone used to pass state to materials for per pixel clipping. A cone's
 * transformation is described by two points and two radii. The points represent the top and bottom faces of the cone and the two radii
 * represent the radius of those two faces. The orientation and height of the cone is described by the vector between the top and bottom
 * points direction and magnitude respectively. To change the height, top radius, or bottom radius of the cone adjust the scale of the
 * component. The scale along the x-axis is the height, and scale along y-axis is the bottom radius, and the scale along the z-axis is the
 * top radius. If the y and z axis scales are the same the cone becomes a capped cylinder.
 */
UCLASS(ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTClippingConeComponent : public UGTClippingPrimitiveComponent
{
	GENERATED_BODY()
public:
	UGTClippingConeComponent();

protected:
	//
	// UGTSceneComponent interface

	/** Accessor to all UGTClippingConeComponent components within a world writing to the WorldParameterCollection. */
	virtual TArray<UGTSceneComponent*>& GetWorldComponents() override;

	//
	// UGTClippingPrimitiveComponent interface

	/** Sends the cone's end points and radii into the current parameter collection. */
	virtual void UpdateParameterCollectionTransform() override;

	/** Cone's only need two FVectors (two points and two radii) to specify their transform. */
	virtual int32 GetTransformColumnCount() const override { return 2; }
};
