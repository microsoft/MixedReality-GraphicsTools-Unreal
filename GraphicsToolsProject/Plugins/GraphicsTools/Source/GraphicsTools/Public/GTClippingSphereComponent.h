// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTClippingPrimitiveComponent.h"

#include "GTClippingSphereComponent.generated.h"

/**
 * Sub class of a ClippingPrimitive that represents an analytic sphere used to pass state to materials for per pixel clipping.
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
