// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTClippingPrimitiveComponent.h"

#include "GTClippingPlaneComponent.generated.h"

/**
 * Sub class of a ClippingPrimitive that represents an analytic plane used to pass state to materials for per pixel clipping. A plane's
 * transformation is described by a normal and distance from the origin along that normal. The normal is the x-axis of the component. The
 * distance is calculated based on the component's world location.
 */
UCLASS(ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTClippingPlaneComponent : public UGTClippingPrimitiveComponent
{
	GENERATED_BODY()
public:
	UGTClippingPlaneComponent();

protected:
	//
	// UGTSceneComponent interface

	/** Accessor to all UGTClippingPlaneComponent components within a world writing to the WorldParameterCollection. */
	virtual TArray<UGTSceneComponent*>& GetWorldComponents() override;

	//
	// UGTClippingPrimitiveComponent interface

	/** Sends the plane's normal and distance from the origin into the current parameter collection. */
	virtual void UpdateParameterCollectionTransform() override;

	/** Plane's only need single FVector to specify their transform. */
	virtual int32 GetTransformColumnCount() const override { return 1; }
};
