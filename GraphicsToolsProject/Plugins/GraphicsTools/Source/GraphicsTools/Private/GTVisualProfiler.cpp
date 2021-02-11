// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTVisualProfiler.h"

#include "GraphicsTools.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AGTVisualProfiler::AGTVisualProfiler()
	: bSnapped(false)
	, ThresholdFrameTime(16.0f)
	, FrameTime(0.0f)
	, RenderThreadTime(0.0f)
	, GameThreadTime(0.0f)
	, GPUFrameTime{0.0f}
	, PrevFrameTime(0)
	, PrevRenderThreadTime(0)
	, PrevGameThreadTime(0)
	, PrevGPUFrameTime{0}
	, PrevNumDrawCalls(0)
	, PrevNumPrimitives(0)
{
	PrimaryActorTick.bCanEverTick = true;

	// TargetFrameTime = // TODO, get from platform.
	ThresholdFrameTime = (ThresholdFrameTime <= 0) ? 1 : ThresholdFrameTime;

	// Acquire default materials.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultQuadMeshFinder(TEXT("/Engine/BasicShapes/Plane"));
	check(DefaultQuadMeshFinder.Object);
	DefaultQuadMesh = DefaultQuadMeshFinder.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMaterialFinder(
		TEXT("Material'/Engine/EngineDebugMaterials/LevelColorationUnlitMaterial.LevelColorationUnlitMaterial'"));
	check(DefaultMaterialFinder.Object);
	DefaultMaterial = DefaultMaterialFinder.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultTextMaterialFinder(
		TEXT("Material'/Engine/EngineMaterials/UnlitText.UnlitText'"));
	check(DefaultTextMaterialFinder.Object);
	DefaultTextMaterial = DefaultTextMaterialFinder.Object;

	// UI constants.
	static const float PrefixYOffset = -3.9f;
	static const float LabelYOffset = -2.4f;
	static const float PivotYOffset = 0;
	static const float HeightZOffset = 0.45f;
	static const FVector BarPivot(0, 1, 0);
	static const FVector BarSize(0.004f, 0.02f, 1);
	static const FRotator QuadRotation(90, 0, 0);
	static const FString ZeroMs(TEXT("0.00 ms"));
	static const FLinearColor BackPlateColor(FColor(80, 80, 80, 255));     // Dark Gray
	static const FLinearColor FrameTimeColor(FColor(0, 164, 239, 255));    // Vivid Cerulean
	static const FLinearColor GameThreadColor(FColor(255, 185, 0, 255));   // Selective Yellow
	static const FLinearColor RenderThreadColor(FColor(242, 80, 34, 255)); // Orioles Orange
	static const FLinearColor GPUTimeColor(FColor(127, 186, 0, 255));      // Apple Green

	// Build the profiler by creating child components.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VisualProfiler"));

	FVector Location(-0.02f, 0, 0.9f);

	CreateQuad(TEXT("BackPlate"), RootComponent, FVector::ZeroVector, FVector(0.025f, 0.08f, 1), BackPlateColor);

	Location.Y = PrefixYOffset;
	CreateText(TEXT("FrameTimeLabelPrefix"), RootComponent, Location, TEXT("Frame: "));
	Location.Y = LabelYOffset;
	FrameTimeLabel = CreateText(TEXT("FrameTimeLabel"), RootComponent, Location, ZeroMs);
	Location.Y = PivotYOffset;
	FrameTimePivot = CreateScene<USceneComponent>(TEXT("FrameTimePivot"), RootComponent, Location, QuadRotation);
	CreateQuad(TEXT("FrameTimePivotQuad"), FrameTimePivot, BarPivot, BarSize, FrameTimeColor, FRotator::ZeroRotator);

	Location.Z = Location.Z - HeightZOffset;

	Location.Y = PrefixYOffset;
	CreateText(TEXT("GameThreadTimeLabelPrefix"), RootComponent, Location, TEXT("Game: "));
	Location.Y = LabelYOffset;
	GameThreadTimeLabel = CreateText(TEXT("GameThreadTimeLabel"), RootComponent, Location, ZeroMs);
	Location.Y = PivotYOffset;
	GameThreadTimePivot = CreateScene<USceneComponent>(TEXT("GameThreadTimePivot"), RootComponent, Location, QuadRotation);
	CreateQuad(TEXT("GameTimePivotQuad"), GameThreadTimePivot, BarPivot, BarSize, GameThreadColor, FRotator::ZeroRotator);

	Location.Z = Location.Z - (HeightZOffset * 0.5f);

	CreateQuad(TEXT("TargetLine"), RootComponent, FVector(Location.X * 2, BarPivot.Y * 2, Location.Z), FVector(0.018f, 0.0005f, 1));

	Location.Z = Location.Z - (HeightZOffset * 0.5f);

	Location.Y = PrefixYOffset;
	CreateText(TEXT("RenderThreadTimeLabelPrefix"), RootComponent, Location, TEXT("Draw: "));
	Location.Y = LabelYOffset;
	RenderThreadTimeLabel = CreateText(TEXT("RenderThreadTimeLabel"), RootComponent, Location, ZeroMs);
	Location.Y = PivotYOffset;
	RenderThreadTimePivot = CreateScene<USceneComponent>(TEXT("RenderThreadTimePivot"), RootComponent, Location, QuadRotation);
	CreateQuad(TEXT("RenderThreadTimePivotQuad"), RenderThreadTimePivot, BarPivot, BarSize, RenderThreadColor, FRotator::ZeroRotator);

	Location.Z = Location.Z - HeightZOffset;

	Location.Y = PrefixYOffset;
	CreateText(TEXT("GPUTimeLabelPrefix"), RootComponent, Location, TEXT("GPU: "));
	Location.Y = LabelYOffset;
	GPUTimeLabel = CreateText(TEXT("GPUTimeLabel"), RootComponent, Location, ZeroMs);
	Location.Y = PivotYOffset;
	GPUTimePivot = CreateScene<USceneComponent>(TEXT("GPUTimePivot"), RootComponent, Location, QuadRotation);
	CreateQuad(TEXT("GPUTimePivotQuad"), GPUTimePivot, BarPivot, BarSize, GPUTimeColor, FRotator::ZeroRotator);

	Location.Z = Location.Z - HeightZOffset;

	Location.Y = PrefixYOffset;
	DrawCallsLabel = CreateText(TEXT("DrawCallsLabel"), RootComponent, Location, TEXT("Draw Calls: 0"));
	Location.Y = 0;
	PrimitivesLabel = CreateText(TEXT("PrimitivesLabel"), RootComponent, Location, TEXT("Polys: 0"));
}

