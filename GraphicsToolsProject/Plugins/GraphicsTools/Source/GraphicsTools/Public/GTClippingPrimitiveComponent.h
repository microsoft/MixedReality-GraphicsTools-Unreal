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
 * materials for per pixel clipping.
 */
UCLASS(Abstract, ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTClippingPrimitiveComponent : public UGTSceneComponent
{
	GENERATED_BODY()

public:
	UGTClippingPrimitiveComponent();

	/** Gets if pixels will be clipped on the inside or outside of the primitive shape. */
	UFUNCTION(BlueprintPure, Category = "Clipping Primitive")
	EGTClippingSide GetClippingSide() const { return Clippingside; }

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
	// UActorComponent interface

	/** Adds the ClippingPrimitive to the material parameter collection. */
	virtual void OnRegister() override;

	/** Removes the ClippingPrimitive from the material parameter collection. */
	virtual void OnUnregister() override;

	/** Adds or removes the ClippingPrimitive from the material parameter collection. */
	virtual void OnVisibilityChanged() override;

	//
	// USceneComponent interface

	/** Notifies systems of the ClippingPrimitive's new transformation. */
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport = ETeleportType::None) override;

#if WITH_EDITOR
	/** Notifies systems of the ClippingPrimitive's new transformation when edited in the details panel. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

private:
	/** Specifies if the primitive discards pixels on the inside or outside of the primitive shape. */
	UPROPERTY(EditAnywhere, Category = "Clipping Primitive", BlueprintGetter = "GetClippingside", BlueprintSetter = "SetClippingside")
	EGTClippingSide Clippingside = EGTClippingSide::Inside;

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
