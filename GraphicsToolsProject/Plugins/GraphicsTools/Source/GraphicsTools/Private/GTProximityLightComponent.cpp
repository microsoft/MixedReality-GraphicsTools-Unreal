// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTProximityLightComponent.h"

void UGTProximityLightComponent::SetNearRaidus(float Radius)
{
	NearRadius = FMath::Clamp(Radius, 1.0f, 500.0f);

	if (NearRadius > FarRadius)
	{
		FarRadius = NearRadius;
	}
}

void UGTProximityLightComponent::SetFarRaidus(float Radius)
{
	FarRadius = FMath::Clamp(Radius, 1.0f, 500.0f);

	if (FarRadius < NearRadius)
	{
		NearRadius = FarRadius;
	}
}

void UGTProximityLightComponent::SetNearDistance(float Distance)
{
	NearDistance = FMath::Clamp(Distance, 1.0f, 500.0f);
}

void UGTProximityLightComponent::SetMinNearSizePercentage(float Percentage)
{
	Percentage = FMath::Clamp(Percentage, 0.0f, 1.0f);
}

#if WITH_EDITOR
void UGTProximityLightComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTProximityLightComponent, NearRadius))
	{
		if (NearRadius > FarRadius)
		{
			FarRadius = NearRadius;
		}
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTProximityLightComponent, FarRadius))
	{
		if (FarRadius < NearRadius)
		{
			NearRadius = FarRadius;
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif