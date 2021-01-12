// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTProximityLightComponent.h"

#include "GraphicsTools.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

#define GT_MAX_PROXIMITY_LIGHTS 4

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
	typedef TArray<TWeakObjectPtr<UGTProximityLightComponent>> LightList;

	LightList& GetLightList(bool bInGameWorld) { return bInGameWorld ? GameLights : EditorLights; }

	void AddLight(UGTProximityLightComponent* Light)
	{
		if (!UGTLightComponent::ValidLight(Light))
		{
			return;
		}

		LightList& Lights = GetLightList(Light->GetWorld()->IsGameWorld());

		if (Lights.Find(Light) == INDEX_NONE)
		{
			Lights.Add(Light);
			UpdateParameterCollection(Light, EParameterCollectionFlags::AllDirty);
		}
	}

	void RemoveLight(UGTProximityLightComponent* Light)
	{
		if (!UGTLightComponent::ValidLight(Light))
		{
			return;
		}

		LightList& Lights = GetLightList(Light->GetWorld()->IsGameWorld());
		const int32 Index = Lights.Find(Light);

		if (Index != INDEX_NONE)
		{
			// Disable the last active light.
			UpdateParameterCollection(Lights[Lights.Num() - 1].Get(), EParameterCollectionFlags::NoneDirty);

			Lights.RemoveAt(Index);

			for (auto CurrentLight : Lights)
			{
				UpdateParameterCollection(CurrentLight.Get(), EParameterCollectionFlags::AllDirty);
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
		if (!UGTLightComponent::ValidLight(Light))
		{
			return;
		}

		const LightList& Lights = GetLightList(Light->GetWorld()->IsGameWorld());
		const int32 LightIndex = Lights.Find(Light);

		if (LightIndex != INDEX_NONE && LightIndex < GT_MAX_PROXIMITY_LIGHTS)
		{
			UMaterialParameterCollectionInstance* ParameterCollectionInstance =
				Light->GetWorld()->GetParameterCollectionInstance(Light->GetParameterCollection());

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::LocationDirty) || DirtyFlags == EParameterCollectionFlags::NoneDirty)
			{
				static FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightLocation0", "ProximityLightLocation1", "ProximityLightLocation2", "ProximityLightLocation3"};
				FLinearColor Location(Light->GetComponentLocation());
				Location.A = DirtyFlags == EParameterCollectionFlags::NoneDirty ? 0.0f : 1.0f;
				SetVectorParameterValue(ParameterCollectionInstance, ParameterNames[LightIndex], Location);
			}

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::SettingsDirty))
			{
				static FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightSettings0", "ProximityLightSettings1", "ProximityLightSettings2", "ProximityLightSettings3"};
				const float PulseScaler = 1.0f + Light->GetPulseTime();
				SetVectorParameterValue(
					ParameterCollectionInstance, ParameterNames[LightIndex],
					FLinearColor(
						Light->GetProjectedRadius() * PulseScaler, 1.0f / Light->GetAttenuationRadius() * PulseScaler,
						1.0f / Light->GetShrinkDistance() * PulseScaler, Light->GetShrinkPercentage()));
			}

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::PulseSettingsDirty))
			{
				static FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightPulseSettings0", "ProximityLightPulseSettings1", "ProximityLightPulseSettings2",
					"ProximityLightPulseSettings3"};
				SetVectorParameterValue(
					ParameterCollectionInstance, ParameterNames[LightIndex],
					FLinearColor(Light->GetProjectedRadius() * Light->GetPulseTime(), 1.0f - Light->GetPulseFadeTime(), 0.0f, 0.0f));
			}

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::CenterColorDirty))
			{
				static FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightCenterColor0", "ProximityLightCenterColor1", "ProximityLightCenterColor2", "ProximityLightCenterColor3"};
				SetVectorParameterValue(ParameterCollectionInstance, ParameterNames[LightIndex], Light->GetCenterColor());
			}

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::MiddleColorDirty))
			{
				static FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightMiddleColor0", "ProximityLightMiddleColor1", "ProximityLightMiddleColor2", "ProximityLightMiddleColor3"};
				SetVectorParameterValue(ParameterCollectionInstance, ParameterNames[LightIndex], Light->GetMiddleColor());
			}

			if (EnumHasAnyFlags(DirtyFlags, EParameterCollectionFlags::OuterColorDirty))
			{
				static FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightOuterColor0", "ProximityLightOuterColor1", "ProximityLightOuterColor2", "ProximityLightOuterColor3"};
				SetVectorParameterValue(ParameterCollectionInstance, ParameterNames[LightIndex], Light->GetOuterColor());
			}
		}
	}

private:
	LightList GameLights;
	LightList EditorLights;
} GlobalProximityLights;

UGTProximityLightComponent::UGTProximityLightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// Don't start ticking until the light needs to be animated.
	PrimaryComponentTick.bStartWithTickEnabled = false;

	bWantsOnUpdateTransform = true;
}

