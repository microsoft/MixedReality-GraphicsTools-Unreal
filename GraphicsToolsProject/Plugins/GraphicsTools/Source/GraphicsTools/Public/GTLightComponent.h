// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Components/SceneComponent.h"

#include "GTLightComponent.generated.h"

class UMaterialParameterCollection;

/**
 * TODO
 */
UCLASS(Abstract, ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTLightComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UGTLightComponent();

	/** TODO */
	UMaterialParameterCollection* GetParameterCollection() { return ParameterCollection; }

	/** TODO */
	const UMaterialParameterCollection* GetParameterCollection() const { return ParameterCollection; }

protected:
#if WITH_EDITOR
	//
	// UActorComponent interface

	/** TODO */
	virtual void OnRegister() override;
#endif

#if WITH_EDITORONLY_DATA
	/** Sprite for the light in the editor. */
	UPROPERTY(transient)
	UTexture2D* EditorTexture;

	/** Sprite scaling for the light in the editor. */
	UPROPERTY(transient)
	float EditorTextureScale;
#endif

private:
	/** The MaterialParameterCollection this light will write to. */
	UPROPERTY(Transient)
	UMaterialParameterCollection* ParameterCollection = nullptr;
};
