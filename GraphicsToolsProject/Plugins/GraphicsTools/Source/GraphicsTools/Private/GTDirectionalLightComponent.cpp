// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTDirectionalLightComponent.h"

#include "GTWorldSubsystem.h"

#include "Components/ArrowComponent.h"

namespace DirectionalLights
{
	typedef TArray<UGTDirectionalLightComponent*> LightList;

	LightList& GetLightList(const UWorld* World) { return World->GetSubsystem<UGTWorldSubsystem>()->DirectionalLights; }

	void UpdateParameterCollection(UGTDirectionalLightComponent* Light, bool LightEnabled = true)
	{
		if (!Light->IsValid())
		{
			return;
		}

		const LightList& Lights = GetLightList(Light->GetWorld());
		const int32 LightIndex = Lights.Find(Light);

		// Only the first directional light will be considered (or a disabled light in the case of removing the last light).
		if (LightEnabled && (LightIndex != 0))
		{
			return;
		}

		{
			static FName DirectionEnabledParameterName("DirectionalLightDirectionEnabled");
			FLinearColor DirectionEnabled(-Light->GetForwardVector());
			DirectionEnabled.A = LightEnabled ? 1.0f : 0.0f;
			Light->SetVectorParameterValue(DirectionEnabledParameterName, DirectionEnabled);
		}
		{
			static FName ColorIntensityParameterName("DirectionalLightColorIntensity");
			FLinearColor ColorIntensity(Light->GetLightColor());
			ColorIntensity.A = Light->GetLightIntensity();
			Light->SetVectorParameterValue(ColorIntensityParameterName, ColorIntensity);
		}
	}

	void AddLight(UGTDirectionalLightComponent* Light)
	{
		if (!Light->IsValid())
		{
			return;
		}

		LightList& Lights = GetLightList(Light->GetWorld());

		if (Lights.Find(Light) == INDEX_NONE)
		{
			if (Lights.Add(Light) == 0)
			{
				UpdateParameterCollection(Light);
			}
		}
	}

	void RemoveLight(UGTDirectionalLightComponent* Light)
	{
		if (!Light->IsValid())
		{
			return;
		}

		LightList& Lights = GetLightList(Light->GetWorld());
		const int32 Index = Lights.Find(Light);

		if (Index != INDEX_NONE)
		{
			Lights.RemoveAt(Index);

			if (Index == 0)
			{
				bool bLightEnabled = Lights.Num() != 0;
				UpdateParameterCollection(bLightEnabled ? Lights[0] : Light, bLightEnabled);
			}
		}
	}
} // namespace DirectionalLights

UGTDirectionalLightComponent::UGTDirectionalLightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsOnUpdateTransform = true;

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
}

void UGTDirectionalLightComponent::SetLightIntensity(float Intensity)
{
	if (LightIntensity != Intensity)
	{
		LightIntensity = Intensity;

		DirectionalLights::UpdateParameterCollection(this);
	}
}

void UGTDirectionalLightComponent::SetLightColor(FColor Color)
{
	if (LightColor != Color)
	{
		LightColor = Color;

		DirectionalLights::UpdateParameterCollection(this);
	}
}

void UGTDirectionalLightComponent::OnRegister()
{
	Super::OnRegister();

	if (IsVisible())
	{
		DirectionalLights::AddLight(this);
	}

#if WITH_EDITORONLY_DATA
	if (ArrowComponent != nullptr)
	{
		ArrowComponent->ArrowColor = GetLightColor();
	}
#endif // WITH_EDITORONLY_DATA
}

void UGTDirectionalLightComponent::OnUnregister()
{
	Super::OnUnregister();

	DirectionalLights::RemoveLight(this);
}

void UGTDirectionalLightComponent::OnVisibilityChanged()
{
	Super::OnVisibilityChanged();

	if (IsVisible())
	{
		DirectionalLights::AddLight(this);
	}
	else
	{
		DirectionalLights::RemoveLight(this);
	}
}

void UGTDirectionalLightComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	DirectionalLights::UpdateParameterCollection(this);
}

#if WITH_EDITOR
void UGTDirectionalLightComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	DirectionalLights::UpdateParameterCollection(this);

	if (ArrowComponent != nullptr)
	{
		ArrowComponent->ArrowColor = GetLightColor();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR
