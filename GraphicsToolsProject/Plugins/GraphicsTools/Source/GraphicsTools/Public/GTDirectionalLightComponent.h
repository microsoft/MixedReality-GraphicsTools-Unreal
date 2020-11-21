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
UCLASS(ClassGroup = (GraphicsTools))
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

protected:
	//
	// UActorComponent interface

	/** Adds the DirectionalLight to the global light list. */
	virtual void OnRegister() override;

	/** Removes the DirectionalLight to the global light list. */
	virtual void OnUnregister() override;

	/** Adds or removes the DirectionalLight to the global light list based on visibility. */
	virtual void OnVisibilityChanged() override;

	//
	// USceneComponent interface

	/** Notifies systems of the DirectionalLight's new direction. */
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport = ETeleportType::None) override;

#if WITH_EDITOR
	/** Updates the material parameter collection. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

#if WITH_EDITORONLY_DATA
public:
	/** Returns ArrowComponent subobject **/
	UArrowComponent* GetArrowComponent() const { return ArrowComponent; }

private:
	// Reference to editor visualization arrow
	UPROPERTY()
	UArrowComponent* ArrowComponent = nullptr;
#endif // WITH_EDITORONLY_DATA

private:
	/** Total energy that the DirectionalLight emits. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetLightIntensity", BlueprintSetter = "SetLightIntensity", Category = "Light",
		meta = (DisplayName = "Intensity", UIMin = "0.0", UIMax = "20.0"))
	float LightIntensity = 10;

	/** The color of the DirectionalLight. */
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetLightColor", BlueprintSetter = "SetLightColor", Category = "Light")
	FColor LightColor = FColor(255, 255, 255, 255);
};
