// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTLightComponent.h"

#include "GTDirectionalLightComponent.generated.h"

class UArrowComponent;

/**
 * A light component that has parallel rays. Will provide a uniform lighting across any affected surface (eg. The Sun). Only 1
 * DirectionalLight will be considered in a scene at a time.
 */
UCLASS(ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTDirectionalLightComponent : public UGTLightComponent
{
	GENERATED_BODY()

public:
	UGTDirectionalLightComponent();

	/** Accessor to the light's intensity. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	float GetLightIntensity() const { return LightIntensity; }

	/** Sets the light's intensity. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetLightIntensity(float Intensity);

	/** Accessor to the light's color. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	FColor GetLightColor() const { return LightColor; }

	/** Sets the light's color. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetLightColor(FColor Color);

	/** Gets the material parameter name used for the direction and enabled state. */
	UFUNCTION(BlueprintPure, Category = "Light")
	const FName& GetDirectionEnabledParameterName() const { return DirectionEnabledParameterName; }

	/** Sets the material parameter name used for the direction and enabled state. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetDirectionEnabledParameterName(const FName& Name);

	/** Gets the material parameter name used for the color and intensity state. */
	UFUNCTION(BlueprintPure, Category = "Light")
	const FName& GetColorIntensityParameterName() const { return ColorIntensityParameterName; }

	/** Sets the material parameter name used for the color and intensity state. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetColorIntensityParameterName(const FName& Name);

protected:
	//
	// UObject interface

#if WITH_EDITOR
	/** Disables the material parameter name properties when a ParameterCollectionOverride isn't present. */
	virtual bool CanEditChange(const FProperty* Property) const override;
#endif // WITH_EDITOR

	//
	// UActorComponent interface

	/** Adds the DirectionalLight to the global light list. */
	virtual void OnRegister() override;

	//
	// USceneComponent interface

#if WITH_EDITOR
	/** Updates the material parameter collection. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	//
	// UGTSceneComponent interface

	/** Accessor to all UGTDirectionalLightComponent components within a world writing to the WorldParameterCollection. */
	virtual TArray<UGTSceneComponent*>& GetWorldComponents() override;

	/** Updates the current parameter collection based on the current UGTDirectionalLightComponent. */
	virtual void UpdateParameterCollection(bool IsDisabled = false) override;

private:
#if WITH_EDITORONLY_DATA
	// Reference to editor visualization arrow
	UPROPERTY()
	UArrowComponent* ArrowComponent = nullptr;
#endif // WITH_EDITORONLY_DATA

	/** Total energy that the DirectionalLight emits. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetLightIntensity", BlueprintSetter = "SetLightIntensity", Category = "Light",
		meta = (DisplayName = "Intensity", UIMin = "0.0", UIMax = "20.0"))
	float LightIntensity = 10;

	/** The color of the DirectionalLight. */
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetLightColor", BlueprintSetter = "SetLightColor", Category = "Light")
	FColor LightColor = FColor(255, 255, 255, 255);

	/** Material parameter name used for the lights direction and enabled state to pass to a material. */
	UPROPERTY(
		EditAnywhere, Category = "Light", BlueprintGetter = "GetDirectionEnabledParameterName",
		BlueprintSetter = "SetDirectionEnabledParameterName", AdvancedDisplay)
	FName DirectionEnabledParameterName;

	/** Material parameter name used for the lights color and intensity to pass to a material. */
	UPROPERTY(
		EditAnywhere, Category = "Light", BlueprintGetter = "GetColorIntensityParameterName",
		BlueprintSetter = "SetColorIntensityParameterName", AdvancedDisplay)
	FName ColorIntensityParameterName;
};
