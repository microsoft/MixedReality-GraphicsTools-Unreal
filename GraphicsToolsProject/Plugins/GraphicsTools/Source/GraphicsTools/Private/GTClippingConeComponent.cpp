// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingConeComponent.h"

#include "GTWorldSubsystem.h"

UGTClippingConeComponent::UGTClippingConeComponent()
{
	{
		static const FName ParameterName("ClippingConeSettings");
		SetSettingsParameterName(ParameterName);
	}
	{
		static const FName ParameterNames[2] = {"ClippingConeStart", "ClippingConeEnd"};
		TArray<FName> Names;
		Names.Append(ParameterNames, UE_ARRAY_COUNT(ParameterNames));
		SetTransformColumnParameterNames(Names);
	}
}

TArray<UGTSceneComponent*>& UGTClippingConeComponent::GetWorldComponents()
{
	return GetWorld()->GetSubsystem<UGTWorldSubsystem>()->ClippingCones;
}

void UGTClippingConeComponent::UpdateParameterCollectionTransform()
{
	const FTransform& Transform = GetComponentTransform();

	FVector HalfAxis = Transform.GetScaledAxis(EAxis::X) * 0.5f;
	FVector Start = Transform.GetLocation() + HalfAxis;
	FVector End = Transform.GetLocation() - HalfAxis;
	FVector Scale = Transform.GetScale3D() * 0.5f;

	SetVectorParameterValue(GetTransformColumnParameterNames()[0], FLinearColor(Start.X, Start.Y, Start.Z, Scale.Z));
	SetVectorParameterValue(GetTransformColumnParameterNames()[1], FLinearColor(End.X, End.Y, End.Z, Scale.Y));
}
