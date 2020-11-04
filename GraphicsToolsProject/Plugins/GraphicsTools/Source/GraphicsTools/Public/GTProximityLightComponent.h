// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTLightComponent.h"

#include "GTProximityLightComponent.generated.h"

class UMaterialParameterCollection;

/**
 * TODO
 */
UCLASS(ClassGroup = (GraphicsTools))
class GRAPHICSTOOLS_API UGTProximityLightComponent : public UGTLightComponent
{
	GENERATED_BODY()

public:
	UGTProximityLightComponent();

	/** Accessor to the light's near radius. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	float GetNearRaidus() const { return NearRadius; }

	/** Sets the light's near radius. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetNearRaidus(float Radius);

	/** Accessor to the light's far radius. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	float GetFarRaidus() const { return FarRadius; }

	/** Sets the light's far radius. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetFarRaidus(float Radius);

	/** Accessor to the light's near distance. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	float GetNearDistance() const { return NearDistance; }

	/** Sets the light's near distance. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetNearDistance(float Distance);

	/** Accessor to the light's near distance. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	float GetMinNearSizePercentage() const { return MinNearSizePercentage; }

	/** Sets the light's near distance. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetMinNearSizePercentage(float Percentage);

	/** Accessor to the light's center color. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	FColor GetCenterColor() const { return CenterColor; }

	/** Sets the light's center color. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetCenterColor(FColor Color) { CenterColor = Color; }

	/** Accessor to the light's middle color. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	FColor GetMiddleColor() const { return MiddleColor; }

	/** Sets the light's middle color. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetMiddleColor(FColor Color) { MiddleColor = Color; }

	/** Accessor to the light's outer color. */
	UFUNCTION(BlueprintGetter, Category = "Light")
	FColor GetOuterColor() const { return OuterColor; }

	/** Sets the light's outer color. */
	UFUNCTION(BlueprintSetter, Category = "Light")
	void SetOuterColor(FColor Color) { OuterColor = Color; }

protected:
	//
	// USceneComponent interface

	/** Notifies systems of the ProximityLight's new location. */
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport = ETeleportType::None) override;

#if WITH_EDITOR
	/** Ensures near and far radii remain less than or greater than each other. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	/** Specifies the radius of the ProximityLight effect when near to a surface. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetNearRaidus", BlueprintSetter = "SetNearRaidus", Category = "Light",
		meta = (UIMin = "1.0", UIMax = "500.0", SliderExponent = "5.0"))
	float NearRadius = 5;

	/** Specifies the radius of the ProximityLight effect when far from a surface. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetFarRaidus", BlueprintSetter = "SetFarRaidus", Category = "Light",
		meta = (UIMin = "1.0", UIMax = "500.0", SliderExponent = "5.0"))
	float FarRadius = 20;

	/** Specifies the distance a ProximityLight must be from a surface (less than or equal to) to be considered near. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetNearDistance", BlueprintSetter = "SetNearDistance", Category = "Light",
		meta = (UIMin = "1.0", UIMax = "500.0", SliderExponent = "5.0"))
	float NearDistance = 2;

	/** When a ProximityLight is near, the smallest size percentage from the near size it can shrink to. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetMinNearSizePercentage", BlueprintSetter = "SetMinNearSizePercentage", Category = "Light",
		meta = (UIMin = "0.0", UIMax = "1.0"))
	float MinNearSizePercentage = 0.35f;

	/** The color of the ProximityLight gradient at the center (RGB) and (A) is gradient extent. */
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetCenterColor", BlueprintSetter = "SetCenterColor", Category = "Light")
	FColor CenterColor = FColor(54, 142, 250, 0);

	/** The color of the ProximityLight gradient in between the center and outer color (RGB) and (A) is gradient extent. */
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetMiddleColor", BlueprintSetter = "SetMiddleColor", Category = "Light")
	FColor MiddleColor = FColor(47, 132, 255, 51);

	/** The color of the ProximityLight gradient at the outer edge (RGB) and (A) is gradient extent. */
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetOuterColor", BlueprintSetter = "SetOuterColor", Category = "Light")
	FColor OuterColor = FColor(246, 93, 255, 255);

	/** The MaterialParameterCollection this ProximityLight will write to. */
	UPROPERTY(Transient)
	UMaterialParameterCollection* ParameterCollection = nullptr;
};
