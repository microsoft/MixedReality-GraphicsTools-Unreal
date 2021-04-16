// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTDirectionalLightComponent.h"

#include "GTWorldSubsystem.h"

#include "Components/ArrowComponent.h"

UGTDirectionalLightComponent::UGTDirectionalLightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	if (!IsRunningCommandlet())
	{
		static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(TEXT("/Engine/EditorResources/LightIcons/S_LightDirectional"));
		check(Texture.Object);
		EditorTexture = Texture.Object;
	}

	// Structure to hold one-time initialization names.
	struct FConstructorStatics
	{
		FName ID_Lighting;
		FText NAME_Lighting;
		FConstructorStatics() : ID_Lighting(TEXT("Lighting")), NAME_Lighting(NSLOCTEXT("SpriteCategory", "Lighting", "Lighting")) {}
	};
	static FConstructorStatics ConstructorStatics;

	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent0"));

	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(255, 255, 255);
		ArrowComponent->bTreatAsASprite = true;
		ArrowComponent->SpriteInfo.Category = ConstructorStatics.ID_Lighting;
		ArrowComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Lighting;
		ArrowComponent->SetupAttachment(this);
		ArrowComponent->bLightAttachment = true;
		ArrowComponent->bIsScreenSizeScaled = true;
	}
#endif // WITH_EDITORONLY_DATA

	{
		static const FName ParameterName("DirectionalLightDirectionEnabled");
		DirectionEnabledParameterName = ParameterName;
	}
	{
		static const FName ParameterName("DirectionalLightColorIntensity");
		ColorIntensityParameterName = ParameterName;
	}
}

void UGTDirectionalLightComponent::SetLightIntensity(float Intensity)
{
	if (LightIntensity != Intensity)
	{
		LightIntensity = Intensity;

		UpdateParameterCollection();
	}
}

void UGTDirectionalLightComponent::SetLightColor(FColor Color)
{
	if (LightColor != Color)
	{
		LightColor = Color;

		UpdateParameterCollection();
	}
}

void UGTDirectionalLightComponent::SetDirectionEnabledParameterName(const FName& Name)
{
	if (DirectionEnabledParameterName != Name)
	{
		DirectionEnabledParameterName = Name;
		UpdateParameterCollection();
	}
}

void UGTDirectionalLightComponent::SetColorIntensityParameterName(const FName& Name)
{
	if (ColorIntensityParameterName != Name)
	{
		ColorIntensityParameterName = Name;
		UpdateParameterCollection();
	}
}

#if WITH_EDITOR
bool UGTDirectionalLightComponent::CanEditChange(const FProperty* Property) const
{
	bool IsEditable = Super::CanEditChange(Property);

	if (IsEditable && Property != nullptr)
	{
		if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(UGTDirectionalLightComponent, DirectionEnabledParameterName) ||
			Property->GetFName() == GET_MEMBER_NAME_CHECKED(UGTDirectionalLightComponent, ColorIntensityParameterName))
		{
			IsEditable = HasParameterCollectionOverride();
		}
	}

	return IsEditable;
}
#endif // WITH_EDITOR

void UGTDirectionalLightComponent::OnRegister()
{
	Super::OnRegister();

#if WITH_EDITORONLY_DATA
	if (ArrowComponent != nullptr)
	{
		ArrowComponent->ArrowColor = GetLightColor();
	}
#endif // WITH_EDITORONLY_DATA
}

#if WITH_EDITOR
void UGTDirectionalLightComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (ArrowComponent != nullptr)
	{
		ArrowComponent->ArrowColor = GetLightColor();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

TArray<UGTSceneComponent*>& UGTDirectionalLightComponent::GetWorldComponents()
{
	return GetWorld()->GetSubsystem<UGTWorldSubsystem>()->DirectionalLights;
}

void UGTDirectionalLightComponent::UpdateParameterCollection(bool IsDisabled)
{
	if (IsValid())
	{
		const TArray<UGTSceneComponent*>& Components = GetWorldComponents();
		const int32 ComponentIndex = Components.Find(this);

		// Only the first directional light will be considered, or a disabled light in the case of removing the last light, or any
		// components with an MPC override.
		if (ComponentIndex == 0 || IsDisabled || HasParameterCollectionOverride())
		{
			{
				FLinearColor DirectionEnabled(-GetForwardVector());
				DirectionEnabled.A = !IsDisabled;
				SetVectorParameterValue(GetDirectionEnabledParameterName(), DirectionEnabled);
			}
			{
				FLinearColor ColorIntensity(GetLightColor());
				ColorIntensity.A = GetLightIntensity();
				SetVectorParameterValue(GetColorIntensityParameterName(), ColorIntensity);
			}
		}
	}
}