void UGTProximityLightComponent::SetProjectedRadius(float Radius)
{
	if (ProjectedRadius != Radius)
	{
		ProjectedRadius = FMath::Clamp(Radius, 1.0f, 500.0f);

		if (ProjectedRadius > AttenuationRadius)
		{
			AttenuationRadius = ProjectedRadius;
		}

		GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::SettingsDirty);
	}
}

void UGTProximityLightComponent::SetAttenuationRadius(float Radius)
{
	if (AttenuationRadius != Radius)
	{
		AttenuationRadius = FMath::Clamp(Radius, 1.0f, 500.0f);

		if (AttenuationRadius < ProjectedRadius)
		{
			ProjectedRadius = AttenuationRadius;
		}

		GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::SettingsDirty);
	}
}

void UGTProximityLightComponent::SetShrinkDistance(float Distance)
{
	if (ShrinkDistance != Distance)
	{
		ShrinkDistance = FMath::Clamp(Distance, 1.0f, 500.0f);

		GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::SettingsDirty);
	}
}

void UGTProximityLightComponent::SetShrinkPercentage(float Percentage)
{
	if (ShrinkPercentage != Percentage)
	{
		ShrinkPercentage = FMath::Clamp(Percentage, 0.0f, 1.0f);

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

void UGTProximityLightComponent::Pulse(float Duration, float FadeOffset, float FadeDuration)
{
	if (PulseState == EPulseState::Idle && Duration > 0)
	{
		PulseState = EPulseState::Animate;
		PulseTimer = 0;
		PulseFadeTimer = 0;
		PulseDuration = Duration;
		PulseFadeOffset = FadeOffset;
		PulseFadeDuration = FadeDuration;

		SetComponentTickEnabled(true);
	}
}

float UGTProximityLightComponent::GetPulseTime() const
{
	if (PulseDuration > 0)
	{
		return FMath::Clamp(PulseTimer / PulseDuration, 0.0f, 1.0f);
	}

	return 0;
}

float UGTProximityLightComponent::GetPulseFadeTime() const
{
	if (PulseFadeDuration > 0)
	{
		return FMath::Clamp(PulseFadeTimer / PulseFadeDuration, 0.0f, 1.0f);
	}

	return 0;
}

void UGTProximityLightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PulseState = PulseTick(DeltaTime);
	GlobalProximityLights.UpdateParameterCollection(
		this, EParameterCollectionFlags::SettingsDirty | EParameterCollectionFlags::PulseSettingsDirty);

	if (PulseState == EPulseState::Idle)
	{
		SetComponentTickEnabled(false);
	}
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

void UGTProximityLightComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::LocationDirty);
}

#if WITH_EDITOR
void UGTProximityLightComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTProximityLightComponent, ProjectedRadius))
	{
		if (ProjectedRadius > AttenuationRadius)
		{
			AttenuationRadius = ProjectedRadius;
		}
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTProximityLightComponent, AttenuationRadius))
	{
		if (AttenuationRadius < ProjectedRadius)
		{
			ProjectedRadius = AttenuationRadius;
		}
	}

	GlobalProximityLights.UpdateParameterCollection(this, EParameterCollectionFlags::AllDirty);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

EPulseState UGTProximityLightComponent::PulseTick(float DeltaTime)
{
	switch (PulseState)
	{
	case EPulseState::Animate:
	{
		// Animate the PulseTime from 0 to PulseDuration.
		if (PulseTimer < PulseDuration)
		{
			PulseTimer = FMath::Clamp(PulseTimer + DeltaTime, 0.0f, PulseDuration);

			// Fade in the pulse.
			if (PulseTimer >= PulseFadeOffset && PulseFadeTimer < PulseFadeDuration)
			{
				PulseFadeTimer = FMath::Clamp(PulseFadeTimer + DeltaTime, 0.0f, PulseFadeDuration);
			}

			return EPulseState::Animate;
		}

		PulseTimer = PulseDuration;
		return EPulseState::FadeIn;
	}
	case EPulseState::FadeIn:
	{
		// Continue to fade in the pulse, animate the PulseFade to PulseFadeDuration (if it's not already there).
		if (PulseFadeTimer < PulseFadeDuration)
		{
			PulseFadeTimer = FMath::Clamp(PulseFadeTimer + DeltaTime, 0.0f, PulseFadeDuration);

			return EPulseState::FadeIn;
		}

		// Reset the pulse once completely faded.
		PulseTimer = 0;
		return EPulseState::FadeOut;
	}
	case EPulseState::FadeOut:
	{
		// Fade out the pulse, animate the PulseFadeTimer to 0.
		if (PulseFadeTimer > 0)
		{
			PulseFadeTimer = FMath::Clamp(PulseFadeTimer - DeltaTime, 0.0f, PulseFadeDuration);

			return EPulseState::FadeOut;
		}

		// The animation is complete.
		PulseFadeTimer = 0;

		return EPulseState::Idle;
	}
	default:
		return EPulseState::Idle;
	}
}
