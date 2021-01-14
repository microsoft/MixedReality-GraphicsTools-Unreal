// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTLightComponent.h"

#include "GTWorldSubsystem.h"

#include "Components/BillboardComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "UObject/ConstructorHelpers.h"

UGTLightComponent::UGTLightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> Finder(TEXT("/GraphicsTools/Materials/MPC_GTSettings"));
	check(Finder.Object);
	ParameterCollection = Finder.Object;

#if WITH_EDITORONLY_DATA
	bVisualizeComponent = true;

	if (!IsRunningCommandlet())
	{
		static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(TEXT("/Engine/EditorResources/LightIcons/S_LightPoint"));
		check(Texture.Object);
		EditorTexture = Texture.Object;
		EditorTextureScale = 0.5f;
	}
#endif // WITH_EDITORONLY_DATA
}

UMaterialParameterCollection* UGTLightComponent::GetParameterCollection()
{
	// Avoid returning a collection which is being destroyed since any systems storing soft pointers may assert.
	if (ParameterCollection != nullptr && ParameterCollection->HasAnyFlags(RF_BeginDestroyed))
	{
		return nullptr;
	}

	return ParameterCollection;
}

const UMaterialParameterCollection* UGTLightComponent::GetParameterCollection() const
{
	return const_cast<UGTLightComponent*>(this)->GetParameterCollection();
}

bool UGTLightComponent::IsValid() const
{
	return (GetWorld() != nullptr && GetWorld()->GetSubsystem<UGTWorldSubsystem>() != nullptr && GetParameterCollection() != nullptr);
}

#if WITH_EDITOR
void UGTLightComponent::OnRegister()
{
	Super::OnRegister();

	if (SpriteComponent != nullptr)
	{
		SpriteComponent->SetSprite(EditorTexture);
		SpriteComponent->SetRelativeScale3D(FVector(EditorTextureScale));
	}
}
#endif // WITH_EDITOR
