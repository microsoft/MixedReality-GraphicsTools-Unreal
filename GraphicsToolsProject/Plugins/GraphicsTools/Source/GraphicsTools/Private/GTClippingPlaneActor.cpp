// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingPlaneActor.h"

#include "GTClippingPlaneComponent.h"

AGTClippingPlaneActor::AGTClippingPlaneActor()
{
	// Make the ClippingPlane component the root component.
	ClippingPrimitiveComponent = CreateDefaultSubobject<UGTClippingPlaneComponent>(TEXT("ClippingPlaneComponent"));
	RootComponent = ClippingPrimitiveComponent;

	// Give the ClippingPlane a sane initial size.
	ClippingPrimitiveComponent->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));
}