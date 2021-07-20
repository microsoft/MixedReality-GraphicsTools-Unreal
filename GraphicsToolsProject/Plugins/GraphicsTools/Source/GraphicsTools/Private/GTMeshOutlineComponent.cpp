// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTMeshOutlineComponent.h"

void UGTMeshOutlineComponent::SetOutlineThickness(float Thickness)
{
	if (OutlineThickness != Thickness)
	{
		UpdateMaterial();
	}
}

void UGTMeshOutlineComponent::SetOutlineColor(FColor Color)
{
	if (OutlineColor != Color)
	{
		UpdateMaterial();
	}
}

void UGTMeshOutlineComponent::SetComputeSmoothNormals(bool Compute)
{
	if (bComputeSmoothNormals != Compute)
	{
		bComputeSmoothNormals = Compute;

		// TODO, rebuild the outline mesh.
	}
}

#if WITH_EDITOR

void UGTMeshOutlineComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTMeshOutlineComponent, OutlineThickness))
	{
		UpdateMaterial();
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTMeshOutlineComponent, OutlineColor))
	{
		UpdateMaterial();
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTMeshOutlineComponent, bComputeSmoothNormals))
	{
		// TODO, rebuild the outline mesh.
	}
}
#endif // WITH_EDITOR

void UGTMeshOutlineComponent::UpdateMaterial()
{
	UMaterialInterface* Material = GetMaterial(0);

	if (Material == nullptr)
	{
		return;
	}

	UMaterialInstanceDynamic* MaterialInstance = CreateDynamicMaterialInstance(0, Material);

	static const FName OutlineThicknessName = "OutlineThickness";
	MaterialInstance->SetScalarParameterValue(OutlineThicknessName, OutlineThickness);

	static const FName OutlineColorName = "OutlineColor";
	MaterialInstance->SetVectorParameterValue(OutlineColorName, OutlineColor);
}
