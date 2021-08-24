// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTMeshOutlineComponentDetails.h"

#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "GTMeshOutlineComponent.h"
#include "GraphicsToolsEditor.h"
#include "IAssetTools.h"
#include "IDetailsView.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshConversion.h"
#include "StaticMeshAttributes.h"

#include "Application/SlateWindowHelper.h"
#include "Dialogs/DlgPickAssetPath.h"
#include "Engine/StaticMesh.h"
#include "Misc/PackageName.h"
#include "Modules/ModuleManager.h"
#include "PhysicsEngine/BodySetup.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/Text/STextBlock.h"

TSharedRef<IDetailCustomization> FGTMeshOutlineComponentDetails::MakeInstance()
{
	return MakeShareable(new FGTMeshOutlineComponentDetails);
}

void FGTMeshOutlineComponentDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& ProcMeshCategory = DetailBuilder.EditCategory("Mesh Outline");

	const FText ConvertToStaticMeshText = FText::AsCultureInvariant("Create Outline Static Mesh");

	// Cache the selected objects.
	SelectedObjectsList = DetailBuilder.GetSelectedObjects();

	ProcMeshCategory.AddCustomRow(ConvertToStaticMeshText, false)
		.NameContent()[SNullWidget::NullWidget]
		.ValueContent()
		.VAlign(VAlign_Center)
		.MaxDesiredWidth(250)[SNew(SButton)
								  .VAlign(VAlign_Center)
								  .ToolTipText(FText::AsCultureInvariant("Create a new StaticMesh asset based on the current StaticMesh "
																		 "modified to work with mesh outlines. Does not modify instance."))
								  .OnClicked(this, &FGTMeshOutlineComponentDetails::ClickedOnConvertToStaticMesh)
								  .IsEnabled(this, &FGTMeshOutlineComponentDetails::ConvertToStaticMeshEnabled)
								  .Content()[SNew(STextBlock).Text(ConvertToStaticMeshText)]];
}

UGTMeshOutlineComponent* FGTMeshOutlineComponentDetails::GetFirstSelectedMeshOutlineComponent() const
{
	UGTMeshOutlineComponent* OutlineMeshComponent = nullptr;

	for (const TWeakObjectPtr<UObject>& Object : SelectedObjectsList)
	{
		UGTMeshOutlineComponent* TestOutlineMeshComponent = Cast<UGTMeshOutlineComponent>(Object.Get());

		if (TestOutlineMeshComponent != nullptr && !TestOutlineMeshComponent->IsTemplate() &&
			TestOutlineMeshComponent->GetStaticMesh() != nullptr)
		{
			OutlineMeshComponent = TestOutlineMeshComponent;
			break;
		}
	}

	return OutlineMeshComponent;
}

bool FGTMeshOutlineComponentDetails::ConvertToStaticMeshEnabled() const
{
	return GetFirstSelectedMeshOutlineComponent() != nullptr;
}

typedef TPair<int32, FVector> IndexVertex;

void SmoothNormals(const TArray<FVector>& Vertices, TArray<FVector>& Normals, TArray<FProcMeshTangent>& Tangents)
{
	TMap<FVector, TArray<IndexVertex>> GroupedVerticies;
	GroupedVerticies.Reserve(Vertices.Num()); // Reserve for the worse case, each vertex is at a unique location.

	// Group all vertices that share the same location in space.
	{
		const int32 Count = Vertices.Num();
		for (int32 Index = 0; Index < Count; ++Index)
		{
			TArray<IndexVertex>& SmoothingGroup = GroupedVerticies.FindOrAdd(Vertices[Index]);
			SmoothingGroup.Add(IndexVertex(Index, Vertices[Index]));
		}
	}

	// If we didn't hit the degenerate case of each vertex is its own group (no vertices shared a location), average the normals/tangents of
	// each group.
	if (GroupedVerticies.Num() != Vertices.Num())
	{
		TArray<FVector> SmoothNormals(Normals);
		TArray<FProcMeshTangent> SmoothTangents(Tangents);

		for (auto& Pair : GroupedVerticies)
		{
			TArray<IndexVertex>& SmoothingGroup = Pair.Value;

			// No need to smooth a group of one.
			if (SmoothingGroup.Num() != 1)
			{
				// Average the normals and tangents.
				FVector SmoothedNormal = FVector::ZeroVector;
				FVector SmoothedTangent = FVector::ZeroVector;

				for (auto& Vertex : SmoothingGroup)
				{
					SmoothedNormal += Normals[Vertex.Key];
					SmoothedTangent += Tangents[Vertex.Key].TangentX;
				}

				SmoothedNormal.Normalize();
				SmoothedTangent.Normalize();

				for (auto& Vertex : SmoothingGroup)
				{
					SmoothNormals[Vertex.Key] = SmoothedNormal;
					SmoothTangents[Vertex.Key] = FProcMeshTangent(SmoothedTangent, false);
				}
			}
		}

		Normals = SmoothNormals;
		Tangents = SmoothTangents;
	}
}

