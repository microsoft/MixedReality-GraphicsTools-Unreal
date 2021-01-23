// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingSphereActor.h"

#include "GTClippingSphereComponent.h"

AGTClippingSphereActor::AGTClippingSphereActor()
{
	// Make the ClippingSphere component the root component.
	ClippingPrimitiveComponent = CreateDefaultSubobject<UGTClippingSphereComponent>(TEXT("ClippingSphereComponent"));
	RootComponent = ClippingPrimitiveComponent;

	// Give the ClippingSphere a sane initial size.
	ClippingPrimitiveComponent->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
}