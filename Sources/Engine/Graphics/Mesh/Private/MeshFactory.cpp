

#include "MeshFactory.h"

#include "StaticMesh.h"

SAsset* SStaticMeshFactory::CreateFromData_Internal(const std::vector<SMeshSectionData>& Sections)
{
	SStaticMesh* Mesh = new SStaticMesh();
	Mesh->SectionsData = Sections;
	return Mesh;
}