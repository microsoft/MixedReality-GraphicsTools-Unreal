// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTLightComponent.h"

#include "GTProximityLightComponent.generated.h"

enum class EPulseState : uint8
{
	Idle,
	Animate,
	FadeIn,
	FadeOut
};
ENUM_CLASS_FLAGS(EPulseState);

/**
 * A proximity light is a Fluent Design System paradigm that acts as a surface projected gradient point light. For a material to be
 * influenced by a proximity light the ProximityLightContribution material function must be used within the material's shader graph.
 */
UCLASS(ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTProximityLightComponent : public UGTLightComponent
{
	GENERATED_BODY()

public:
	UGTProximityLightComponent();

	/** Accessor to the light's projected radius. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	float GetProjectedRadius() const { return ProjectedRadius; }

	/** Sets the light's projected radius. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetProjectedRadius(float Radius);

	/** Accessor to the light's attenuation radius. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	float GetAttenuationRadius() const { return AttenuationRadius; }

	/** Sets the light's attenuation radius. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetAttenuationRadius(float Radius);

	/** Accessor to the light's shrink distance. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	float GetShrinkDistance() const { return ShrinkDistance; }

	/** Sets the light's shrink distance. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetShrinkDistance(float Distance);

	/** Accessor to the light's shrink percentage. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	float GetShrinkPercentage() const { return ShrinkPercentage; }

	/** Sets the light's shrink percentage. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetShrinkPercentage(float Percentage);

	/** Accessor to the light's center color. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	FColor GetCenterColor() const { return CenterColor; }

	/** Sets the light's center color. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetCenterColor(FColor Color);

	/** Accessor to the light's middle color. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	FColor GetMiddleColor() const { return MiddleColor; }

	/** Sets the light's middle color. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetMiddleColor(FColor Color);

	/** Accessor to the light's outer color. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	FColor GetOuterColor() const { return OuterColor; }

	/** Sets the light's outer color. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetOuterColor(FColor Color);

	/** Gets the material parameter name array used to represent each location of a ProximityLight. */
	UFUNCTION(BlueprintPure, Category = "Light")
	const TArray<FName>& GetLocationParameterNames() const { return LocationParameterNames; }

	/** Sets the material parameter name array used to represent each location of a ProximityLight. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetLocationParameterNames(const TArray<FName>& Names);

	/** Gets the material parameter name array used to represent each setting of a ProximityLight. */
	UFUNCTION(BlueprintPure, Category = "Light")
	const TArray<FName>& GetSettingsParameterNames() const { return SettingsParameterNames; }

	/** Sets the material parameter name array used to represent each setting of a ProximityLight. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetSettingsParameterNames(const TArray<FName>& Names);

	/** Gets the material parameter name array used to represent each pulse setting of a ProximityLight. */
	UFUNCTION(BlueprintPure, Category = "Light")
	const TArray<FName>& GetPulseSettingsParameterNames() const { return PulseSettingsParameterNames; }

	/** Sets the material parameter name array used to represent each pulse setting of a ProximityLight. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetPulseSettingsParameterNames(const TArray<FName>& Names);

	/** Gets the material parameter name array used to represent each center color of a ProximityLight. */
	UFUNCTION(BlueprintPure, Category = "Light")
	const TArray<FName>& GetCenterColorParameterNames() const { return CenterColorParameterNames; }

	/** Sets the material parameter name array used to represent each center color of a ProximityLight. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetCenterColorParameterNames(const TArray<FName>& Names);

	/** Gets the material parameter name array used to represent each middle color of a ProximityLight. */
	UFUNCTION(BlueprintPure, Category = "Light")
	const TArray<FName>& GetMiddleColorParameterNames() const { return MiddleColorParameterNames; }

	/** Sets the material parameter name array used to represent each middle color of a ProximityLight. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetMiddleColorParameterNames(const TArray<FName>& Names);

	/** Gets the material parameter name array used to represent each outer color of a ProximityLight. */
	UFUNCTION(BlueprintPure, Category = "Light")
	const TArray<FName>& GetOuterColorParameterNames() const { return OuterColorParameterNames; }

	/** Sets the material parameter name array used to represent each outer color of a ProximityLight. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetOuterColorParameterNames(const TArray<FName>& Names);

	/** Initiates a pulse, if one is not already occurring, which simulates a user touching a surface. */
	UFUNCTION(BlueprintCallable, Category = "Light")
	void Pulse(float Duration = 0.2f, float FadeOffset = 0.1f, float FadeDuration = 0.1f);

	/** Calculates the normalized pulse animation time. */
	UFUNCTION(BlueprintCallable, Category = "Light")
	float GetPulseTime() const;

	/** Calculates the normalized pulse fade animation time. */
	UFUNCTION(BlueprintCallable, Category = "Light")
	float GetPulseFadeTime() const;

protected:
	//
	// UObject interface

#if WITH_EDITOR
	/** Disables the material parameter name properties when a ParameterCollectionOverride isn't present. */
	virtual bool CanEditChange(const FProperty* Property) const override;
#endif // WITH_EDITOR

	//
	// UActorComponent interface

	/** Conditional tick method which occurs when a light needs to animate. */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//
	// USceneComponent interface

#if WITH_EDITOR
	/** Ensures projected and attenuation radii remain less than or greater than each other. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	//
	// UGTSceneComponent interface

	/** Accessor to all UGTProximityLightComponent components within a world writing to the WorldParameterCollection. */
	virtual TArray<UGTSceneComponent*>& GetWorldComponents() override;

	/** Updates the current parameter collection based on the current UGTProximityLightComponent. */
	virtual void UpdateParameterCollection(bool IsDisabled = false) override;

private:
	EPulseState PulseTick(float DeltaTime);

	/** Specifies the radius of the ProximityLight effect when projected onto a surface. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetProjectedRadius", BlueprintSetter = "SetProjectedRadius", Category = "Light",
		meta = (UIMin = "1.0", UIMax = "500.0", SliderExponent = "5.0"))
	float ProjectedRadius = 5;

	/** Specifies the radius at which the ProximityLight effect is no longer visible. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetAttenuationRadius", BlueprintSetter = "SetAttenuationRadius", Category = "Light",
		meta = (UIMin = "1.0", UIMax = "500.0", SliderExponent = "5.0"))
	float AttenuationRadius = 20;

	/** Specifies the distance a ProximityLight must be from a surface (less than or equal to) to begin shrinking to the
	(ProjectedRadius * ShrinkPercentage) radius. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetShrinkDistance", BlueprintSetter = "SetShrinkDistance", Category = "Light",
		meta = (UIMin = "1.0", UIMax = "500.0", SliderExponent = "5.0"))
	float ShrinkDistance = 2;

	/** When a ProximityLight is closer to a surface than the ShrinkDistance, the smallest percentage of the ProjectedRadius it can
	 * shrink to. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetShrinkPercentage", BlueprintSetter = "SetShrinkPercentage", Category = "Light",
		meta = (UIMin = "0.0", UIMax = "1.0"))
	float ShrinkPercentage = 0.35f;

	/** The color of the ProximityLight gradient at the center (RGB) and (A) is gradient extent. */
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetCenterColor", BlueprintSetter = "SetCenterColor", Category = "Light")
	FColor CenterColor = FColor(75, 148, 251, 0);

	/** The color of the ProximityLight gradient in between the center and outer color (RGB) and (A) is gradient extent. */
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetMiddleColor", BlueprintSetter = "SetMiddleColor", Category = "Light")
	FColor MiddleColor = FColor(47, 132, 255, 51);

	/** The color of the ProximityLight gradient at the outer edge (RGB) and (A) is gradient extent. */
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetOuterColor", BlueprintSetter = "SetOuterColor", Category = "Light")
	FColor OuterColor = FColor(114, 55, 191, 255);

	/** Parameter name array used to represent each location of a ProximityLight to pass to a material. */
	UPROPERTY(
		EditAnywhere, Category = "Light", BlueprintGetter = "GetLocationParameterNames", BlueprintSetter = "SetLocationParameterNames",
		AdvancedDisplay)
	TArray<FName> LocationParameterNames;

	/** Parameter name array used to represent each setting of a ProximityLight to pass to a material. */
	UPROPERTY(
		EditAnywhere, Category = "Light", BlueprintGetter = "GetSettingsParameterNames", BlueprintSetter = "SetSettingsParameterNames",
		AdvancedDisplay)
	TArray<FName> SettingsParameterNames;

	/** Parameter name array used to represent each pulse setting of a ProximityLight to pass to a material. */
	UPROPERTY(
		EditAnywhere, Category = "Light", BlueprintGetter = "GetPulseSettingsParameterNames",
		BlueprintSetter = "SetPulseSettingsParameterNames", AdvancedDisplay)
	TArray<FName> PulseSettingsParameterNames;

	/** Parameter name array used to represent each center color of a ProximityLight to pass to a material. */
	UPROPERTY(
		EditAnywhere, Category = "Light", BlueprintGetter = "GetCenterColorParameterNames",
		BlueprintSetter = "SetCenterColorParameterNames", AdvancedDisplay)
	TArray<FName> CenterColorParameterNames;

	/** Parameter name array used to represent each middle color of a ProximityLight to pass to a material. */
	UPROPERTY(
		EditAnywhere, Category = "Light", BlueprintGetter = "GetMiddleColorParameterNames",
		BlueprintSetter = "SetMiddleColorParameterNames", AdvancedDisplay)
	TArray<FName> MiddleColorParameterNames;

	/** Parameter name array used to represent each outer color of a ProximityLight to pass to a material. */
	UPROPERTY(
		EditAnywhere, Category = "Light", BlueprintGetter = "GetOuterColorParameterNames", BlueprintSetter = "SetOuterColorParameterNames",
		AdvancedDisplay)
	TArray<FName> OuterColorParameterNames;

	EPulseState PulseState = EPulseState::Idle;
	float PulseTimer = 0;
	float PulseFadeTimer = 0;
	float PulseDuration = 0;
	float PulseFadeOffset = 0;
	float PulseFadeDuration = 0;
};
