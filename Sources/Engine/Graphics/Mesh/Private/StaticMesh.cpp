#include "StaticMesh.h"

SStaticMesh::SStaticMesh(const SMeshData& InMeshData) 
	: IMesh(InMeshData, SRendererApi::ExecuteFunction<SMeshHandle, const SMeshData*>("CreateMesh", &InMeshData)) {}

