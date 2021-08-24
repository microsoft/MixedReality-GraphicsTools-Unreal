// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTMeshOutlineComponent.h"

#include "GraphicsTools.h"

#include "Materials/MaterialInstanceDynamic.h"

UGTMeshOutlineComponent::UGTMeshOutlineComponent()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> OutlineMaterialFinder(TEXT("/GraphicsTools/Materials/M_GTDefaultOutline"));
	check(OutlineMaterialFinder.Object);
	OutlineMaterial = OutlineMaterialFinder.Object;

	// Disable collision on outline meshes.
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UGTMeshOutlineComponent::SetOutlineColor(FColor Color)
{
	if (OutlineColor != Color)
	{
		OutlineColor = Color;

		UpdateMaterial();
	}
}

void UGTMeshOutlineComponent::SetOutlineThickness(float Thickness)
{
	if (OutlineThickness != Thickness)
	{
		OutlineThickness = Thickness;

		UpdateMaterial();
	}
}

void UGTMeshOutlineComponent::SetComputeSmoothNormals(bool Compute)
{
	if (bComputeSmoothNormals != Compute)
	{
		bComputeSmoothNormals = Compute;

		UE_LOG(
			GraphicsTools, Warning,
			TEXT("It is recomended that the outline static mesh is re-created after altering the smooth normal's property."));
	}
}

void UGTMeshOutlineComponent::OnRegister()
{
	Super::OnRegister();

#if WITH_EDITORONLY_DATA

	if (GetStaticMesh() == nullptr)
	{
		// If a static mesh isn't specified, try grabbing one from the parent.
		if (UStaticMeshComponent* Parent = Cast<UStaticMeshComponent>(GetAttachParent()))
		{
			SetStaticMesh(Parent->GetStaticMesh());
			SetMaterial(0, OutlineMaterial);
			UpdateMaterial();
			SetRelativeScale3D(FVector::OneVector);
		}
	}

	if (GetMaterial(0) == nullptr)
	{
		// Use the default outline material if one isn't specified.
		SetMaterial(0, OutlineMaterial);
		UpdateMaterial();
	}
#endif // WITH_EDITOR
}

#if WITH_EDITOR

void UGTMeshOutlineComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTMeshOutlineComponent, OutlineColor))
	{
		UpdateMaterial();
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTMeshOutlineComponent, OutlineThickness))
	{
		UpdateMaterial();
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTMeshOutlineComponent, bComputeSmoothNormals))
	{
		UE_LOG(
			GraphicsTools, Warning,
			TEXT("It is recomended that the outline static mesh is re-created after altering the smooth normal's property."));
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

	FName OutlineInstanceMaterialName = NAME_None;

#if WITH_EDITOR
	if (Cast<UMaterialInstanceDynamic>(Material) == nullptr)
	{
		OutlineInstanceMaterialName = *Material->GetName().Append("Instance");
	}
#endif

	UMaterialInstanceDynamic* MaterialInstance = CreateDynamicMaterialInstance(0, Material, OutlineInstanceMaterialName);

	static const FName OutlineColorName = "BaseColor";
	MaterialInstance->SetVectorParameterValue(OutlineColorName, OutlineColor);

	static const FName OutlineThicknessName = "OutlineThickness";
	MaterialInstance->SetScalarParameterValue(OutlineThicknessName, OutlineThickness);
}
