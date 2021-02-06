// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTVisualProfiler.h"

#include "GraphicsTools.h"

#include "Components/TextRenderComponent.h"
#include "UObject/ConstructorHelpers.h"

AGTVisualProfiler::AGTVisualProfiler()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultTextMaterial(TEXT("Material'/Engine/EngineMaterials/UnlitText.UnlitText'"));
	check(DefaultTextMaterial.Object);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VisualProfiler"));

	LabelComponent = CreateAndAttachComponent<UTextRenderComponent>(TEXT("Label"), RootComponent);
	LabelComponent->SetMaterial(0, DefaultTextMaterial.Object);
	LabelComponent->SetText(FText::AsCultureInvariant("Hello World"));
}

void AGTVisualProfiler::BeginPlay()
{
	Super::BeginPlay();
}

void AGTVisualProfiler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Draw calls
	{
		const int32 NumDrawCalls = GNumDrawCallsRHI;
		LabelComponent->SetText(FText::Format(FText::AsCultureInvariant("Draw Calls: {0}"), FText::AsNumber(NumDrawCalls)));
		UE_LOG(GraphicsTools, Warning, TEXT("Num draw calls: %i"), NumDrawCalls);
	}

	// Primitives
	{
		int32 NumPrimitives = GNumPrimitivesDrawnRHI;
	}
}
