// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTProximityLightActor.h"

#include "GTProximityLightComponent.h"

AGTProximityLightActor::AGTProximityLightActor()
{
	// Make the ProximityLight component the root component.
	LightComponent = CreateDefaultSubobject<UGTProximityLightComponent>(TEXT("ProximityLightComponent"));
	RootComponent = LightComponent;
}