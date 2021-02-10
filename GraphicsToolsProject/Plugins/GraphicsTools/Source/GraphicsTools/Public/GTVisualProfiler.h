// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "GTVisualProfiler.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;

/**
 * TODO Timing calculations mirrored from FStatUnitData.
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
	T* CreateAndAttachComponent(FName Name, USceneComponent* Parent, FVector RelativeLocation, FRotator RelativeRotation)
	{
		T* Component = CreateDefaultSubobject<T>(Name);
		Component->SetupAttachment(Parent);
		Component->SetRelativeLocation(RelativeLocation);
		Component->SetRelativeRotation(RelativeRotation);
		return Component;
	}

	UStaticMeshComponent* CreateQuad(
		const FName& Name, USceneComponent* Parent, FVector RelativeLocation, FRotator RelativeRotation, FVector RelativeScale,
		FLinearColor Color);
	UTextRenderComponent* CreateText(
		const FName& Name, USceneComponent* Parent, FVector RelativeLocation, FRotator RelativeRotation, const FString& Text,
		float Size = 0.5f, bool LeftAlign = true);

	void UpdateTransform(float DeltaTime);

protected: // TEMP
	/** TODO. */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BackPlate = nullptr;

	/** TODO. */
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* FrameTimeLabelPrefix = nullptr;

	/** TODO. */
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* FrameTimeLabel = nullptr;

	/** TODO. */
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* GameThreadTimeLabelPrefix = nullptr;

	/** TODO. */
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* GameThreadTimeLabel = nullptr;

	/** TODO. */
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* RenderThreadTimeLabelPrefix = nullptr;

	/** TODO. */
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* RenderThreadTimeLabel = nullptr;

	/** TODO. */
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* GPUTimeLabelPrefix = nullptr;

	/** TODO. */
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* GPUTimeLabel = nullptr;

	/** TODO. */
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* DrawCallsLabel = nullptr;

	/** TODO. */
	UPROPERTY(VisibleAnywhere)
	UTextRenderComponent* PrimitivesLabel = nullptr;

private:
	UPROPERTY(Transient)
	UStaticMesh* DefaultQuadMesh = nullptr;

	UPROPERTY(Transient)
	UMaterial* DefaultMaterial = nullptr;

	UPROPERTY(Transient)
	UMaterial* DefaultTextMaterial = nullptr;

	/** Unit frame times filtered with a simple running average */
	float RenderThreadTime;
	float GameThreadTime;
	float GPUFrameTime[MAX_NUM_GPUS];
	float FrameTime;

	/** Raw equivalents of the above variables */
	float RawRenderThreadTime;
	float RawGameThreadTime;
	float RawGPUFrameTime[MAX_NUM_GPUS];
	float RawFrameTime;

	// int32 CurrentIndex;
	// TArray<float> RenderThreadTimes;
	// TArray<float> GameThreadTimes;
	// TArray<float> GPUFrameTimes[MAX_NUM_GPUS];
	// TArray<float> FrameTimes;
};
