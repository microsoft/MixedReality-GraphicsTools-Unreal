// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "Components/SceneComponent.h"

#include "GTSceneComponent.generated.h"

class UMaterialParameterCollection;

/**
 * Abstract base class for all scene actor components within Graphics Tools.
 */
UCLASS(Abstract, ClassGroup = (GraphicsTools), meta = (BlueprintSpawnableComponent))
class GRAPHICSTOOLS_API UGTSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UGTSceneComponent();

	/** Sets a vector value on the current material parameter collection, returns true if successful. */
	bool SetVectorParameterValue(FName ParameterName, const FLinearColor& ParameterValue);

	/** Returns true if the scene belongs to a world and has a material parameter collection. */
	virtual bool IsValid() const;

protected:
	/** Const accessor to the current material parameter collection. */
	const UMaterialParameterCollection* GetParameterCollection() const;

#if WITH_EDITOR
	//
	// UActorComponent interface

	/** Applies editor sprite textures. */
	virtual void OnRegister() override;
#endif // WITH_EDITOR

#if WITH_EDITORONLY_DATA
	/** Sprite for the scene in the editor. */
	UPROPERTY(transient)
	UTexture2D* EditorTexture;

	/** Sprite scaling for the scene in the editor. */
	UPROPERTY(transient)
	float EditorTextureScale;
#endif // WITH_EDITORONLY_DATA

private:
	/** The MaterialParameterCollection this scene will write to. */
	UPROPERTY(Transient)
	UMaterialParameterCollection* ParameterCollection = nullptr;
};