void AGTVisualProfiler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

		float RawGPUFrameTime[MAX_NUM_GPUS];

		for (uint32 GPUIndex : FRHIGPUMask::All())
		{
			// Number of milliseconds the GPU was busy last frame.
			const uint32 GPUCycles = RHIGetGPUFrameCycles(GPUIndex);
			RawGPUFrameTime[GPUIndex] = FPlatformTime::ToMilliseconds(GPUCycles);
			GPUFrameTime[GPUIndex] = 0.9 * GPUFrameTime[GPUIndex] + 0.1 * RawGPUFrameTime[GPUIndex];
		}

		// Apply the timings to the UI.
		if (CheckTimeDirty(FrameTime, PrevFrameTime))
		{
			FrameTimeLabel->SetText(FText::AsCultureInvariant(FString::Printf(TEXT("%3.2f ms"), FrameTime)));
			FrameTimeLabel->SetTextRenderColor(TimeToTextColor(FrameTime));
			FrameTimePivot->SetRelativeScale3D(FVector(1, TimeToScale(FrameTime), 1));
		}

		if (CheckTimeDirty(GameThreadTime, PrevGameThreadTime))
		{
			GameThreadTimeLabel->SetText(FText::AsCultureInvariant(FString::Printf(TEXT("%3.2f ms"), GameThreadTime)));
			GameThreadTimeLabel->SetTextRenderColor(TimeToTextColor(GameThreadTime));
			GameThreadTimePivot->SetRelativeScale3D(FVector(1, TimeToScale(GameThreadTime), 1));
		}

		if (CheckTimeDirty(RenderThreadTime, PrevRenderThreadTime))
		{
			RenderThreadTimeLabel->SetText(FText::AsCultureInvariant(FString::Printf(TEXT("%3.2f ms"), RenderThreadTime)));
			RenderThreadTimeLabel->SetTextRenderColor(TimeToTextColor(RenderThreadTime));
			RenderThreadTimePivot->SetRelativeScale3D(FVector(1, TimeToScale(RenderThreadTime), 1));
		}

		if (CheckTimeDirty(GPUFrameTime[0], PrevGPUFrameTime[0]))
		{
			GPUTimeLabel->SetText(FText::AsCultureInvariant(FString::Printf(TEXT("%3.2f ms"), GPUFrameTime[0])));
			GPUTimeLabel->SetTextRenderColor(TimeToTextColor(GPUFrameTime[0]));
			GPUTimePivot->SetRelativeScale3D(FVector(1, TimeToScale(GPUFrameTime[0]), 1));
		}
	}

	// Draw calls.
	{
		static const int32 ProfilerDrawCalls = 16; // Removed profiling induced draw calls.
		const int32 NumDrawCalls = FMath::Max(GNumDrawCallsRHI - ProfilerDrawCalls, 0);

		if (CheckCountDirty(NumDrawCalls, PrevNumDrawCalls))
		{
			DrawCallsLabel->SetText(FText::Format(FText::AsCultureInvariant("Draw Calls: {0}"), FText::AsNumber(NumDrawCalls)));
		}
	}

	// Primitives.
	{
		static const int32 ProfilerPrimitives = 336; // Removed profiling induced primitives.
		int32 NumPrimitives = FMath::Max(GNumPrimitivesDrawnRHI - ProfilerPrimitives, 0);

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

UStaticMeshComponent* AGTVisualProfiler::CreateQuad(
	const FName& Name, USceneComponent* Parent, FVector RelativeLocation, FVector RelativeScale, FLinearColor Color,
	FRotator RelativeRotation)
{
	UStaticMeshComponent* Component = CreateScene<UStaticMeshComponent>(Name, Parent, RelativeLocation, RelativeRotation);
	Component->SetRelativeScale3D(RelativeScale);
	Component->SetStaticMesh(DefaultQuadMesh);
	UMaterialInstanceDynamic* Material = Component->CreateDynamicMaterialInstance(0, DefaultMaterial);
	Material->SetVectorParameterValue(TEXT("Color"), Color);
	Component->SetMaterial(0, Material);
	Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	return Component;
}

UTextRenderComponent* AGTVisualProfiler::CreateText(
	const FName& Name, USceneComponent* Parent, FVector RelativeLocation, const FString& Text, FRotator RelativeRotation, float Size,
	bool LeftAlign)
{
	UTextRenderComponent* Component = CreateScene<UTextRenderComponent>(Name, Parent, RelativeLocation, RelativeRotation);
	Component->SetMaterial(0, DefaultTextMaterial);
	Component->SetWorldSize(Size);
	Component->SetHorizontalAlignment(LeftAlign ? EHorizTextAligment::EHTA_Left : EHorizTextAligment::EHTA_Right);
	Component->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Component->SetText(FText::AsCultureInvariant(Text));

	return Component;
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

float AGTVisualProfiler::TimeToScale(float Time) const
{
	return FMath::Clamp(Time / ThresholdFrameTime, 0.0f, 2.0f);
}

FColor AGTVisualProfiler::TimeToTextColor(float Time) const
{
	static const FColor MissedFrameColor(255, 20, 5, 255); // Orange
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