bool BuildOutlineMesh(UProceduralMeshComponent* ProceduralMesh, UGTMeshOutlineComponent* OutlineMesh)
{
	FStaticMeshRenderData* RenderData = OutlineMesh->GetStaticMesh()->GetRenderData();

	if (RenderData == nullptr)
	{
		UE_LOG(GraphicsToolsEditor, Warning, TEXT("Failed to create the outline mesh becasue the source mesh has no render data."));

		return false;
	}

	if (RenderData->LODResources.Num() == 0)
	{
		UE_LOG(
			GraphicsToolsEditor, Warning, TEXT("Failed to build the outline mesh becasue the source mesh has nothing in the LOD chain."));

		return false;
	}

	FStaticMeshLODResources& LOD0 = RenderData->LODResources[0];

	// Positions.
	TArray<FVector> Vertices;
	{
		const int32 Count = LOD0.VertexBuffers.PositionVertexBuffer.GetNumVertices();

		Vertices.Reserve(Count);

		for (int32 Index = 0; Index < Count; ++Index)
		{
			Vertices.Add(LOD0.VertexBuffers.PositionVertexBuffer.VertexPosition(Index));
		}
	}

	// Triangles (flip to emulate front face culling).
	TArray<int32> Triangles;
	{
		const int32 Count = LOD0.IndexBuffer.GetNumIndices();

		Triangles.Reserve(Count);

		for (int32 Index = 0; Index < Count; Index += 3)
		{
			int32 A = LOD0.IndexBuffer.GetIndex(Index + 0);
			int32 B = LOD0.IndexBuffer.GetIndex(Index + 1);
			int32 C = LOD0.IndexBuffer.GetIndex(Index + 2);
			Triangles.Add(C);
			Triangles.Add(B);
			Triangles.Add(A);
		}
	}

	// Normals, UV0, and tangents.
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;
	{
		const int32 Count = LOD0.VertexBuffers.StaticMeshVertexBuffer.GetNumVertices();

		Normals.Reserve(Count);
		UV0.Reserve(Count);
		Tangents.Reserve(Count);

		for (int32 Index = 0; Index < Count; ++Index)
		{
			Normals.Add(LOD0.VertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(Index));
			UV0.Add(LOD0.VertexBuffers.StaticMeshVertexBuffer.GetVertexUV(Index, 0));
			Tangents.Add(FProcMeshTangent(LOD0.VertexBuffers.StaticMeshVertexBuffer.VertexTangentX(Index), false));
		}
	}

	// Vertex colors.
	TArray<FColor> VertexColors;
	{
		const int32 Count = LOD0.VertexBuffers.ColorVertexBuffer.GetNumVertices();

		VertexColors.Reserve(Count);

		for (int32 Index = 0; Index < Count; ++Index)
		{
			VertexColors.Add(LOD0.VertexBuffers.ColorVertexBuffer.VertexColor(Index));
		}
	}

	if (OutlineMesh->GetComputeSmoothNormals())
	{
		SmoothNormals(Vertices, Normals, Tangents);
	}

	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, false);

	return true;
}

