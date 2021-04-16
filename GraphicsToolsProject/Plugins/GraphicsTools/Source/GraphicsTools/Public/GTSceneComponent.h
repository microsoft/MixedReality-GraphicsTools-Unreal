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

	/** Specifies the current WorldParameterCollection override, if null is passed in the component will start writing to the
	 * WorldParameterCollection. */
	UFUNCTION(BlueprintSetter, Category = "GT Scene Component")
	void SetParameterCollectionOverride(UMaterialParameterCollection* Override);

	/** Returns true if the WorldParameterCollection is currently being overridden. */
	UFUNCTION(BlueprintPure, Category = "GT Scene Component")
	bool HasParameterCollectionOverride() const { return ParameterCollectionOverride != nullptr; }

protected:
	//
	// UActorComponent interface

	/** Adds the component to the world and applies editor sprite textures. */
	virtual void OnRegister() override;

	/** Removes the component from the world list. */
	virtual void OnUnregister() override;

	/** Adds or removes the component to the world list based on visibility. */
	virtual void OnVisibilityChanged() override;

	//
	// USceneComponent interface

	/** Updates the parameter collection when the transform changes. */
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport = ETeleportType::None) override;

#if WITH_EDITOR
	/** Updates the material parameter collection. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

	/** Returns true if the scene belongs to a world and has a material parameter collection. */
	virtual bool IsValid() const;

	/** Const accessor to the current material parameter collection in use. */
	const UMaterialParameterCollection* GetParameterCollection() const;

	/** Sets a vector value on the current material parameter collection, returns true if successful. */
	bool SetVectorParameterValue(FName ParameterName, const FLinearColor& ParameterValue);

	/** Adds this component to a list which will be processed when writing to the WorldParameterCollection. */
	void AddToWorldParameterCollection();

	/** Removes this component from a list which will be processed when writing to the WorldParameterCollection. */
	void RemoveFromWorldParameterCollection();

	/** Pure virtual accessor to all components of a specific type within a world writing to the WorldParameterCollection. */
	virtual TArray<UGTSceneComponent*>& GetWorldComponents() PURE_VIRTUAL(UGTSceneComponent::UpdateParameterCollection, return Empty;);

	/** Pure virtual method that updates the parameter collection based on the current type. */
	virtual void UpdateParameterCollection(bool IsDisabled = false) PURE_VIRTUAL(UGTSceneComponent::UpdateParameterCollection, );

#if WITH_EDITORONLY_DATA
	/** Sprite for the scene in the editor. */
	UPROPERTY(transient)
	UTexture2D* EditorTexture;

	/** Sprite scaling for the scene in the editor. */
	UPROPERTY(transient)
	float EditorTextureScale;
#endif // WITH_EDITORONLY_DATA

private:
	/** An override that removes the component from the WorldParameterCollection and allows the user to control what
	 * MaterialParameterCollection gets written to. */
	UPROPERTY(EditAnywhere, Category = "GT Scene Component", BlueprintSetter = "SetParameterCollectionOverride", AdvancedDisplay)
	UMaterialParameterCollection* ParameterCollectionOverride = nullptr;

	/** The default MaterialParameterCollection all components within the current world will write to. */
	UPROPERTY(Transient)
	UMaterialParameterCollection* WorldParameterCollection = nullptr;

	/** Empty component list which is used by the pure virtual GetWorldComponents method. */
	static TArray<UGTSceneComponent*> Empty;
};
