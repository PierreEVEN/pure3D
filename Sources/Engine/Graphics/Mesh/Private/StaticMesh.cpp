#include "StaticMesh.h"

SStaticMesh::SStaticMesh(const SMeshData& InMeshData) 
	: IMesh({ IMesh::SMeshSection(InMeshData, SRendererApi::Get()->CreateMesh(&InMeshData)) }) {}

