// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTVisualProfiler.h"

#include "GraphicsTools.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

const int32 NumberOfSamples = 200;

AGTVisualProfiler::AGTVisualProfiler()
	: RenderThreadTime(0.0f)
	, GameThreadTime(0.0f)
	, GPUFrameTime{0.0f}
	, FrameTime(0.0f)
	, RawRenderThreadTime(0.0f)
	, RawGameThreadTime(0.0f)
	, RawGPUFrameTime{0.0f}
	, RawFrameTime(0.0f)
//, CurrentIndex(0)
{
	// RenderThreadTimes.AddZeroed(NumberOfSamples);
	// GameThreadTimes.AddZeroed(NumberOfSamples);
	// for (auto& GPUFrameTimesArray : GPUFrameTimes)
	//{
	//	GPUFrameTimesArray.AddZeroed(NumberOfSamples);
	//}
	// FrameTimes.AddZeroed(NumberOfSamples);

	PrimaryActorTick.bCanEverTick = true;

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

	// Create child components.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VisualProfiler"));

	BackPlate = CreateQuad(
		TEXT("BackPlate"), RootComponent, FVector::ZeroVector, FRotator(0, 90, 90), FVector(0.1f, 0.02f, 1),
		FLinearColor(20 / 256.0f, 20 / 256.0f, 20 / 256.0f, 1));

	const float DepthOffset = -0.02f;
	const FRotator TextRotation(0, 180, 0);
	const FString ZeroMs(TEXT("0.00 ms"));
	{
		FrameTimeLabelPrefix =
			CreateText(TEXT("FrameTimeLabelPrefix"), RootComponent, FVector(DepthOffset, -4.7f, 0.6f), TextRotation, TEXT("Frame: "));
		FrameTimeLabel = CreateText(TEXT("FrameTimeLabel"), RootComponent, FVector(DepthOffset, -3.3f, 0.6f), TextRotation, ZeroMs);

		GameThreadTimeLabelPrefix =
			CreateText(TEXT("GameThreadTimeLabelPrefix"), RootComponent, FVector(DepthOffset, -4.7f, 0.2f), TextRotation, TEXT("Game: "));
		GameThreadTimeLabel =
			CreateText(TEXT("GameThreadTimeLabel"), RootComponent, FVector(DepthOffset, -3.3f, 0.2f), TextRotation, ZeroMs);

		RenderThreadTimeLabelPrefix = CreateText(
			TEXT("RenderThreadTimeLabelPrefix"), RootComponent, FVector(DepthOffset, -4.7f, -0.2f), TextRotation, TEXT("Draw: "));
		RenderThreadTimeLabel =
			CreateText(TEXT("RenderThreadTimeLabel"), RootComponent, FVector(DepthOffset, -3.3f, -0.2f), TextRotation, ZeroMs);

		GPUTimeLabelPrefix =
			CreateText(TEXT("GPUTimeLabelPrefix"), RootComponent, FVector(DepthOffset, -4.7f, -0.6f), TextRotation, TEXT("GPU: "));
		GPUTimeLabel = CreateText(TEXT("GPUTimeLabel"), RootComponent, FVector(DepthOffset, -3.3f, -0.6f), TextRotation, ZeroMs);
	}

	DrawCallsLabel =
		CreateText(TEXT("DrawCallsLabel"), RootComponent, FVector(DepthOffset, 0, 0.25), FRotator(0, 180, 0), TEXT("Draw Calls: 0"));

	PrimitivesLabel =
		CreateText(TEXT("PrimitivesLabel"), RootComponent, FVector(DepthOffset, 0, -0.25), FRotator(0, 180, 0), TEXT("Primitives: 0"));
}

void AGTVisualProfiler::BeginPlay()
{
	Super::BeginPlay();
}

