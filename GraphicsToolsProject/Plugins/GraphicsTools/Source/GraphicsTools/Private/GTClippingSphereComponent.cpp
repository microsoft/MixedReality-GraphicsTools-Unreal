// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingSphereComponent.h"

#include "GTWorldSubsystem.h"

#include "Engine/World.h"

UGTClippingSphereComponent::UGTClippingSphereComponent()
{
	{
		static const FName ParameterName("ClippingSphereSettings");
		SetSettingsParameterName(ParameterName);
	}
	{
		static const FName ParameterNames[4] = {
			"ClippingSphereTransformColumn0", "ClippingSphereTransformColumn1", "ClippingSphereTransformColumn2",
			"ClippingSphereTransformColumn3"};
		TArray<FName> Names;
		Names.Append(ParameterNames, UE_ARRAY_COUNT(ParameterNames));
		SetTransformColumnParameterNames(Names);
	}
}

TArray<UGTSceneComponent*>& UGTClippingSphereComponent::GetWorldComponents()
{
	return GetWorld()->GetSubsystem<UGTWorldSubsystem>()->ClippingSpheres;
}
