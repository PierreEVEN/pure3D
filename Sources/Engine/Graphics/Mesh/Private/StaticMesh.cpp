#include "StaticMesh.h"

SStaticMesh::SStaticMesh(const SMeshData& InMeshData) 
	: IMesh(InMeshData, SRendererApi::Get()->CreateMesh(&InMeshData)) {}

