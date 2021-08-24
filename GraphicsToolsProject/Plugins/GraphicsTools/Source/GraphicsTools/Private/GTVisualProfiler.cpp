// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTVisualProfiler.h"

#include "GraphicsTools.h"

#include "Camera/PlayerCameraManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/PlayerController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/EngineVersionComparison.h"
#include "UObject/ConstructorHelpers.h"

const float DefaultThresholdFrameTime = (1.0f / 60.0f) * 1000; // Default to 16.6ms.
const int32 SortPriorityLow = 100;
const int32 SortPriorityMed = 101;
const int32 SortPriorityHigh = 102;

AGTVisualProfiler::AGTVisualProfiler()
	: bSnapped(false)
	, ThresholdFrameTime(DefaultThresholdFrameTime)
	, FrameTime(0.0f)
	, RenderThreadTime(0.0f)
	, GameThreadTime(0.0f)
	, GPUFrameTime(0.0f)
	, PrevFrameTime(0)
	, PrevRenderThreadTime(0)
	, PrevGameThreadTime(0)
	, PrevGPUFrameTime(0)
	, PrevNumDrawCalls(0)
	, PrevNumPrimitives(0)
{
	PrimaryActorTick.bCanEverTick = true;

	ThresholdFrameTime = QueryThresholdFrameTime();

	// Acquire default meshes and materials.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultQuadMeshFinder(TEXT("/Engine/BasicShapes/Plane"));
	check(DefaultQuadMeshFinder.Object);
	DefaultQuadMesh = DefaultQuadMeshFinder.Object;

	// Note, the default visual profiler materials are translucent but write depth on HoloLens 2 to support depth based late stage
	// re-projection. They are transparent so that we can mark them to ignore depth testing so that the profiler renders on top of
	// everything else.
	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMaterialFinder(TEXT("/GraphicsTools/Materials/M_VisualProfiler"));
	DefaultMaterial = DefaultMaterialFinder.Object;

	if (DefaultMaterial == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> BackupMaterialFinder(
			TEXT("/Engine/EngineDebugMaterials/LevelColorationUnlitMaterial"));
		DefaultMaterial = BackupMaterialFinder.Object;
	}

	check(DefaultMaterial);

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultTextMaterialFinder(TEXT("/GraphicsTools/Materials/M_VisualProfilerText"));
	DefaultTextMaterial = DefaultTextMaterialFinder.Object;

	if (DefaultTextMaterial == nullptr)
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> BackupTextMaterialFinder(TEXT("/Engine/EngineMaterials/UnlitText"));
		DefaultTextMaterial = BackupTextMaterialFinder.Object;
	}

	check(DefaultTextMaterial);

	// UI constants.
	static const float PrefixYOffset = -3.9f;
	static const float LabelYOffset = -2.4f;
	static const float PivotYOffset = 0;
	static const float HeightZOffset = 0.45f;
	static const FVector BarPivot(0, 1, 0);
	static const FVector BarSize(0.004f, 0.02f, 1);
	static const FRotator QuadRotation(90, 0, 0);
	static const FString UnavailableLabel(TEXT("Unavailable"));
	static const FLinearColor BackPlateColor(FColor(80, 80, 80));     // Dark Gray
	static const FLinearColor FrameTimeColor(FColor(0, 164, 239));    // Vivid Cerulean
	static const FLinearColor GameThreadColor(FColor(255, 185, 0));   // Selective Yellow
	static const FLinearColor RenderThreadColor(FColor(242, 80, 34)); // Orioles Orange
	static const FLinearColor GPUTimeColor(FColor(127, 186, 0));      // Apple Green

	// Build the profiler by creating child components.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VisualProfiler"));
	RootComponent->SetAutoActivate(true);

	FVector Location(-0.02f, 0, 0.9f);

	CreateQuad(TEXT("BackPlate"), RootComponent, FVector::ZeroVector, FVector(0.025f, 0.08f, 1), SortPriorityLow, BackPlateColor);

	Location.Y = PrefixYOffset;
	CreateText(TEXT("FrameTimeLabelPrefix"), RootComponent, Location, TEXT("Frame: "), SortPriorityMed);
	Location.Y = LabelYOffset;
	FrameTimeLabel = CreateText(TEXT("FrameTimeLabel"), RootComponent, Location, UnavailableLabel, SortPriorityMed);
	Location.Y = PivotYOffset;
	FrameTimePivot = CreateScene<USceneComponent>(TEXT("FrameTimePivot"), RootComponent, Location, QuadRotation);
	FrameTimePivot->SetRelativeScale3D(FVector(0, 1, 0));
	CreateQuad(TEXT("FrameTimePivotQuad"), FrameTimePivot, BarPivot, BarSize, SortPriorityMed, FrameTimeColor, FRotator::ZeroRotator);

	Location.Z = Location.Z - HeightZOffset;

	Location.Y = PrefixYOffset;
	CreateText(TEXT("GameThreadTimeLabelPrefix"), RootComponent, Location, TEXT("Game: "), SortPriorityMed);
	Location.Y = LabelYOffset;
	GameThreadTimeLabel = CreateText(TEXT("GameThreadTimeLabel"), RootComponent, Location, UnavailableLabel, SortPriorityMed);
	Location.Y = PivotYOffset;
	GameThreadTimePivot = CreateScene<USceneComponent>(TEXT("GameThreadTimePivot"), RootComponent, Location, QuadRotation);
	GameThreadTimePivot->SetRelativeScale3D(FVector(0, 1, 0));
	CreateQuad(TEXT("GameTimePivotQuad"), GameThreadTimePivot, BarPivot, BarSize, SortPriorityMed, GameThreadColor, FRotator::ZeroRotator);

	Location.Z = Location.Z - (HeightZOffset * 0.5f);

	CreateQuad(
		TEXT("TargetLine"), RootComponent, FVector(Location.X * 2, BarPivot.Y * 2, Location.Z), FVector(0.018f, 0.0005f, 1),
		SortPriorityHigh);

	Location.Z = Location.Z - (HeightZOffset * 0.5f);

	Location.Y = PrefixYOffset;
	CreateText(TEXT("RenderThreadTimeLabelPrefix"), RootComponent, Location, TEXT("Draw: "), SortPriorityMed);
	Location.Y = LabelYOffset;
	RenderThreadTimeLabel = CreateText(TEXT("RenderThreadTimeLabel"), RootComponent, Location, UnavailableLabel, SortPriorityMed);
	Location.Y = PivotYOffset;
	RenderThreadTimePivot = CreateScene<USceneComponent>(TEXT("RenderThreadTimePivot"), RootComponent, Location, QuadRotation);
	RenderThreadTimePivot->SetRelativeScale3D(FVector(0, 1, 0));
	CreateQuad(
		TEXT("RenderThreadTimePivotQuad"), RenderThreadTimePivot, BarPivot, BarSize, SortPriorityMed, RenderThreadColor,
		FRotator::ZeroRotator);

	Location.Z = Location.Z - HeightZOffset;

	Location.Y = PrefixYOffset;
	CreateText(TEXT("GPUTimeLabelPrefix"), RootComponent, Location, TEXT("GPU: "), SortPriorityMed);
	Location.Y = LabelYOffset;
	GPUTimeLabel = CreateText(TEXT("GPUTimeLabel"), RootComponent, Location, UnavailableLabel, SortPriorityMed);
	Location.Y = PivotYOffset;
	GPUTimePivot = CreateScene<USceneComponent>(TEXT("GPUTimePivot"), RootComponent, Location, QuadRotation);
	GPUTimePivot->SetRelativeScale3D(FVector(0, 1, 0));
	CreateQuad(TEXT("GPUTimePivotQuad"), GPUTimePivot, BarPivot, BarSize, SortPriorityMed, GPUTimeColor, FRotator::ZeroRotator);

	Location.Z = Location.Z - HeightZOffset;

	Location.Y = PrefixYOffset;
	DrawCallsLabel = CreateText(TEXT("DrawCallsLabel"), RootComponent, Location, TEXT("Draw Calls: 0"), SortPriorityMed);
	Location.Y = 0;
	PrimitivesLabel = CreateText(TEXT("PrimitivesLabel"), RootComponent, Location, TEXT("Polys: 0"), SortPriorityMed);
}

