// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTLightComponent.h"

#include "UObject/ConstructorHelpers.h"

UGTLightComponent::UGTLightComponent()
{
#if WITH_EDITORONLY_DATA
	if (!IsRunningCommandlet())
	{
		static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(TEXT("/Engine/EditorResources/LightIcons/S_LightPoint"));
		check(Texture.Object);
		EditorTexture = Texture.Object;
	}
#endif // WITH_EDITORONLY_DATA
}