void AGTVisualProfiler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		float DiffTime = FApp::GetCurrentTime() - FApp::GetLastTime();

		RawFrameTime = DiffTime * 1000.0f;
		FrameTime = 0.9 * FrameTime + 0.1 * RawFrameTime;

		/** Number of milliseconds the gamethread was used last frame. */
		RawGameThreadTime = FPlatformTime::ToMilliseconds(GGameThreadTime);
		GameThreadTime = 0.9 * GameThreadTime + 0.1 * RawGameThreadTime;

		/** Number of milliseconds the renderthread was used last frame. */
		RawRenderThreadTime = FPlatformTime::ToMilliseconds(GRenderThreadTime);
		RenderThreadTime = 0.9 * RenderThreadTime + 0.1 * RawRenderThreadTime;

		for (uint32 GPUIndex : FRHIGPUMask::All())
		{
			/** Number of milliseconds the GPU was busy last frame. */
			const uint32 GPUCycles = RHIGetGPUFrameCycles(GPUIndex);
			RawGPUFrameTime[GPUIndex] = FPlatformTime::ToMilliseconds(GPUCycles);
			GPUFrameTime[GPUIndex] = 0.9 * GPUFrameTime[GPUIndex] + 0.1 * RawGPUFrameTime[GPUIndex];
		}

		/*RenderThreadTimes[CurrentIndex] = RenderThreadTime;
		GameThreadTimes[CurrentIndex] =GameThreadTime;
		for (uint32 GPUIndex : FRHIGPUMask::All())
		{
			GPUFrameTimes[GPUIndex][CurrentIndex] = GPUFrameTime[GPUIndex];
		}
		FrameTimes[CurrentIndex] = FrameTime;

		CurrentIndex++;

		if (CurrentIndex == NumberOfSamples)
		{
			CurrentIndex = 0;
		}*/
	}

	// Timings.
	{
		FrameTimeLabel->SetText(FText::AsCultureInvariant(FString::Printf(TEXT("%3.2f ms"), FrameTime)));
		GameThreadTimeLabel->SetText(FText::AsCultureInvariant(FString::Printf(TEXT("%3.2f ms"), GameThreadTime)));
		RenderThreadTimeLabel->SetText(FText::AsCultureInvariant(FString::Printf(TEXT("%3.2f ms"), RenderThreadTime)));
		GPUTimeLabel->SetText(FText::AsCultureInvariant(FString::Printf(TEXT("%3.2f ms"), GPUFrameTime[0])));
	}

	// Draw calls.
	{
		const int32 NumDrawCalls = GNumDrawCallsRHI;
		DrawCallsLabel->SetText(FText::Format(FText::AsCultureInvariant("Draw Calls: {0}"), FText::AsNumber(NumDrawCalls)));
	}

	// Primitives.
	{
		int32 NumPrimitives = GNumPrimitivesDrawnRHI;

		if (NumPrimitives < 10000)
		{
			PrimitivesLabel->SetText(FText::Format(FText::AsCultureInvariant("Primitives: {0}"), FText::AsNumber(NumPrimitives)));
		}
		else
		{
			// TODO
			//float NumPrimitivesK = NumPrimitives / 1000.f;
			//PrimitivesLabel->SetText(
			//	FText::Format(FText::AsCultureInvariant("Primitives: {0}"), *FString::Printf(TEXT("%.1fK"), NumPrimitivesK)));
		}

	}

	UpdateTransform(DeltaTime);
}

UStaticMeshComponent* AGTVisualProfiler::CreateQuad(
	const FName& Name, USceneComponent* Parent, FVector RelativeLocation, FRotator RelativeRotation, FVector RelativeScale,
	FLinearColor Color)
{
	UStaticMeshComponent* Component = CreateAndAttachComponent<UStaticMeshComponent>(Name, Parent, RelativeLocation, RelativeRotation);
	Component->SetRelativeScale3D(RelativeScale);
	Component->SetStaticMesh(DefaultQuadMesh);
	UMaterialInstanceDynamic* Material = Component->CreateDynamicMaterialInstance(0, DefaultMaterial);
	Material->SetVectorParameterValue(TEXT("Color"), Color);
	Component->SetMaterial(0, Material);
	Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	return Component;
}

UTextRenderComponent* AGTVisualProfiler::CreateText(
	const FName& Name, USceneComponent* Parent, FVector RelativeLocation, FRotator RelativeRotation, const FString& Text, float Size,
	bool LeftAlign)
{
	UTextRenderComponent* Component = CreateAndAttachComponent<UTextRenderComponent>(Name, Parent, RelativeLocation, RelativeRotation);
	Component->SetMaterial(0, DefaultTextMaterial);
	Component->SetWorldSize(Size);
	Component->SetHorizontalAlignment(LeftAlign ? EHorizTextAligment::EHTA_Left : EHorizTextAligment::EHTA_Right);
	Component->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Component->SetText(FText::AsCultureInvariant(Text));

	return Component;
}

void AGTVisualProfiler::UpdateTransform(float DeltaTime)
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager)
		{
			const FVector CameraLocation = CameraManager->GetCameraLocation();
			const FRotator CameraRotation = CameraManager->GetCameraRotation();
			const FVector CameraForward = CameraRotation.Vector();
			float fov = CameraManager->GetFOVAngle();
			const float Distance = FMath::Max((16.0f / CameraManager->GetFOVAngle()) * 100, 25.0f);

			FVector TargetLocation = CameraLocation + (CameraForward * Distance);
			TargetLocation += CameraRotation.RotateVector(FVector(0, 0, -1)) * 2.5; // TODO, make property.
			FQuat TargetRotation = CameraRotation.Quaternion();

			const float WindowFollowSpeed = 5.0f; // TODO, make property.
			const float t = DeltaTime * WindowFollowSpeed;
			SetActorLocationAndRotation(
				FMath::Lerp(GetActorLocation(), TargetLocation, t), FQuat::Slerp(GetActorQuat(), TargetRotation, t));
		}
	}
}
