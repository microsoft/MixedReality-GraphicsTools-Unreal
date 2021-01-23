// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTClippingPrimitiveComponent.h"

#include "UObject/ConstructorHelpers.h"

namespace ClippingPrimitives
{
	void UpdateParameterCollection(UGTClippingPrimitiveComponent* Primitive)
	{
		if (!Primitive->IsValid())
		{
			return;
		}

		FTransform Tranform = Primitive->GetComponentTransform();
		Tranform.SetScale3D(Tranform.GetScale3D() * 2); // Double the scale to ensure sizing is consistent with other Unreal primitives.
		FMatrix InverseMatrixTranspose = Tranform.ToInverseMatrixWithScale().GetTransposed();

		for (int32 Index = 0; Index < 4; ++Index)
		{
			static FName ParameterNames[4] = {
				"ClippingPrimitiveTransformColumn0", "ClippingPrimitiveTransformColumn1", "ClippingPrimitiveTransformColumn2",
				"ClippingPrimitiveTransformColumn3"};
			Primitive->SetVectorParameterValue(ParameterNames[Index], InverseMatrixTranspose.GetColumn(Index));
		}
	}
} // namespace ClippingPrimitives

UGTClippingPrimitiveComponent::UGTClippingPrimitiveComponent()
{
	bWantsOnUpdateTransform = true;

#if WITH_EDITORONLY_DATA
	EditorTexture = nullptr;
#endif // WITH_EDITORONLY_DATA
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