void AGTVisualProfiler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RootComponent->IsActive() && RootComponent->IsVisible())
	{
		// Calculate the current frame times. (Timing calculations mirrored from FStatUnitData.)
		{
			float DiffTime = FApp::GetCurrentTime() - FApp::GetLastTime();
			float RawFrameTime = DiffTime * 1000.0f;
			FrameTime = 0.9 * FrameTime + 0.1 * RawFrameTime;

			// Number of milliseconds the game thread was used last frame.
			float RawGameThreadTime = FPlatformTime::ToMilliseconds(GGameThreadTime);
			GameThreadTime = 0.9 * GameThreadTime + 0.1 * RawGameThreadTime;

			// Number of milliseconds the render thread was used last frame.
			float RawRenderThreadTime = FPlatformTime::ToMilliseconds(GRenderThreadTime);
			RenderThreadTime = 0.9 * RenderThreadTime + 0.1 * RawRenderThreadTime;

			// Number of milliseconds the GPU was busy last frame.
			const uint32 GPUCycles = RHIGetGPUFrameCycles(0); // We only track the first GPU.
			float RawGPUFrameTime = FPlatformTime::ToMilliseconds(GPUCycles);
			GPUFrameTime = 0.9 * GPUFrameTime + 0.1 * RawGPUFrameTime;

			ApplyTiming(FrameTime, PrevFrameTime, FrameTimeLabel, FrameTimePivot);
			ApplyTiming(GameThreadTime, PrevGameThreadTime, GameThreadTimeLabel, GameThreadTimePivot);
			ApplyTiming(RenderThreadTime, PrevRenderThreadTime, RenderThreadTimeLabel, RenderThreadTimePivot);
			ApplyTiming(GPUFrameTime, PrevGPUFrameTime, GPUTimeLabel, GPUTimePivot);
		}

		// Draw calls.
		{
			static const int32 ProfilerDrawCalls = 16; // Removed profiling induced draw calls.

#if UE_VERSION_OLDER_THAN(4, 27, 0)
			const int32 NumDrawCalls = FMath::Max(GNumDrawCallsRHI - ProfilerDrawCalls, 0);
#else
			const int32 NumDrawCalls = FMath::Max(GNumDrawCallsRHI[0] - ProfilerDrawCalls, 0);
#endif

			if (CheckCountDirty(NumDrawCalls, PrevNumDrawCalls))
			{
				DrawCallsLabel->SetText(FText::Format(FText::AsCultureInvariant("Draw Calls: {0}"), FText::AsNumber(NumDrawCalls)));
			}
		}

		// Primitives.
		{
			static const int32 ProfilerPrimitives = 336; // Removed profiling induced primitives.

#if UE_VERSION_OLDER_THAN(4, 27, 0)
			int32 NumPrimitives = FMath::Max(GNumPrimitivesDrawnRHI - ProfilerPrimitives, 0);
#else
			int32 NumPrimitives = FMath::Max(GNumPrimitivesDrawnRHI[0] - ProfilerPrimitives, 0);
#endif
			if (CheckCountDirty(NumPrimitives, PrevNumPrimitives))
			{
				if (NumPrimitives < 10000)
				{
					PrimitivesLabel->SetText(FText::Format(FText::AsCultureInvariant("Polys: {0}"), FText::AsNumber(NumPrimitives)));
				}
				else
				{
					float NumPrimitivesK = NumPrimitives / 1000.f;
					PrimitivesLabel->SetText(FText::AsCultureInvariant(FString::Printf(TEXT("Polys: %.1fK"), NumPrimitivesK)));
				}
			}
		}

		SolveToCamera(DeltaTime);
	}
}

