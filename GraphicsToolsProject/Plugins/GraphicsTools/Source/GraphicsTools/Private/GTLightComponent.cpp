// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTLightComponent.h"
#include "Components/BillboardComponent.h"

UGTLightComponent::UGTLightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

#if WITH_EDITORONLY_DATA
	bVisualizeComponent = true;

	if (!IsRunningCommandlet())
	{
		static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(TEXT("/Engine/EditorResources/LightIcons/S_LightPointMove"));
		check(Texture.Object);
		EditorTexture = Texture.Object;
		EditorTextureScale = 0.5f;
	}
#endif
}

// Called when the game starts
void UGTLightComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UGTLightComponent::OnRegister()
{
	Super::OnRegister();

	if (SpriteComponent != nullptr)
	{
		SpriteComponent->SetSprite(EditorTexture);
		SpriteComponent->SetRelativeScale3D(FVector(EditorTextureScale));
	}
}

void UGTLightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
