// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingConeActor.h"

#include "GTClippingConeComponent.h"

AGTClippingConeActor::AGTClippingConeActor()
{
	// Make the ClippingCone component the root component.
	ClippingPrimitiveComponent = CreateDefaultSubobject<UGTClippingConeComponent>(TEXT("ClippingConeComponent"));
	RootComponent = ClippingPrimitiveComponent;

	// Give the ClippingCone a sane initial rotation & size.
	ClippingPrimitiveComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	ClippingPrimitiveComponent->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
}