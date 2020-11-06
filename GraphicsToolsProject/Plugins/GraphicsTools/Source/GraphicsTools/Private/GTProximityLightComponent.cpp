// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTProximityLightComponent.h"

#include "GraphicsTools.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

enum class EParameterCollectionFlags : uint8
{
	NoneDirty = 0,
	LocationDirty = 1 << 2,
	SettingsDirty = 1 << 3,
	PulseSettingsDirty = 1 << 4,
	CenterColorDirty = 1 << 5,
	MiddleColorDirty = 1 << 6,
	OuterColorDirty = 1 << 7,

	AllDirty = static_cast<uint8>(~0)
};
ENUM_CLASS_FLAGS(EParameterCollectionFlags);

struct ProximityLights
{
public:
	void AddLight(UGTProximityLightComponent* Light)
	{
		if (Lights.Find(Light) == INDEX_NONE)
		{
			Lights.Add(Light);
			UpdateParameterCollection(Light, EParameterCollectionFlags::AllDirty);
		}
	}

	void RemoveLight(UGTProximityLightComponent* Light)
	{
		int32 Index = Lights.Find(Light);

		if (Index != INDEX_NONE)
		{
			// Disable the last active light.
			UpdateParameterCollection(Lights[Lights.Num() - 1], EParameterCollectionFlags::NoneDirty);

			Lights.RemoveAt(Index);

			for (UGTProximityLightComponent* CurrentLight : Lights)
			{
				UpdateParameterCollection(CurrentLight, EParameterCollectionFlags::AllDirty);
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

	void UpdateParameterCollection(UGTProximityLightComponent* Light, EParameterCollectionFlags DirtyFlags)
	{
		// Ensure the light's world isn't being destroyed. If a light is in a world being destroyed the ParameterCollectionInstance will be
		// invalid.
		if (Light->GetParameterCollection() == nullptr || Light->GetWorld()->HasAnyFlags(RF_BeginDestroyed))
		{
			return;
		}

		static const int32 MaxProximityLights = 3;

		int32 LightIndex = Lights.Find(Light);

		if (LightIndex != INDEX_NONE && LightIndex < MaxProximityLights)
		{
			UMaterialParameterCollectionInstance* ParameterCollectionInstance =
				Light->GetWorld()->GetParameterCollectionInstance(Light->GetParameterCollection());

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::LocationDirty) || DirtyFlags == EParameterCollectionFlags::NoneDirty)
			{
				static FName ParameterNames[MaxProximityLights] = {
					"ProximityLightLocation0", "ProximityLightLocation1", "ProximityLightLocation2"};
				FLinearColor Location(Light->GetComponentLocation());
				Location.A = DirtyFlags == EParameterCollectionFlags::NoneDirty ? 0.0f : 1.0f;
				SetVectorParameterValue(ParameterCollectionInstance, ParameterNames[LightIndex], Location);
			}

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::SettingsDirty))
			{
				static FName ParameterNames[MaxProximityLights] = {
					"ProximityLightSettings0", "ProximityLightSettings1", "ProximityLightSettings2"};
				SetVectorParameterValue(
					ParameterCollectionInstance, ParameterNames[LightIndex],
					FLinearColor(
						Light->GetNearRaidus(), 1.0f / Light->GetFarRaidus(), 1.0f / Light->GetNearDistance(),
						Light->GetMinNearSizePercentage()));
			}

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::PulseSettingsDirty))
			{
				static FName ParameterNames[MaxProximityLights] = {
					"ProximityLightPulseSettings0", "ProximityLightPulseSettings1", "ProximityLightPulseSettings2"};
				SetVectorParameterValue(
					ParameterCollectionInstance, ParameterNames[LightIndex], FLinearColor(0.0f, 1.0f, 0.0f, 0.0f)); // TODO
			}

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::CenterColorDirty))
			{
				static FName ParameterNames[MaxProximityLights] = {
					"ProximityLightCenterColor0", "ProximityLightCenterColor1", "ProximityLightCenterColor2"};
				SetVectorParameterValue(ParameterCollectionInstance, ParameterNames[LightIndex], Light->GetCenterColor());
			}

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::MiddleColorDirty))
			{
				static FName ParameterNames[MaxProximityLights] = {
					"ProximityLightMiddleColor0", "ProximityLightMiddleColor1", "ProximityLightMiddleColor2"};
				SetVectorParameterValue(ParameterCollectionInstance, ParameterNames[LightIndex], Light->GetMiddleColor());
			}

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::OuterColorDirty))
			{
				static FName ParameterNames[MaxProximityLights] = {
					"ProximityLightOuterColor0", "ProximityLightOuterColor1", "ProximityLightOuterColor2"};
				SetVectorParameterValue(ParameterCollectionInstance, ParameterNames[LightIndex], Light->GetOuterColor());
			}
		}
	}

private:
	TArray<UGTProximityLightComponent*> Lights;
} GlobalProximityLights;

UGTProximityLightComponent::UGTProximityLightComponent()
{
	bWantsOnUpdateTransform = true;
}

void UGTProximityLightComponent::SetNearRaidus(float Radius)
{
	if (NearRadius != Radius)
	{
		NearRadius = FMath::Clamp(Radius, 1.0f, 500.0f);

		if (NearRadius > FarRadius)
		{
			FarRadius = NearRadius;
		}

		GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::SettingsDirty);
	}
}

void UGTProximityLightComponent::SetFarRaidus(float Radius)
{
	if (FarRadius != Radius)
	{
		FarRadius = FMath::Clamp(Radius, 1.0f, 500.0f);

		if (FarRadius < NearRadius)
		{
			NearRadius = FarRadius;
		}

		GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::SettingsDirty);
	}
}

void UGTProximityLightComponent::SetNearDistance(float Distance)
{
	if (NearDistance != Distance)
	{
		NearDistance = FMath::Clamp(Distance, 1.0f, 500.0f);

		GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::SettingsDirty);
	}
}

void UGTProximityLightComponent::SetMinNearSizePercentage(float Percentage)
{
	if (MinNearSizePercentage != Percentage)
	{
		MinNearSizePercentage = FMath::Clamp(Percentage, 0.0f, 1.0f);

		GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::SettingsDirty);
	}
}

void UGTProximityLightComponent::SetCenterColor(FColor Color)
{
	if (CenterColor != Color)
	{
		CenterColor = Color;

		GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::CenterColorDirty);
	}
}

void UGTProximityLightComponent::SetMiddleColor(FColor Color)
{
	if (MiddleColor != Color)
	{
		MiddleColor = Color;

		GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::MiddleColorDirty);
	}
}

void UGTProximityLightComponent::SetOuterColor(FColor Color)
{
	if (OuterColor != Color)
	{
		OuterColor = Color;

		GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::OuterColorDirty);
	}
}

void UGTProximityLightComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::LocationDirty);
}

void UGTProximityLightComponent::OnRegister()
{
	Super::OnRegister();

	if (IsVisible())
	{
		GlobalProximityLights.AddLight(this);
	}
}

void UGTProximityLightComponent::OnUnregister()
{
	Super::OnUnregister();

	GlobalProximityLights.RemoveLight(this);
}

void UGTProximityLightComponent::OnVisibilityChanged()
{
	Super::OnVisibilityChanged();

	if (IsVisible())
	{
		GlobalProximityLights.AddLight(this);
	}
	else
	{
		GlobalProximityLights.RemoveLight(this);
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

	GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::AllDirty);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
