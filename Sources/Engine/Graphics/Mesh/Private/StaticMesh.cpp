#include "StaticMesh.h"

SStaticMesh::SStaticMesh(const SMeshData& InMeshData) 
	: IMesh({ IMesh::SMeshSection(InMeshData, IRendererApi::Get()->CreateMesh(&InMeshData)) }) {}

