// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTDirectionalLightActor.h"

#include "GTDirectionalLightComponent.h"

AGTDirectionalLightActor::AGTDirectionalLightActor()
{
	// Make the light component the root component.
	LightComponent = CreateDefaultSubobject<UGTDirectionalLightComponent>(TEXT("DirectionalLightComponent"));
	RootComponent = LightComponent;
}