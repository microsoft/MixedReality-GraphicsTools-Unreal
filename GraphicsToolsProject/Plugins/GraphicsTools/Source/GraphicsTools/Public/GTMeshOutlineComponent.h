// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"

#include "GTMeshOutlineComponent.generated.h"

/**
 * TODO
 */
UCLASS(ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTMeshOutlineComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	/** TODO */
	UFUNCTION(BlueprintGetter, Category = "Mesh Outline")
	float GetOutlineThickness() const { return OutlineThickness; }

	/** TODO */
	UFUNCTION(BlueprintSetter, Category = "Mesh Outline")
	void SetOutlineThickness(float Thickness);

	/** TODO */
	UFUNCTION(BlueprintGetter, Category = "Mesh Outline")
	FColor GetOutlineColor() const { return OutlineColor; }

	/** TODO */
	UFUNCTION(BlueprintSetter, Category = "Mesh Outline")
	void SetOutlineColor(FColor Color);

	/** TODO */
	UFUNCTION(BlueprintGetter, Category = "Mesh Outline")
	bool GetComputeSmoothNormals() const { return bComputeSmoothNormals; }

	/** TODO */
	UFUNCTION(BlueprintSetter, Category = "Mesh Outline")
	void SetComputeSmoothNormals(bool Compute);

protected:
#if WITH_EDITOR
	/** TODO */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

private:
	/** TODO */
	void UpdateMaterial();

	/** TODO*/
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetOutlineThickness", BlueprintSetter = "SetOutlineThickness", Category = "Mesh Outline",
		Category = "Mesh Outline", meta = (UIMin = "0.0", UIMax = "10.0"))
	float OutlineThickness = 0.5f;

	/** TODO*/
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetOutlineColor", BlueprintSetter = "SetOutlineColor", Category = "Mesh Outline")
	FColor OutlineColor = FColor(255, 0, 0, 255);

	/** TODO*/
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetComputeSmoothNormals", BlueprintSetter = "SetComputeSmoothNormals", Category = "Mesh Outline")
	bool bComputeSmoothNormals = true;
};
