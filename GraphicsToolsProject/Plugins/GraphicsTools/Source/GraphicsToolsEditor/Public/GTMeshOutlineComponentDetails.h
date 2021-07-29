// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

#include "Input/Reply.h"

class IDetailLayoutBuilder;
class UGTMeshOutlineComponent;

/**
 * Details panel customization that displays a button to serialize a outline mesh to a static mesh asset.
 */
class FGTMeshOutlineComponentDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it. */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface. */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	/** Handle clicking the convert button. */
	FReply ClickedOnConvertToStaticMesh();

	/** Is the convert button enabled. */
	bool ConvertToStaticMeshEnabled() const;

	/** Util to get the OutlineMeshComponent we want to convert. */
	UGTMeshOutlineComponent* GetFirstSelectedMeshOutlineComponent() const;

	/** Cached array of selected objects. */
	TArray<TWeakObjectPtr<UObject>> SelectedObjectsList;
};
