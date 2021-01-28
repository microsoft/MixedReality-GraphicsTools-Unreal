// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTProximityLightComponent.h"

#include "GTWorldSubsystem.h"

#define GT_MAX_PROXIMITY_LIGHTS 4

UGTProximityLightComponent::UGTProximityLightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// Don't start ticking until the light needs to be animated.
	PrimaryComponentTick.bStartWithTickEnabled = false;
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

		UpdateParameterCollection();
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

		UpdateParameterCollection();
	}
}

void UGTProximityLightComponent::SetShrinkDistance(float Distance)
{
	if (ShrinkDistance != Distance)
	{
		ShrinkDistance = FMath::Clamp(Distance, 1.0f, 500.0f);

		UpdateParameterCollection();
	}
}

void UGTProximityLightComponent::SetShrinkPercentage(float Percentage)
{
	if (ShrinkPercentage != Percentage)
	{
		ShrinkPercentage = FMath::Clamp(Percentage, 0.0f, 1.0f);

		UpdateParameterCollection();
	}
}

void UGTProximityLightComponent::SetCenterColor(FColor Color)
{
	if (CenterColor != Color)
	{
		CenterColor = Color;

		UpdateParameterCollection();
	}
}

void UGTProximityLightComponent::SetMiddleColor(FColor Color)
{
	if (MiddleColor != Color)
	{
		MiddleColor = Color;

		UpdateParameterCollection();
	}
}

void UGTProximityLightComponent::SetOuterColor(FColor Color)
{
	if (OuterColor != Color)
	{
		OuterColor = Color;

		UpdateParameterCollection();
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
	UpdateParameterCollection();

	if (PulseState == EPulseState::Idle)
	{
		SetComponentTickEnabled(false);
	}
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

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

TArray<UGTSceneComponent*>& UGTProximityLightComponent::GetWorldComponents()
{
	return GetWorld()->GetSubsystem<UGTWorldSubsystem>()->ProximityLights;
}

void UGTProximityLightComponent::UpdateParameterCollection(bool IsDisabled)
{
	if (IsValid())
	{
		const TArray<UGTSceneComponent*>& Components = GetWorldComponents();
		const int32 ComponentIndex = Components.Find(this);

		if (ComponentIndex != INDEX_NONE && ComponentIndex < GT_MAX_PROXIMITY_LIGHTS)
		{
			{
				static const FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightLocation0", "ProximityLightLocation1", "ProximityLightLocation2", "ProximityLightLocation3"};
				FLinearColor Location(GetComponentLocation());
				Location.A = !IsDisabled;
				SetVectorParameterValue(ParameterNames[ComponentIndex], Location);
			}
			{
				static const FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightSettings0", "ProximityLightSettings1", "ProximityLightSettings2", "ProximityLightSettings3"};
				const float PulseScaler = 1.0f + GetPulseTime();
				SetVectorParameterValue(
					ParameterNames[ComponentIndex], FLinearColor(
														GetProjectedRadius() * PulseScaler, 1.0f / GetAttenuationRadius() * PulseScaler,
														1.0f / GetShrinkDistance() * PulseScaler, GetShrinkPercentage()));
			}
			{
				static const FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightPulseSettings0", "ProximityLightPulseSettings1", "ProximityLightPulseSettings2",
					"ProximityLightPulseSettings3"};
				SetVectorParameterValue(
					ParameterNames[ComponentIndex],
					FLinearColor(GetProjectedRadius() * GetPulseTime(), 1.0f - GetPulseFadeTime(), 0.0f, 0.0f));
			}
			{
				static const FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightCenterColor0", "ProximityLightCenterColor1", "ProximityLightCenterColor2", "ProximityLightCenterColor3"};
				SetVectorParameterValue(ParameterNames[ComponentIndex], GetCenterColor());
			}
			{
				static const FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightMiddleColor0", "ProximityLightMiddleColor1", "ProximityLightMiddleColor2", "ProximityLightMiddleColor3"};
				SetVectorParameterValue(ParameterNames[ComponentIndex], GetMiddleColor());
			}
			{
				static const FName ParameterNames[GT_MAX_PROXIMITY_LIGHTS] = {
					"ProximityLightOuterColor0", "ProximityLightOuterColor1", "ProximityLightOuterColor2", "ProximityLightOuterColor3"};
				SetVectorParameterValue(ParameterNames[ComponentIndex], GetOuterColor());
			}
		}
	}
}

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