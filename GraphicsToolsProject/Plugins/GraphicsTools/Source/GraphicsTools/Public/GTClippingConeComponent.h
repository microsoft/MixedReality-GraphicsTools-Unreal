// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTClippingPrimitiveComponent.h"

#include "GTClippingConeComponent.generated.h"

/**
 * Sub class of a ClippingPrimitive that represents an analytic cone used to pass state to materials for per pixel clipping.
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
