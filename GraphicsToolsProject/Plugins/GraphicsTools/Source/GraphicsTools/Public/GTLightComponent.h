// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Components/SceneComponent.h"

#include "GTLightComponent.generated.h"

class UMaterialParameterCollection;

/**
 * Abstract base class for all light actor components within Graphics Tools.
 */
UCLASS(Abstract, ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTLightComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UGTLightComponent();

	/** Accessor to the global Graphics Tools material parameter collection. */
	UMaterialParameterCollection* GetParameterCollection() { return ParameterCollection; }

	/** Const accessor to the global Graphics Tools material parameter collection. */
	const UMaterialParameterCollection* GetParameterCollection() const { return ParameterCollection; }

protected:
#if WITH_EDITOR
	//
	// UActorComponent interface

	/** Applies editor sprite textures. */
	virtual void OnRegister() override;
#endif // WITH_EDITOR

#if WITH_EDITORONLY_DATA
	/** Sprite for the light in the editor. */
	UPROPERTY(transient)
	UTexture2D* EditorTexture;

	/** Sprite scaling for the light in the editor. */
	UPROPERTY(transient)
	float EditorTextureScale;
#endif // WITH_EDITORONLY_DATA

private:
	/** The MaterialParameterCollection this light will write to. */
	UPROPERTY(Transient)
	UMaterialParameterCollection* ParameterCollection = nullptr;
};
