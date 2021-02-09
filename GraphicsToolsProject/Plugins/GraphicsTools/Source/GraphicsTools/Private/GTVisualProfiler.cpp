// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTVisualProfiler.h"

#include "GraphicsTools.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AGTVisualProfiler::AGTVisualProfiler()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane"));
	check(PlaneMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMaterial(
		TEXT("Material'/Engine/EngineDebugMaterials/LevelColorationUnlitMaterial.LevelColorationUnlitMaterial'"));
	check(DefaultMaterial.Object);

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultTextMaterial(TEXT("Material'/Engine/EngineMaterials/UnlitText.UnlitText'"));
	check(DefaultTextMaterial.Object);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VisualProfiler"));

	{
		BackPlate = CreateAndAttachComponent<UStaticMeshComponent>(TEXT("BackPlate"), RootComponent);
		BackPlate->SetRelativeRotation(FRotator(0, 90, 90));
		BackPlate->SetRelativeScale3D(FVector(0.1f, 0.02f, 1));
		BackPlate->SetStaticMesh(PlaneMesh.Object);
		UMaterialInstanceDynamic* Material = BackPlate->CreateDynamicMaterialInstance(0, DefaultMaterial.Object);
		Material->SetVectorParameterValue(TEXT("Color"), FLinearColor(20 / 256.0f, 20 / 256.0f, 20 / 256.0f, 1));
		BackPlate->SetMaterial(0, Material);
		BackPlate->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	{
		DrawCallsLabel = CreateAndAttachComponent<UTextRenderComponent>(TEXT("DrawCallsLabel"), RootComponent);
		DrawCallsLabel->SetRelativeLocation(FVector(-0.1f, 0, 0.25));
		DrawCallsLabel->SetRelativeRotation(FRotator(0, 180, 0));
		DrawCallsLabel->SetMaterial(0, DefaultTextMaterial.Object);
		DrawCallsLabel->SetWorldSize(0.5f);
		DrawCallsLabel->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		DrawCallsLabel->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
		DrawCallsLabel->SetText(FText::AsCultureInvariant("Draw Calls: 0"));
	}
	{
		PrimitivesLabel = CreateAndAttachComponent<UTextRenderComponent>(TEXT("PrimitivesLabel"), RootComponent);
		PrimitivesLabel->SetRelativeLocation(FVector(-0.1f, 0, -0.25));
		PrimitivesLabel->SetRelativeRotation(FRotator(0, 180, 0));
		PrimitivesLabel->SetMaterial(0, DefaultTextMaterial.Object);
		PrimitivesLabel->SetWorldSize(0.5f);
		PrimitivesLabel->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		PrimitivesLabel->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
		PrimitivesLabel->SetText(FText::AsCultureInvariant("Primitives: 0"));
	}
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
		DrawCallsLabel->SetText(FText::Format(FText::AsCultureInvariant("Draw Calls: {0}"), FText::AsNumber(NumDrawCalls)));
	}

	// Primitives
	{
		int32 NumPrimitives = GNumPrimitivesDrawnRHI;
		PrimitivesLabel->SetText(FText::Format(FText::AsCultureInvariant("Primitives: {0}"), FText::AsNumber(NumPrimitives)));
	}

	UpdateTransform(DeltaTime);
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
