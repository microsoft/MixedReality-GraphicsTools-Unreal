// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

#include "GTMeshOutlineComponent.generated.h"

/**
 * TODO
 */
UCLASS(ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTMeshOutlineComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	//
	// UObject interface

	virtual void PostLoad() override;

	/** TODO */
	UFUNCTION(BlueprintGetter, Category = "Mesh Outline")
	UStaticMesh* GetStaticMesh() const { return StaticMesh; }

	/** TODO */
	UFUNCTION(BlueprintSetter, Category = "Mesh Outline")
	void SetStaticMesh(UStaticMesh* Mesh);

protected:
#if WITH_EDITOR
	/** TODO */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

private:
	/** TODO */
	void UpdateMesh();

	/** TODO*/
	UPROPERTY(EditAnywhere, BlueprintGetter = "GetStaticMesh", BlueprintSetter = "SetStaticMesh", Category = "Mesh Outline")
	UStaticMesh* StaticMesh = nullptr;
};
