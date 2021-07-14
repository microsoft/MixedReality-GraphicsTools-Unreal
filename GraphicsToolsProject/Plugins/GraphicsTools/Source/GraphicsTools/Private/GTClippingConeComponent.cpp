// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingConeComponent.h"

#include "GTWorldSubsystem.h"

#include "Engine/World.h"

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

	FVector HalfHeight = Transform.GetScaledAxis(EAxis::X) * 0.5f;
	FVector Top = Transform.GetLocation() + HalfHeight;
	FVector Bottom = Transform.GetLocation() - HalfHeight;
	FVector ScaleBottomTop = Transform.GetScale3D() * 0.5f;

	SetVectorParameterValue(GetTransformColumnParameterNames()[0], FLinearColor(Top.X, Top.Y, Top.Z, ScaleBottomTop.Z));
	SetVectorParameterValue(GetTransformColumnParameterNames()[1], FLinearColor(Bottom.X, Bottom.Y, Bottom.Z, ScaleBottomTop.Y));
}
