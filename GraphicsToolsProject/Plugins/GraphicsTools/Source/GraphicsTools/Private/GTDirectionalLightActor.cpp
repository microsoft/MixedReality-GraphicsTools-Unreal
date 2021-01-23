// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTDirectionalLightActor.h"

#include "GTDirectionalLightComponent.h"

AGTDirectionalLightActor::AGTDirectionalLightActor()
{
	// Make the DirectionalLight component the root component.
	LightComponent = CreateDefaultSubobject<UGTDirectionalLightComponent>(TEXT("DirectionalLightComponent"));
	RootComponent = LightComponent;

	// Give the light a "nice" initial rotation.
	LightComponent->SetRelativeRotation(FRotator(-46.0f, 0.0f, 0.0f));
}