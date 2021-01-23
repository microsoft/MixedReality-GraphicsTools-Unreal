// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingBoxActor.h"

#include "GTClippingBoxComponent.h"

AGTClippingBoxActor::AGTClippingBoxActor()
{
	// Make the ClippingBox component the root component.
	ClippingPrimitiveComponent = CreateDefaultSubobject<UGTClippingBoxComponent>(TEXT("ClippingBoxComponent"));
	RootComponent = ClippingPrimitiveComponent;

	// Give the ClippingBox a sane initial size.
	ClippingPrimitiveComponent->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
}