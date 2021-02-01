// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/WorldSubsystem.h"

#include "GTWorldSubsystem.generated.h"

class UGTSceneComponent;

/**
 * Subsystem to hold all graphics data associated with a world that will effect the world's MPC_GTSettings material parameter collection.
 */
UCLASS(ClassGroup = GraphicsTools)
class GRAPHICSTOOLS_API UGTWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** List of all DirectionalLights within a world. */
	TArray<UGTSceneComponent*> DirectionalLights;

	/** List of all ProximityLights within a world. */
	TArray<UGTSceneComponent*> ProximityLights;

	/** List of all ClippingPlanes within a world. */
	TArray<UGTSceneComponent*> ClippingPlanes;

	/** List of all ClippingSpheres within a world. */
	TArray<UGTSceneComponent*> ClippingSpheres;

	/** List of all ClippingBoxes within a world. */
	TArray<UGTSceneComponent*> ClippingBoxes;

	/** List of all ClippingCones within a world. */
	TArray<UGTSceneComponent*> ClippingCones;

protected:
	/** Allows editor preview worlds to have this subsystem. */
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
};
