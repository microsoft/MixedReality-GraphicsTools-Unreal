// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTMeshOutlineComponent.h"

void UGTMeshOutlineComponent::PostLoad()
{
	Super::PostLoad();

	UpdateMesh();
	UpdateMaterial();
}

void UGTMeshOutlineComponent::SetStaticMesh(UStaticMesh* Mesh)
{
	if (StaticMesh != Mesh)
	{
		StaticMesh = Mesh;

		UpdateMesh();
		UpdateMaterial();
	}
}

void UGTMeshOutlineComponent::SetOutlineThickness(float Thickness)
{
	if (OutlineThickness != Thickness)
	{
		UpdateMaterial();
	}
}

void UGTMeshOutlineComponent::SetOutlineColor(FColor Color)
{
	if (OutlineColor != Color)
	{
		UpdateMaterial();
	}
}

void UGTMeshOutlineComponent::SetComputeSmoothNormals(bool Compute)
{
	if (bComputeSmoothNormals != Compute)
	{
		bComputeSmoothNormals = Compute;

		UpdateMesh();
	}
}

#if WITH_EDITOR
void UGTMeshOutlineComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTMeshOutlineComponent, StaticMesh))
	{
		UpdateMesh();
		UpdateMaterial();
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTMeshOutlineComponent, OutlineThickness))
	{
		UpdateMaterial();
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTMeshOutlineComponent, OutlineColor))
	{
		UpdateMaterial();
	}
	else if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGTMeshOutlineComponent, bComputeSmoothNormals))
	{
		UpdateMesh();
	}
}
#endif // WITH_EDITOR

void UGTMeshOutlineComponent::UpdateMesh()
{
	if (StaticMesh == nullptr || StaticMesh->RenderData == nullptr)
	{
		return;
	}

	if (StaticMesh->RenderData->LODResources.Num() == 0)
	{
		return;
	}

	FStaticMeshLODResources& LOD0 = StaticMesh->RenderData->LODResources[0];

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

	if (bComputeSmoothNormals)
	{
		SmoothNormals(Vertices, Normals, Tangents);
	}

	CreateMeshSection(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, false);
}

typedef TPair<int32, FVector> IndexVertex;

void UGTMeshOutlineComponent::SmoothNormals(const TArray<FVector>& Vertices, TArray<FVector>& Normals, TArray<FProcMeshTangent>& Tangents)
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

void UGTMeshOutlineComponent::UpdateMaterial()
{
	UMaterialInterface* Material = GetMaterial(0);

	if (Material == nullptr)
	{
		return;
	}

	UMaterialInstanceDynamic* MaterialInstance = CreateDynamicMaterialInstance(0, Material);

	static const FName OutlineThicknessName = "OutlineThickness";
	MaterialInstance->SetScalarParameterValue(OutlineThicknessName, OutlineThickness);

	static const FName OutlineColorName = "OutlineColor";
	MaterialInstance->SetVectorParameterValue(OutlineColorName, OutlineColor);
}
