// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Components/SceneComponent.h"

#include "GTLightComponent.generated.h"

/**
 * TODO
 */
UCLASS(Abstract, ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTLightComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UGTLightComponent();

protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	/** UActorComponent Interface */
	virtual void OnRegister() override;
#endif

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#if WITH_EDITORONLY_DATA
	/** Sprite for the light in the editor. */
	UPROPERTY(transient)
	UTexture2D* EditorTexture;

	/** Sprite scaling for the light in the editor. */
	UPROPERTY(transient)
	float EditorTextureScale;
#endif
};
