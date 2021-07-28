// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Components/StaticMeshComponent.h"

#include "GTMeshOutlineComponent.generated.h"

/**
   Component which can be used to render an outline around a static mesh. Using this component introduces an additional render pass of
   the object being outlined, but is designed to run performantly on mobile mixed reality devices and does not utilize any post processes.
   Because this effect happens during the default render pass there are a few limitations of this effect which include:
	   - The outline mesh must be watertight hull (and not double sided) else you may see split edges, holes, or other artifacts.
	   - Mesh concavities can intersect each other when the outline is thick. This could be solved if the sort order of opaque objects could
		 be controlled so that the outline renders first and depth write disabled.
	   - Outlines will not render though occluding objects.
	   - Translucent objects may show the outline mesh in areas you would expect to be occluded.
   Note, this component assumes a material is being used which has a `BaseColor` and `OutlineThickness` parameter to function correctly. The
   `OutlineThickness` should scale the vertex position offset along the vertex normal.
 */
UCLASS(ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent), HideCategories = (Physics, Collision))
class GRAPHICSTOOLS_API UGTMeshOutlineComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UGTMeshOutlineComponent();

	/** Accessor to the outline color. */
	UFUNCTION(BlueprintGetter, Category = "Mesh Outline")
	FColor GetOutlineColor() const { return OutlineColor; }

	/** Sets the outline color and updates the material instance. */
	UFUNCTION(BlueprintSetter, Category = "Mesh Outline")
	void SetOutlineColor(FColor Color);

	/** Accessor to the outline thickness (in Unreal units). */
	UFUNCTION(BlueprintGetter, Category = "Mesh Outline")
	float GetOutlineThickness() const { return OutlineThickness; }

	/** Sets the outline thickness (in Unreal units) and updates the material instance. */
	UFUNCTION(BlueprintSetter, Category = "Mesh Outline")
	void SetOutlineThickness(float Thickness);

	/** Sets if the mesh outline generation algorithm should smooth normals. */
	UFUNCTION(BlueprintGetter, Category = "Mesh Outline")
	bool GetComputeSmoothNormals() const { return bComputeSmoothNormals; }

	/** Accessor for if the mesh outline generation algorithm should smooth normals. */
	UFUNCTION(BlueprintSetter, Category = "Mesh Outline")
	void SetComputeSmoothNormals(bool Compute);

	//
	// UObject interface

	/** Sets up the components default state when registered. */
	virtual void OnRegister() override;

protected:
#if WITH_EDITOR
	/** Responds to details panel updates. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

private:
	/** Updates the `BaseColor` and `OutlineThickness` parameter on a material instance. */
	void UpdateMaterial();

	/** The color of the mesh outline. Passes this value into the `BaseColor` parameter of the material instance. */
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetOutlineColor", BlueprintSetter = "SetOutlineColor", Category = "Mesh Outline")
	FColor OutlineColor = FColor(255, 0, 0, 255);

	/** The thickness (in Unreal units) of the mesh outline. Passes this value into the `OutlineThickness` parameter of the material
	 * instance. It is up to the material to choose what to do with this value, but normally it is used to determine the distance to offset
	 * the vertex position along the vertex normal. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetOutlineThickness", BlueprintSetter = "SetOutlineThickness", Category = "Mesh Outline",
		Category = "Mesh Outline", meta = (UIMin = "0.0", UIMax = "10.0"))
	float OutlineThickness = 0.5f;

	/** This setting is optional for some meshes. Outline extrusion occurs by moving a vertex along a vertex normal, on some meshes
	 * extruding along the default normals will cause discontinuities in the outline. To fix these discontinuities, you can check this box
	 * to generate a smooth normal set during outline mesh generation. */
	UPROPERTY(
		EditAnywhere, BlueprintGetter = "GetComputeSmoothNormals", BlueprintSetter = "SetComputeSmoothNormals", Category = "Mesh Outline")
	bool bComputeSmoothNormals = true;

	/** The default outline material. */
	UPROPERTY(Transient)
	UMaterialInterface* OutlineMaterial = nullptr;
};
