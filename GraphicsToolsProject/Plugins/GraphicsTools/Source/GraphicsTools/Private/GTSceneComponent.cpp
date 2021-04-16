// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTSceneComponent.h"

#include "GTWorldSubsystem.h"
#include "GraphicsTools.h"

#include "Components/BillboardComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "UObject/ConstructorHelpers.h"

TArray<UGTSceneComponent*> UGTSceneComponent::Empty;

UGTSceneComponent::UGTSceneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsOnUpdateTransform = true;

	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> Finder(TEXT("/GraphicsTools/Materials/MPC_GTSettings"));
	check(Finder.Object);
	WorldParameterCollection = Finder.Object;

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

void UGTSceneComponent::SetParameterCollectionOverride(UMaterialParameterCollection* Override)
{
	RemoveFromWorldParameterCollection();
	ParameterCollectionOverride = (Override == WorldParameterCollection) ? nullptr : Override;
	AddToWorldParameterCollection();
}

void UGTSceneComponent::OnRegister()
{
	Super::OnRegister();

	if (IsVisible())
	{
		AddToWorldParameterCollection();
	}

#if WITH_EDITOR
	if (SpriteComponent != nullptr)
	{
		SpriteComponent->SetSprite(EditorTexture);
		SpriteComponent->SetRelativeScale3D(FVector(EditorTextureScale));
	}
#endif // WITH_EDITOR
}

void UGTSceneComponent::OnUnregister()
{
	Super::OnUnregister();

	RemoveFromWorldParameterCollection();
}

void UGTSceneComponent::OnVisibilityChanged()
{
	Super::OnVisibilityChanged();

	if (IsVisible())
	{
		AddToWorldParameterCollection();
	}
	else
	{
		RemoveFromWorldParameterCollection();
	}
}

void UGTSceneComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	UpdateParameterCollection();
}

#if WITH_EDITOR
void UGTSceneComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UpdateParameterCollection();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR

bool UGTSceneComponent::IsValid() const
{
	// Ensure the world isn't being destroyed since any systems storing soft pointers may assert.
	return (
		!IsEngineExitRequested() && GetWorld() != nullptr && GetWorld()->HasAnyFlags(RF_BeginDestroyed) == false &&
		GetWorld()->GetSubsystem<UGTWorldSubsystem>() != nullptr && GetParameterCollection() != nullptr);
}

const UMaterialParameterCollection* UGTSceneComponent::GetParameterCollection() const
{
	const UMaterialParameterCollection* CurrentCollection =
		HasParameterCollectionOverride() ? ParameterCollectionOverride : WorldParameterCollection;

	// Avoid returning a collection which is being destroyed since any systems storing soft pointers may assert.
	if (CurrentCollection != nullptr && CurrentCollection->HasAnyFlags(RF_BeginDestroyed))
	{
		return nullptr;
	}

	return CurrentCollection;
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

void UGTSceneComponent::AddToWorldParameterCollection()
{
	if (IsValid())
	{
		if (HasParameterCollectionOverride())
		{
			UpdateParameterCollection();
		}
		else
		{
			TArray<UGTSceneComponent*>& Components = GetWorldComponents();

			if (Components.Find(this) == INDEX_NONE)
			{
				Components.Add(this);
				UpdateParameterCollection();
			}
		}
	}
}

void UGTSceneComponent::RemoveFromWorldParameterCollection()
{
	if (IsValid())
	{
		if (HasParameterCollectionOverride())
		{
			UpdateParameterCollection(true);
		}
		else
		{
			TArray<UGTSceneComponent*>& Components = GetWorldComponents();
			const int32 ComponentIndex = Components.Find(this);

			if (ComponentIndex != INDEX_NONE)
			{
				// Disable the last component.
				Components[Components.Num() - 1]->UpdateParameterCollection(true);

				Components.RemoveAt(ComponentIndex);

				for (auto Component : Components)
				{
					Component->UpdateParameterCollection();
				}
			}
		}
	}
}
