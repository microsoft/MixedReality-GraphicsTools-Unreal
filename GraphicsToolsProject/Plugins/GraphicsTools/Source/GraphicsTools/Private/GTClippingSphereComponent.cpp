// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingSphereComponent.h"

UGTClippingSphereComponent::UGTClippingSphereComponent()
{
	{
		static FName ParameterName("ClippingSphereSettings");
		SetSettingsParameterName(ParameterName);
	}
	{
		static FName ParameterNames[4] = {
			"ClippingSphereTransformColumn0", "ClippingSphereTransformColumn1", "ClippingSphereTransformColumn2",
			"ClippingSphereTransformColumn3"};
		TArray<FName> Names;
		Names.Append(ParameterNames, UE_ARRAY_COUNT(ParameterNames));
		SetTransformColumnParameterNames(Names);
	}
}
