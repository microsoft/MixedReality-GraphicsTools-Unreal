// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTSceneComponent.h"

#include "GTClippingPrimitiveComponent.generated.h"

class UMaterialParameterCollection;

/**
 * Abstract base class for all ClippingPrimitive actor components within Graphics Tools.
 */
UCLASS(Abstract, ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTClippingPrimitiveComponent : public UGTSceneComponent
{
	GENERATED_BODY()

public:
	UGTClippingPrimitiveComponent();

protected:
	//
	// UActorComponent interface

	/** Adds the ClippingPrimitive to the material parameter collection. */
	virtual void OnRegister() override;

	/** Removes the ClippingPrimitive from the material parameter collection. */
	virtual void OnUnregister() override;

	/** Adds or removes the ClippingPrimitive from the material parameter collection. */
	virtual void OnVisibilityChanged() override;

	//
	// USceneComponent interface

	/** Notifies systems of the ClippingPrimitive's new transformation. */
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport = ETeleportType::None) override;

#if WITH_EDITOR
	/** Notifies systems of the ClippingPrimitive's new transformation when edited in the details panel. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
