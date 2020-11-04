// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "GTLightActor.generated.h"

class UGTLightComponent;

/**
 * TODO
 */
UCLASS(Abstract, ClassGroup = GraphicsTools, hideCategories = (Input, Collision, Replication))
class GRAPHICSTOOLS_API AGTLightActor : public AActor
{
	GENERATED_BODY()

public:
	AGTLightActor();

protected:
	UPROPERTY(Category = "GT Light", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UGTLightComponent* LightComponent;
};
