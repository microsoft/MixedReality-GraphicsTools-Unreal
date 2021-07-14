// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "GTVisualProfiler.generated.h"

class UMaterial;
class UStaticMeshComponent;
class UTextRenderComponent;

/**
 * The VisualProfiler provides a drop in, single actor class, solution for viewing your Windows Mixed Reality Unreal application's frame,
 * game, render, and GPU time. Missed frames are displayed as red text and bar graphs to find problem areas. Draw calls and primitive counts
 * (polygons/triangles) are reported as well.
 */
UCLASS(ClassGroup = GraphicsTools)
class GRAPHICSTOOLS_API AGTVisualProfiler : public AActor
{
	GENERATED_BODY()

public:
	AGTVisualProfiler();

	/** Getter to the FollowSpeed. */
	UFUNCTION(BlueprintPure, Category = "Visual Profiler")
	float GetFollowSpeed() const { return FollowSpeed; }

	/** Setter to the FollowSpeed. */
	UFUNCTION(BlueprintSetter, Category = "Visual Profiler")
	void SetFollowSpeed(float Speed) { FollowSpeed = Speed; }

	/** Getter to the FollowOffset. */
	UFUNCTION(BlueprintPure, Category = "Visual Profiler")
	FVector GetFollowOffset() const { return FollowOffset; }

	/** Setter to the FollowOffset. */
	UFUNCTION(BlueprintSetter, Category = "Visual Profiler")
	void SetFollowOffset(FVector Offset) { FollowOffset = Offset; }

	/** Getter to the PitchOffset. */
	UFUNCTION(BlueprintPure, Category = "Visual Profiler")
	float GetPitchOffset() const { return PitchOffset; }

	/** Setter to the PitchOffset. */
	UFUNCTION(BlueprintSetter, Category = "Visual Profiler")
	void SetPitchOffset(float Offset) { PitchOffset = Offset; }

private:
	//
	// AActor interface

	/** Updates frame timings and solves the profiler towards the camera. */
	virtual void Tick(float DeltaTime) override;

	/** Moves the profiler actor towards the first player controller's camera. */
	void SolveToCamera(float DeltaTime);

	/** Utility method to allocate and add a scene component to the profiler. */
	template <class T>
	T* CreateScene(FName Name, USceneComponent* Parent, FVector RelativeLocation, FRotator RelativeRotation)
	{
		T* Component = CreateDefaultSubobject<T>(Name);
		Component->SetupAttachment(Parent);
		Component->SetRelativeLocation(RelativeLocation);
		Component->SetRelativeRotation(RelativeRotation);
		return Component;
	}

	/** Creates a child actor component static mesh of a quad (plane). */
	UStaticMeshComponent* CreateQuad(
		const FName& Name, USceneComponent* Parent, FVector RelativeLocation, FVector RelativeScale, int32 SortPriority,
		FLinearColor Color = FLinearColor::White, FRotator RelativeRotation = FRotator(90, 0, 0));

	/** Creates a child actor component text renderer. */
	UTextRenderComponent* CreateText(
		const FName& Name, USceneComponent* Parent, FVector RelativeLocation, const FString& Text, int32 SortPriority,
		FRotator RelativeRotation = FRotator(0, 180, 0), float Size = 0.5f, bool LeftAlign = true);

	/** Applies the current time to the text label and bar graph. */
	void ApplyTiming(float Time, int32& PrevTime, UTextRenderComponent* Label, USceneComponent* Pivot);

	/** Converts a frame time to a scale used by the frame time bars. */
	float TimeToScale(float Time) const;

	/** Converts a frame time to a color used by the frame time labels. */
	FColor TimeToTextColor(float Time) const;

	/** Returns true if the time value presented to the user will change. */
	static bool CheckTimeDirty(float Time, int32& PrevTime);

	/** Returns true if the count value presented to the user will change. */
	static bool CheckCountDirty(int32 Count, int32& PrevCount);

	/** Enumerates the RHI for available refresh rates and picks the first valid one. */
	static float QueryThresholdFrameTime();

	/** How quickly to interpolate the profiler towards its target location and rotation. */
	UPROPERTY(
		EditAnywhere, Category = "Visual Profiler", BlueprintGetter = "GetFollowSpeed", BlueprintSetter = "SetFollowSpeed",
		meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float FollowSpeed = 5.0f;

	/** The offset from the target location in camera local space. */
	UPROPERTY(EditAnywhere, Category = "Visual Profiler", BlueprintGetter = "GetFollowOffset", BlueprintSetter = "SetFollowOffset")
	FVector FollowOffset = FVector(30.0f, 0.0f, -3.0f);

	/** Extra pitch applied on top of the target rotation (in degrees). */
	UPROPERTY(
		EditAnywhere, Category = "Visual Profiler", BlueprintGetter = "GetPitchOffset", BlueprintSetter = "SetPitchOffset",
		meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float PitchOffset = 18.0f;

	/** Assets used to construct the profiler. */
	UPROPERTY(Transient)
	UStaticMesh* DefaultQuadMesh = nullptr;
	UPROPERTY(Transient)
	UMaterial* DefaultMaterial = nullptr;
	UPROPERTY(Transient)
	UMaterial* DefaultTextMaterial = nullptr;

	/** Profiler components. */
	UPROPERTY(Transient)
	UTextRenderComponent* FrameTimeLabel = nullptr;
	UPROPERTY(Transient)
	USceneComponent* FrameTimePivot = nullptr;
	UPROPERTY(Transient)
	UTextRenderComponent* GameThreadTimeLabel = nullptr;
	UPROPERTY(Transient)
	USceneComponent* GameThreadTimePivot = nullptr;
	UPROPERTY(Transient)
	UTextRenderComponent* RenderThreadTimeLabel = nullptr;
	UPROPERTY(Transient)
	USceneComponent* RenderThreadTimePivot = nullptr;
	UPROPERTY(Transient)
	UTextRenderComponent* GPUTimeLabel = nullptr;
	UPROPERTY(Transient)
	USceneComponent* GPUTimePivot = nullptr;
	UPROPERTY(Transient)
	UTextRenderComponent* DrawCallsLabel = nullptr;
	UPROPERTY(Transient)
	UTextRenderComponent* PrimitivesLabel = nullptr;

	/** Has the solver snapped to the camera yet. */
	bool bSnapped;

	/** The time in milliseconds before a frame cannot be completed in time to match the vsync. */
	float ThresholdFrameTime;

	/** Unit frame times filtered with a simple running average. */
	float FrameTime;
	float RenderThreadTime;
	float GameThreadTime;
	float GPUFrameTime;

	/** Cache of above frame timings after formatting. Used to avoid unnecessary updates. */
	int32 PrevFrameTime;
	int32 PrevRenderThreadTime;
	int32 PrevGameThreadTime;
	int32 PrevGPUFrameTime;

	/** Cache of other stats. Used to avoid unnecessary updates. */
	int32 PrevNumDrawCalls;
	int32 PrevNumPrimitives;
};
