

#include "MeshFactory.h"

#include "StaticMesh.h"

SAsset* SStaticMeshFactory::CreateFromData_Internal(const std::vector<SMeshData::SVertice>& Vertices,
	const std::vector<uint32_t>& Triangles, const std::vector<SMaterial*>& Materials)
{
	SStaticMesh* Mesh = new SStaticMesh();

	return Mesh;
}