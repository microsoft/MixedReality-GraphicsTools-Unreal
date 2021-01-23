// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "GTClippingPrimitiveActor.generated.h"

class UGTClippingPrimitiveComponent;

/**
 * Abstract base class for all Clipping Primitive actor types within Graphics Tools.
 */
UCLASS(Abstract, ClassGroup = GraphicsTools, hideCategories = (Input, Collision, Replication))
class GRAPHICSTOOLS_API AGTClippingPrimitiveActor : public AActor
{
	GENERATED_BODY()

public:
	AGTClippingPrimitiveActor();

protected:
	/** Handle to the clipping primitive component derived Graphics Tools clipping primitive actors instantiate. */
	UPROPERTY(Category = "GT Clipping Primitive", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UGTClippingPrimitiveComponent* ClippingPrimitiveComponent;
};
