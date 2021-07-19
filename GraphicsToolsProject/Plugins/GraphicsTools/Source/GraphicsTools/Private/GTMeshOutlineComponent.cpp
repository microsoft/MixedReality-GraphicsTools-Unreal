// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "GTMeshOutlineComponent.h"

void UGTMeshOutlineComponent::PostLoad()
{
	Super::PostLoad();

	UpdateMesh();
}

void UGTMeshOutlineComponent::SetStaticMesh(UStaticMesh* Mesh)
{
	if (StaticMesh != Mesh)
	{
		StaticMesh = Mesh;

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

	// Smooth the normals.
	// TODO

	CreateMeshSection(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, false);
}
