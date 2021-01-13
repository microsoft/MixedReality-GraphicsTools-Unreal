// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/WorldSubsystem.h"

#include "GTWorldSubsystem.generated.h"

class UGTDirectionalLightComponent;
class UGTProximityLightComponent;

/**
 * Subsystem to hold all graphics data associated with a world.
 */
UCLASS(ClassGroup = GraphicsTools)
class GRAPHICSTOOLS_API UGTWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** List of all DirectionalLights within a world. */
	TArray<UGTDirectionalLightComponent*> DirectionalLights;

	/** List of all ProximityLights within a world. */
	TArray<UGTProximityLightComponent*> ProximityLights;

protected:
	/** Allows editor preview worlds to have this subsystem. */
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
};
