// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTSceneComponent.h"

#include "GTWorldSubsystem.h"
#include "GraphicsTools.h"

#include "Components/BillboardComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "UObject/ConstructorHelpers.h"

UGTSceneComponent::UGTSceneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> Finder(TEXT("/GraphicsTools/Materials/MPC_GTSettings"));
	check(Finder.Object);
	ParameterCollection = Finder.Object;

#if WITH_EDITORONLY_DATA
	bVisualizeComponent = true;

	if (!IsRunningCommandlet())
	{
		static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(TEXT("/Engine/EditorResources/EmptyActor.EmptyActor"));
		check(Texture.Object);
		EditorTexture = Texture.Object;
		EditorTextureScale = 0.5f;
	}
#endif // WITH_EDITORONLY_DATA
}

bool UGTSceneComponent::SetVectorParameterValue(FName ParameterName, const FLinearColor& ParameterValue)
{
	UMaterialParameterCollectionInstance* ParameterCollectionInstance =
		GetWorld()->GetParameterCollectionInstance(GetParameterCollection());

	if (ParameterCollectionInstance != nullptr)
	{
		if (ParameterCollectionInstance->SetVectorParameterValue(ParameterName, ParameterValue))
		{
			return true;
		}

		UE_LOG(
			GraphicsTools, Warning, TEXT("Unable to find %s parameter in material parameter collection %s."), *ParameterName.ToString(),
			*ParameterCollectionInstance->GetCollection()->GetPathName());
	}

	return false;
}

bool UGTSceneComponent::IsValid() const
{
	return (GetWorld() != nullptr && GetWorld()->GetSubsystem<UGTWorldSubsystem>() != nullptr && GetParameterCollection() != nullptr);
}

const UMaterialParameterCollection* UGTSceneComponent::GetParameterCollection() const
{
	// Avoid returning a collection which is being destroyed since any systems storing soft pointers may assert.
	if (ParameterCollection != nullptr && ParameterCollection->HasAnyFlags(RF_BeginDestroyed))
	{
		return nullptr;
	}

	return ParameterCollection;
}

#if WITH_EDITOR
void UGTSceneComponent::OnRegister()
{
	Super::OnRegister();

	if (SpriteComponent != nullptr)
	{
		SpriteComponent->SetSprite(EditorTexture);
		SpriteComponent->SetRelativeScale3D(FVector(EditorTextureScale));
	}
}
#endif // WITH_EDITOR
