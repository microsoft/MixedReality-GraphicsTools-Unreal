// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingPrimitiveComponent.h"

#include "GraphicsTools.h"

#include "UObject/ConstructorHelpers.h"

namespace ClippingPrimitives
{
	void UpdateParameterCollection(UGTClippingPrimitiveComponent* Primitive)
	{
		if (!Primitive->IsValid())
		{
			return;
		}

		{
			float Visible = Primitive->IsRegistered() && Primitive->IsVisible();
			float Side = Primitive->GetClippingSide() == EGTClippingSide::Inside ? 1 : -1;
			Primitive->SetVectorParameterValue(Primitive->GetSettingsParameterName(), FLinearColor(Visible, Side, 0));
		}
		{
			FTransform Tranform = Primitive->GetComponentTransform();
			Tranform.SetScale3D(Tranform.GetScale3D() * 2); // Double the scale to ensure sizing is consistent with other Unreal primitives.
			FMatrix InverseMatrixTranspose = Tranform.ToInverseMatrixWithScale().GetTransposed();
			const TArray<FName>& ParameterNames = Primitive->GetTransformColumnParameterNames();

			for (int32 Index = 0; Index < 4; ++Index)
			{
				Primitive->SetVectorParameterValue(ParameterNames[Index], InverseMatrixTranspose.GetColumn(Index));
			}
		}
	}
} // namespace ClippingPrimitives

UGTClippingPrimitiveComponent::UGTClippingPrimitiveComponent()
{
	bWantsOnUpdateTransform = true;

#if WITH_EDITORONLY_DATA
	EditorTexture = nullptr;
#endif // WITH_EDITORONLY_DATA

	{
		static FName ParameterName("ClippingPrimitiveSettings");
		SettingsParameterName = ParameterName;
	}
	{
		static FName ParameterNames[4] = {
			"ClippingPrimitiveTransformColumn0", "ClippingPrimitiveTransformColumn1", "ClippingPrimitiveTransformColumn2",
			"ClippingPrimitiveTransformColumn3"};

		TransformColumnParameterNames.Append(ParameterNames, UE_ARRAY_COUNT(ParameterNames));
	}
}

void UGTClippingPrimitiveComponent::SetClippingSide(EGTClippingSide Side)
{
	if (Clippingside != Side)
	{
		Clippingside = Side;
		ClippingPrimitives::UpdateParameterCollection(this);
	}
}

void UGTClippingPrimitiveComponent::SetSettingsParameterName(const FName& Name)
{
	if (SettingsParameterName != Name)
	{
		SettingsParameterName = Name;
		ClippingPrimitives::UpdateParameterCollection(this);
	}
}

void UGTClippingPrimitiveComponent::SetTransformColumnParameterNames(const TArray<FName>& Names)
{
	if (Names.Num() == 4)
	{
		TransformColumnParameterNames = Names;
		ClippingPrimitives::UpdateParameterCollection(this);
	}
	else
	{
		UE_LOG(
			GraphicsTools, Warning, TEXT("Unable to SetTransformColumnParameterNames because the input does not contain 4 column names."));
	}
}

void UGTClippingPrimitiveComponent::OnRegister()
{
	Super::OnRegister();

	if (IsVisible())
	{
		ClippingPrimitives::UpdateParameterCollection(this);
	}
}

void UGTClippingPrimitiveComponent::OnUnregister()
{
	Super::OnUnregister();

	ClippingPrimitives::UpdateParameterCollection(this);
}

void UGTClippingPrimitiveComponent::OnVisibilityChanged()
{
	Super::OnVisibilityChanged();

	ClippingPrimitives::UpdateParameterCollection(this);
}

void UGTClippingPrimitiveComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	ClippingPrimitives::UpdateParameterCollection(this);
}

#if WITH_EDITOR
void UGTClippingPrimitiveComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	ClippingPrimitives::UpdateParameterCollection(this);
}
#endif // WITH_EDITOR