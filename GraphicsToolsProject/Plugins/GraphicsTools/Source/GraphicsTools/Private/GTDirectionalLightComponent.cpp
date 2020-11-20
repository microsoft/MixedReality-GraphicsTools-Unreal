// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTDirectionalLightComponent.h"

#include "GraphicsTools.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

struct DirectionalLights
{
public:
	typedef TArray<UGTDirectionalLightComponent*> LightList;

	LightList& GetLightList(bool bInGameWorld) { return bInGameWorld ? GameLights : EditorLights; }

	void AddLight(UGTDirectionalLightComponent* Light)
	{
		LightList& Lights = GetLightList(Light->GetWorld()->IsGameWorld());

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
		LightList& Lights = GetLightList(Light->GetWorld()->IsGameWorld());
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

	void SetVectorParameterValue(
		UMaterialParameterCollectionInstance* ParameterCollectionInstance, FName ParameterName, const FLinearColor& ParameterValue)
	{
		const bool bFoundParameter = ParameterCollectionInstance->SetVectorParameterValue(ParameterName, ParameterValue);

		if (!bFoundParameter)
		{
			UE_LOG(
				GraphicsTools, Warning, TEXT("Unable to find %s parameter in material parameter collection %s."), *ParameterName.ToString(),
				*ParameterCollectionInstance->GetCollection()->GetPathName());
		}
	}

	void UpdateParameterCollection(UGTDirectionalLightComponent* Light, bool LightEnabled = true)
	{
		// Ensure the light's world isn't being destroyed. If a light is in a world being destroyed the ParameterCollectionInstance will be
		// invalid.
		if (Light->GetParameterCollection() == nullptr || Light->GetWorld()->HasAnyFlags(RF_BeginDestroyed))
		{
			return;
		}

		const LightList& Lights = GetLightList(Light->GetWorld()->IsGameWorld());
		const int32 LightIndex = Lights.Find(Light);

		// Only the first directional light will be considered (or a disabled light in the case of removing the last light).
		if (LightEnabled && (LightIndex != 0))
		{
			return;
		}

		UMaterialParameterCollectionInstance* ParameterCollectionInstance =
			Light->GetWorld()->GetParameterCollectionInstance(Light->GetParameterCollection());

		{
			static FName DirectionEnabledParameterName("DirectionalLightDirectionEnabled");
			FLinearColor DirectionEnabled(-Light->GetForwardVector());
			DirectionEnabled.A = LightEnabled ? 1.0f : 0.0f;
			SetVectorParameterValue(ParameterCollectionInstance, DirectionEnabledParameterName, DirectionEnabled);
		}
		{
			static FName ColorIntensityParameterName("DirectionalLightColorIntensity");
			FLinearColor ColorIntensity(Light->GetLightColor());
			ColorIntensity.A = Light->GetLightIntensity();
			SetVectorParameterValue(ParameterCollectionInstance, ColorIntensityParameterName, ColorIntensity);
		}
	}

private:
	LightList GameLights;
	LightList EditorLights;
} GlobalDirectionalLights;

UGTDirectionalLightComponent::UGTDirectionalLightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsOnUpdateTransform = true;

#if WITH_EDITORONLY_DATA
	if (!IsRunningCommandlet())
	{
		static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(
			TEXT("/Engine/EditorResources/LightIcons/S_LightDirectionalMove")); // TODO, show arrow, implement intensity.
		check(Texture.Object);
		EditorTexture = Texture.Object;
	}
#endif
}

void UGTDirectionalLightComponent::SetLightIntensity(float Intensity)
{
	if (LightIntensity != Intensity)
	{
		LightIntensity = Intensity;

		GlobalDirectionalLights.UpdateParameterCollection(this);
	}
}

void UGTDirectionalLightComponent::SetLightColor(FColor Color)
{
	if (LightColor != Color)
	{
		LightColor = Color;

		GlobalDirectionalLights.UpdateParameterCollection(this);
	}
}

void UGTDirectionalLightComponent::OnRegister()
{
	Super::OnRegister();

	if (IsVisible())
	{
		GlobalDirectionalLights.AddLight(this);
	}
}

void UGTDirectionalLightComponent::OnUnregister()
{
	Super::OnUnregister();

	GlobalDirectionalLights.RemoveLight(this);
}

void UGTDirectionalLightComponent::OnVisibilityChanged()
{
	Super::OnVisibilityChanged();

	if (IsVisible())
	{
		GlobalDirectionalLights.AddLight(this);
	}
	else
	{
		GlobalDirectionalLights.RemoveLight(this);
	}
}

void UGTDirectionalLightComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	GlobalDirectionalLights.UpdateParameterCollection(this);
}

#if WITH_EDITOR
void UGTDirectionalLightComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	GlobalDirectionalLights.UpdateParameterCollection(this);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
