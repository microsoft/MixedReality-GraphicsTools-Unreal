// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GTSceneComponent.h"

#include "GTClippingPrimitiveComponent.generated.h"

class UMaterialParameterCollection;

UENUM(BlueprintType)
enum class EGTClippingSide : uint8
{
	/** Pixels on the interior of the clipping primitive are discarded. */
	Inside,
	/** Pixels on the exterior of the clipping primitive are discarded. */
	Outside
};

/**
 * Abstract base class for all ClippingPrimitive actor components within Graphics Tools. Represents an analytic shape used to pass state to
 * materials for per pixel clipping. A sphere's transformation is described by 4x4 matrix that represents the sphere's location, rotation,
 * and scale. A sphere can be non-uniformally scaled along the x, y, or z axis to form an ellipsoid. The sphere's radii along the x, y, or
 * z axis is determined by the magnitude of the scale along each axis. A scale of one represents a unit sphere.
 */
UCLASS(Abstract, ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTClippingPrimitiveComponent : public UGTSceneComponent
{
	GENERATED_BODY()

public:
	UGTClippingPrimitiveComponent();

	/** Gets if pixels will be clipped on the inside or outside of the primitive shape. */
	UFUNCTION(BlueprintPure, Category = "Clipping Primitive")
	EGTClippingSide GetClippingSide() const { return ClippingSide; }

	/** Sets if pixels will be clipped on the inside or outside of the primitive shape. */
	UFUNCTION(BlueprintSetter, Category = "Clipping Primitive")
	void SetClippingSide(EGTClippingSide Side);

	/** Gets the material parameter name used for general settings. */
	UFUNCTION(BlueprintPure, Category = "Clipping Primitive")
	const FName& GetSettingsParameterName() const { return SettingsParameterName; }

	/** Sets the material parameter name used for general settings. */
	UFUNCTION(BlueprintSetter, Category = "Clipping Primitive")
	void SetSettingsParameterName(const FName& Name);

	/** Gets the material parameter name array used to represent each column of the primitive's transformation matrix. */
	UFUNCTION(BlueprintPure, Category = "Clipping Primitive")
	const TArray<FName>& GetTransformColumnParameterNames() const { return TransformColumnParameterNames; }

	/** Sets the material parameter name array used to represent each column of the primitive's transformation matrix. */
	UFUNCTION(BlueprintSetter, Category = "Clipping Primitive")
	void SetTransformColumnParameterNames(const TArray<FName>& Names);

protected:
	//
	// UObject interface

#if WITH_EDITOR
	/** Disables the material parameter name properties when a ParameterCollectionOverride isn't present. */
	virtual bool CanEditChange(const FProperty* Property) const override;

	//
	// USceneComponent interface

	/** Ensures projected and attenuation radii remain less than or greater than each other. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	//
	// UGTSceneComponent interface

	/** Updates the current parameter collection based on the current primitive. */
	virtual void UpdateParameterCollection(bool IsDisabled = false) override;

	/** Sends the primitive's transform into the current parameter collection. */
	virtual void UpdateParameterCollectionTransform();

	/** The number of column's this primitives transform expects. */
	virtual int32 GetTransformColumnCount() const { return 4; }

private:
	/** Specifies if the primitive discards pixels on the inside or outside of the primitive shape. */
	UPROPERTY(EditAnywhere, Category = "Clipping Primitive", BlueprintGetter = "GetClippingside", BlueprintSetter = "SetClippingside")
	EGTClippingSide ClippingSide = EGTClippingSide::Inside;

	/** Material parameter name used for general primitive settings to pass to a material. */
	UPROPERTY(
		EditAnywhere, Category = "Clipping Primitive", BlueprintGetter = "GetSettingsParameterName",
		BlueprintSetter = "SetSettingsParameterName", AdvancedDisplay)
	FName SettingsParameterName;

	/** Parameter name array used to represent each column of the primitive's transformation matrix to pass to a material. */
	UPROPERTY(
		EditAnywhere, Category = "Clipping Primitive", BlueprintGetter = "GetTransformColumnParameterNames",
		BlueprintSetter = "SetTransformColumnParameterNames", AdvancedDisplay)
	TArray<FName> TransformColumnParameterNames;
};
