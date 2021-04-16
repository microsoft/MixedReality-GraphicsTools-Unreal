// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingPrimitiveComponent.h"

#include "GraphicsTools.h"

#include "UObject/ConstructorHelpers.h"

UGTClippingPrimitiveComponent::UGTClippingPrimitiveComponent()
{
#if WITH_EDITORONLY_DATA
	EditorTexture = nullptr;
#endif // WITH_EDITORONLY_DATA

	{
		static const FName ParameterName("ClippingPrimitiveSettings");
		SettingsParameterName = ParameterName;
	}
	{
		static const FName ParameterNames[4] = {
			"ClippingPrimitiveTransformColumn0", "ClippingPrimitiveTransformColumn1", "ClippingPrimitiveTransformColumn2",
			"ClippingPrimitiveTransformColumn3"};
		TransformColumnParameterNames.Append(ParameterNames, UE_ARRAY_COUNT(ParameterNames));
	}
}

void UGTClippingPrimitiveComponent::SetClippingSide(EGTClippingSide Side)
{
	if (ClippingSide != Side)
	{
		ClippingSide = Side;
		UpdateParameterCollection();
	}
}

void UGTClippingPrimitiveComponent::SetSettingsParameterName(const FName& Name)
{
	if (SettingsParameterName != Name)
	{
		SettingsParameterName = Name;
		UpdateParameterCollection();
	}
}

void UGTClippingPrimitiveComponent::SetTransformColumnParameterNames(const TArray<FName>& Names)
{
	if (Names.Num() >= GetTransformColumnCount())
	{
		TransformColumnParameterNames = Names;
		UpdateParameterCollection();
	}
	else
	{
		UE_LOG(
			GraphicsTools, Warning,
			TEXT("Unable to SetTransformColumnParameterNames because the input does not contain at least %i column names."),
			GetTransformColumnCount());
	}
}

#if WITH_EDITOR
bool UGTClippingPrimitiveComponent::CanEditChange(const FProperty* Property) const
{
	bool IsEditable = Super::CanEditChange(Property);

	if (IsEditable && Property != nullptr)
	{
		if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UGTClippingPrimitiveComponent, SettingsParameterName) ||
			Property->GetFName() == GET_MEMBER_NAME_CHECKED(UGTClippingPrimitiveComponent, TransformColumnParameterNames))
		{
			IsEditable = HasParameterCollectionOverride();
		}
	}

	return IsEditable;
}

void UGTClippingPrimitiveComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTClippingPrimitiveComponent, TransformColumnParameterNames))
	{
		// Ensure we always have GetTransformColumnCount names.
		while (TransformColumnParameterNames.Num() < GetTransformColumnCount())
		{
			TransformColumnParameterNames.Add(FName());
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

void UGTClippingPrimitiveComponent::UpdateParameterCollection(bool IsDisabled)
{
	if (IsValid())
	{
		const TArray<UGTSceneComponent*>& Components = GetWorldComponents();
		const int32 ComponentIndex = Components.Find(this);

		// Only the first clipping primitive of this type will be considered, or a disabled clipping primitive of this type in the case of
		// removing the last clipping primitive of this type, or any components with an MPC override.
		if (ComponentIndex == 0 || IsDisabled || HasParameterCollectionOverride())
		{
			const float Side = GetClippingSide() == EGTClippingSide::Inside ? 1 : -1;
			SetVectorParameterValue(GetSettingsParameterName(), FLinearColor(!IsDisabled, Side, 0));

			UpdateParameterCollectionTransform();
		}
	}
}

void UGTClippingPrimitiveComponent::UpdateParameterCollectionTransform()
{
	FTransform Tranform = GetComponentTransform();
	Tranform.SetScale3D(Tranform.GetScale3D() * 2); // Double the scale to ensure sizing is consistent with other Unreal primitives.
	FMatrix InverseMatrixTranspose = Tranform.ToInverseMatrixWithScale().GetTransposed();
	const TArray<FName>& ParameterNames = GetTransformColumnParameterNames();

	for (int32 ColumnIndex = 0; ColumnIndex < GetTransformColumnCount(); ++ColumnIndex)
	{
		SetVectorParameterValue(ParameterNames[ColumnIndex], InverseMatrixTranspose.GetColumn(ColumnIndex));
	}
}
