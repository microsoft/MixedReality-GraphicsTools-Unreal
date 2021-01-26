// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingBoxComponent.h"

UGTClippingBoxComponent::UGTClippingBoxComponent()
{
	{
		static FName ParameterName("ClippingBoxSettings");
		SetSettingsParameterName(ParameterName);
	}
	{
		static FName ParameterNames[4] = {
			"ClippingBoxTransformColumn0", "ClippingBoxTransformColumn1", "ClippingBoxTransformColumn2", "ClippingBoxTransformColumn3"};
		TArray<FName> Names;
		Names.Append(ParameterNames, UE_ARRAY_COUNT(ParameterNames));
		SetTransformColumnParameterNames(Names);
	}
}
