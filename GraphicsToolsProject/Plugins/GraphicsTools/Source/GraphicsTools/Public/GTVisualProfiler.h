// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "GTVisualProfiler.generated.h"

class UTextRenderComponent;

/**
 * TODO
 */
UCLASS(ClassGroup = GraphicsTools)
class GRAPHICSTOOLS_API AGTVisualProfiler : public AActor
{
	GENERATED_BODY()

public:
	AGTVisualProfiler();

private:
	//
	// AActor interface

	/** TODO. */
	virtual void BeginPlay() override;
	/** TODO. */
	virtual void Tick(float DeltaTime) override;

	/** Utility method to allocate and add a scene component to the profiler. */
	template <class T>
	T* CreateAndAttachComponent(FName Name, USceneComponent* Parent)
	{
		T* Component = CreateDefaultSubobject<T>(Name);
		Component->SetupAttachment(Parent);
		return Component;
	}

	/** TODO. */
	UPROPERTY()
	UTextRenderComponent* LabelComponent = nullptr;
};
