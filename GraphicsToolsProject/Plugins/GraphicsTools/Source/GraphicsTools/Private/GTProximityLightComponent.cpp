// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTProximityLightComponent.h"

#include "GraphicsTools.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

UGTProximityLightComponent::UGTProximityLightComponent()
{
	bWantsOnUpdateTransform = true;

	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> Finder(TEXT("/GraphicsTools/Materials/MPC_GTSettings"));
	check(Finder.Object);
	ParameterCollection = Finder.Object;
}

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

void UGTProximityLightComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	if (ParameterCollection)
	{
		UMaterialParameterCollectionInstance* ParameterCollectionInstance = GetWorld()->GetParameterCollectionInstance(ParameterCollection);
		static FName ParameterNames[] = {"ProximityLightLocation0", "ProximityLightLocation1"};
		FName ParameterName = true ? ParameterNames[0] : ParameterNames[1];
		const bool bFoundParameter = ParameterCollectionInstance->SetVectorParameterValue(ParameterName, GetComponentLocation());

		if (!bFoundParameter)
		{
			UE_LOG(
				GraphicsTools, Warning, TEXT("Unable to find %s parameter in material parameter collection %s."), *ParameterName.ToString(),
				*ParameterCollection->GetPathName());
		}
	}
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