void AGTVisualProfiler::SolveToCamera(float DeltaTime)
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager)
		{
			const FVector CameraLocation = CameraManager->GetCameraLocation();
			const FRotator CameraRotation = CameraManager->GetCameraRotation();

			FVector TargetLocation = CameraLocation + CameraRotation.RotateVector(FollowOffset);
			FQuat TargetRotation = CameraRotation.Quaternion();
			TargetRotation *= FQuat(FVector(0, 1, 0), FMath::DegreesToRadians(PitchOffset));

			const float T = bSnapped ? FMath::Clamp(DeltaTime * FollowSpeed, 0.0f, 1.0f) : 1;
			SetActorLocationAndRotation(
				FMath::Lerp(GetActorLocation(), TargetLocation, T), FQuat::Slerp(GetActorQuat(), TargetRotation, T));

			bSnapped = true;
		}
	}
}

UStaticMeshComponent* AGTVisualProfiler::CreateQuad(
	const FName& Name, USceneComponent* Parent, FVector RelativeLocation, FVector RelativeScale, int32 SortPriority, FLinearColor Color,
	FRotator RelativeRotation)
{
	UStaticMeshComponent* Component = CreateScene<UStaticMeshComponent>(Name, Parent, RelativeLocation, RelativeRotation);
	Component->SetRelativeScale3D(RelativeScale);
	Component->SetStaticMesh(DefaultQuadMesh);
	UMaterialInstanceDynamic* Material = Component->CreateDynamicMaterialInstance(0, DefaultMaterial);
	Material->SetVectorParameterValue(TEXT("Color"), Color);
	Component->SetMaterial(0, Material);
	Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Component->SetTranslucentSortPriority(SortPriority);

	return Component;
}