FReply FGTMeshOutlineComponentDetails::ClickedOnConvertToStaticMesh()
{
	UGTMeshOutlineComponent* MeshOutlineComponent = GetFirstSelectedMeshOutlineComponent();

	if (MeshOutlineComponent != nullptr)
	{
		FString NewNameSuggestion = MeshOutlineComponent->GetStaticMesh()->GetName() + FString(TEXT("Outline"));
		FString PackageName = FString(TEXT("/Game/Meshes/")) + NewNameSuggestion;

		FString Name;
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		AssetToolsModule.Get().CreateUniqueAssetName(PackageName, TEXT(""), PackageName, Name);

		TSharedPtr<SDlgPickAssetPath> PickAssetPathWidget = SNew(SDlgPickAssetPath)
																.Title(FText::AsCultureInvariant("Choose a New Outline Mesh Location"))
																.DefaultAssetPath(FText::FromString(PackageName));

		if (PickAssetPathWidget->ShowModal() == EAppReturnType::Ok)
		{
			// Get the full name of where we want to create the physics asset.
			FString UserPackageName = PickAssetPathWidget->GetFullAssetPath().ToString();
			FName MeshName(*FPackageName::GetLongPackageAssetName(UserPackageName));

			// Check if the user inputed a valid asset name, if they did not, give it the generated default name
			if (MeshName == NAME_None)
			{
				// Use the defaults that were already generated.
				UserPackageName = PackageName;
				MeshName = *Name;
			}

			// Create a temporary ProceduralMeshComponent to populate.
			UProceduralMeshComponent* ProceduralMeshComponent =
				NewObject<UProceduralMeshComponent>(MeshOutlineComponent->GetOwner(), NAME_None, RF_Transactional | RF_TextExportTransient);
			ProceduralMeshComponent->SetupAttachment(MeshOutlineComponent);
			ProceduralMeshComponent->bIsEditorOnly = true;

			if (BuildOutlineMesh(ProceduralMeshComponent, MeshOutlineComponent))
			{
				FMeshDescription MeshDescription = BuildMeshDescription(ProceduralMeshComponent);

				if (MeshDescription.Polygons().Num() > 0)
				{
					UPackage* Package = CreatePackage(*UserPackageName);
					check(Package);

					// Create StaticMesh object.
					UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, MeshName, RF_Public | RF_Standalone);
					StaticMesh->InitResources();

					StaticMesh->SetLightingGuid(FGuid::NewGuid());

					// Add source to new StaticMesh.
					FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
					SrcModel.BuildSettings.bRecomputeNormals = false;
					SrcModel.BuildSettings.bRecomputeTangents = false;
					SrcModel.BuildSettings.bRemoveDegenerates = false;
					SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
					SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
					SrcModel.BuildSettings.bGenerateLightmapUVs = true;
					SrcModel.BuildSettings.SrcLightmapIndex = 0;
					SrcModel.BuildSettings.DstLightmapIndex = 1;
					StaticMesh->CreateMeshDescription(0, MoveTemp(MeshDescription));
					StaticMesh->CommitMeshDescription(0);

					// Collision.
					if (!ProceduralMeshComponent->bUseComplexAsSimpleCollision)
					{
						StaticMesh->CreateBodySetup();
						UBodySetup* NewBodySetup = StaticMesh->GetBodySetup();
						NewBodySetup->BodySetupGuid = FGuid::NewGuid();
						NewBodySetup->AggGeom.ConvexElems = ProceduralMeshComponent->ProcMeshBodySetup->AggGeom.ConvexElems;
						NewBodySetup->bGenerateMirroredCollision = false;
						NewBodySetup->bDoubleSidedGeometry = true;
						NewBodySetup->CollisionTraceFlag = CTF_UseDefault;
						NewBodySetup->CreatePhysicsMeshes();
					}

					// Materials.
					TSet<UMaterialInterface*> UniqueMaterials;
					const int32 NumSections = ProceduralMeshComponent->GetNumSections();

					for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
					{
						FProcMeshSection* ProcSection = ProceduralMeshComponent->GetProcMeshSection(SectionIdx);
						UMaterialInterface* Material = ProceduralMeshComponent->GetMaterial(SectionIdx);
						UniqueMaterials.Add(Material);
					}

					// Copy materials to new mesh.
					for (auto* Material : UniqueMaterials)
					{
						StaticMesh->GetStaticMaterials().Add(FStaticMaterial(Material));
					}

					// Set the imported version before calling the build.
					StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;

					// Build mesh from source.
					StaticMesh->Build(false);
					StaticMesh->PostEditChange();

					// Notify asset registry of new asset.
					FAssetRegistryModule::AssetCreated(StaticMesh);

					// Apply the static mesh to the mesh outline component.
					MeshOutlineComponent->SetStaticMesh(StaticMesh);
				}
				else
				{
					UE_LOG(
						GraphicsToolsEditor, Warning, TEXT("Failed to create the outline mesh becasue the source mesh has no polygons."));
				}
			}

			// Release the procedural mesh component since it is no longer required.
			ProceduralMeshComponent->DestroyComponent();
			ProceduralMeshComponent = nullptr;
		}
	}

	return FReply::Handled();
}
