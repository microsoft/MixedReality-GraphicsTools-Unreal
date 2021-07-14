// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingBoxComponent.h"

#include "GTWorldSubsystem.h"

#include "Engine/World.h"

UGTClippingBoxComponent::UGTClippingBoxComponent()
{
	{
		static const FName ParameterName("ClippingBoxSettings");
		SetSettingsParameterName(ParameterName);
	}
	{
		static const FName ParameterNames[4] = {
			"ClippingBoxTransformColumn0", "ClippingBoxTransformColumn1", "ClippingBoxTransformColumn2", "ClippingBoxTransformColumn3"};
		TArray<FName> Names;
		Names.Append(ParameterNames, UE_ARRAY_COUNT(ParameterNames));
		SetTransformColumnParameterNames(Names);
	}
}

TArray<UGTSceneComponent*>& UGTClippingBoxComponent::GetWorldComponents()
{
	return GetWorld()->GetSubsystem<UGTWorldSubsystem>()->ClippingBoxes;
}
