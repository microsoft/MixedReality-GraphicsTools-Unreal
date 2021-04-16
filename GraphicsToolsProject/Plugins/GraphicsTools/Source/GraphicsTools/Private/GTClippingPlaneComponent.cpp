// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingPlaneComponent.h"

#include "GTWorldSubsystem.h"

UGTClippingPlaneComponent::UGTClippingPlaneComponent()
{
	{
		static const FName ParameterName("ClippingPlaneSettings");
		SetSettingsParameterName(ParameterName);
	}
	{
		static const FName ParameterNames[1] = {"ClippingPlane"};
		TArray<FName> Names;
		Names.Append(ParameterNames, UE_ARRAY_COUNT(ParameterNames));
		SetTransformColumnParameterNames(Names);
	}
}

TArray<UGTSceneComponent*>& UGTClippingPlaneComponent::GetWorldComponents()
{
	return GetWorld()->GetSubsystem<UGTWorldSubsystem>()->ClippingPlanes;
}

void UGTClippingPlaneComponent::UpdateParameterCollectionTransform()
{
	const FTransform& Tranform = GetComponentTransform();
	FVector Normal = Tranform.GetUnitAxis(EAxis::X);

	SetVectorParameterValue(
		GetTransformColumnParameterNames()[0],
		FLinearColor(Normal.X, Normal.Y, Normal.Z, FVector::DotProduct(Normal, Tranform.GetLocation())));
}