UTextRenderComponent* AGTVisualProfiler::CreateText(
	const FName& Name, USceneComponent* Parent, FVector RelativeLocation, const FString& Text, int32 SortPriority,
	FRotator RelativeRotation, float Size, bool LeftAlign)
{
	UTextRenderComponent* Component = CreateScene<UTextRenderComponent>(Name, Parent, RelativeLocation, RelativeRotation);
	Component->SetMaterial(0, DefaultTextMaterial);
	Component->SetWorldSize(Size);
	Component->SetHorizontalAlignment(LeftAlign ? EHorizTextAligment::EHTA_Left : EHorizTextAligment::EHTA_Right);
	Component->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Component->SetText(FText::AsCultureInvariant(Text));
	Component->SetTranslucentSortPriority(SortPriority);

	return Component;
}

void AGTVisualProfiler::ApplyTiming(float Time, int32& PrevTime, UTextRenderComponent* Label, USceneComponent* Pivot)
{
	if (AGTVisualProfiler::CheckTimeDirty(Time, PrevTime))
	{
		Label->SetText(FText::AsCultureInvariant(FString::Printf(TEXT("%3.2f ms"), Time)));
		Label->SetTextRenderColor(TimeToTextColor(Time));
		Pivot->SetRelativeScale3D(FVector(1, TimeToScale(Time), 1));
	}
}

float AGTVisualProfiler::TimeToScale(float Time) const
{
	return FMath::Clamp(Time / ThresholdFrameTime, 0.0f, 2.0f);
}

FColor AGTVisualProfiler::TimeToTextColor(float Time) const
{
	static const FColor MissedFrameColor(255, 20, 5); // Orange
	return (static_cast<int32>(Time) > (static_cast<int32>(ThresholdFrameTime) + 1)) ? MissedFrameColor : FColor::White;
}

bool AGTVisualProfiler::CheckTimeDirty(float Time, int32& PrevTime)
{
	int32 NewTime = static_cast<int32>(Time * 100);
	const bool Dirty = NewTime != PrevTime;
	PrevTime = NewTime;

	return Dirty;
}

bool AGTVisualProfiler::CheckCountDirty(int32 Count, int32& PrevCount)
{
	const bool Dirty = Count != PrevCount;
	PrevCount = Count;

	return Dirty;
}

float AGTVisualProfiler::QueryThresholdFrameTime()
{
	float Result = -1;
	FScreenResolutionArray Resolutions;

	if (RHIGetAvailableResolutions(Resolutions, false))
	{
		for (int32 Index = 0; Index < Resolutions.Num(); ++Index)
		{
			if (Resolutions[Index].RefreshRate != 0)
			{
				Result = (1.0f / Resolutions[Index].RefreshRate) * 1000.0f;
				break;
			}
		}
	}

	return (Result <= 0) ? DefaultThresholdFrameTime : Result;
}
