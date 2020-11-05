// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTProximityLightComponent.h"

#include "GraphicsTools.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

TArray<UGTProximityLightComponent*> UGTProximityLightComponent::ProximityLights;

UGTProximityLightComponent::UGTProximityLightComponent()
{
	bWantsOnUpdateTransform = true;
}

void UGTProximityLightComponent::SetNearRaidus(float Radius)
{
	NearRadius = FMath::Clamp(Radius, 1.0f, 500.0f);

	if (NearRadius > FarRadius)
	{
		FarRadius = NearRadius;
	}

	UpdateParameterCollection(false, true);
}

void UGTProximityLightComponent::SetFarRaidus(float Radius)
{
	FarRadius = FMath::Clamp(Radius, 1.0f, 500.0f);

	if (FarRadius < NearRadius)
	{
		NearRadius = FarRadius;
	}

	UpdateParameterCollection(false, true);
}

void UGTProximityLightComponent::SetNearDistance(float Distance)
{
	MinNearSizePercentage = FMath::Clamp(Distance, 1.0f, 500.0f);

	UpdateParameterCollection(false, true);
}

void UGTProximityLightComponent::SetMinNearSizePercentage(float Percentage)
{
	Percentage = FMath::Clamp(Percentage, 0.0f, 1.0f);

	UpdateParameterCollection(false, true);
}

void UGTProximityLightComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	UpdateParameterCollection(true, false);
}

void UGTProximityLightComponent::OnRegister()
{
	Super::OnRegister();

	AddLight();
}

void UGTProximityLightComponent::OnUnregister()
{
	Super::OnUnregister();

	RemoveLight();
}

void UGTProximityLightComponent::OnVisibilityChanged()
{
	Super::OnVisibilityChanged();

	if (IsVisible())
	{
		AddLight();
	}
	else
	{
		RemoveLight();
	}

	UE_LOG(GraphicsTools, Warning, TEXT("OnVisibilityChanged %i"), IsVisible());
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

void UGTProximityLightComponent::AddLight()
{
	if (ProximityLights.Find(this) == INDEX_NONE)
	{
		ProximityLights.Add(this);
		UpdateParameterCollection(true, true);
	}
}

void UGTProximityLightComponent::RemoveLight()
{
	if (ProximityLights.Remove(this))
	{
		for (UGTProximityLightComponent* Light : ProximityLights)
		{
			Light->UpdateParameterCollection(true, true);
		}
	}
}

void SetVectorParameterValue(
	UMaterialParameterCollectionInstance* ParameterCollectionInstance, FName ParameterName, const FLinearColor& ParameterValue)
{
	const bool bFoundParameter = ParameterCollectionInstance->SetVectorParameterValue(ParameterName, ParameterValue);

	if (!bFoundParameter)
	{
		UE_LOG(
			GraphicsTools, Warning, TEXT("Unable to find %s parameter in material parameter collection %s."), *ParameterName.ToString(),
			*ParameterCollectionInstance->GetCollection()->GetPathName());
	}
}

void UGTProximityLightComponent::UpdateParameterCollection(bool LocationDirty, bool SettingsDirty)
{
	if (ParameterCollection)
	{
		int32 LightIndex = ProximityLights.Find(this);

		static const int32 MaxProximityLights = 2;

		if (LightIndex != INDEX_NONE && LightIndex < MaxProximityLights)
		{
			UMaterialParameterCollectionInstance* ParameterCollectionInstance =
				GetWorld()->GetParameterCollectionInstance(ParameterCollection);

			if (LocationDirty)
			{
				static FName ParameterNames[MaxProximityLights] = {"ProximityLightLocation0", "ProximityLightLocation1"};
				SetVectorParameterValue(ParameterCollectionInstance, ParameterNames[LightIndex], GetComponentLocation());
			}

			if (SettingsDirty)
			{
				static FName ParameterNames[MaxProximityLights] = {"ProximityLightSettings0", "ProximityLightSettings1"};
				SetVectorParameterValue(
					ParameterCollectionInstance, ParameterNames[LightIndex],
					FLinearColor(NearRadius, 1.0f / FarRadius, 1.0f / NearDistance, MinNearSizePercentage));
			}
		}
	}
}