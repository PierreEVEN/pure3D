#pragma once
#include "Mesh.h"
#include "MeshFactory.h"

#include "StaticMesh.refl.h"

REFLECT()
class SStaticMesh : public IMesh {

	friend class SStaticMeshFactory;
	
	REFLECT_BODY()


public:

	SStaticMesh() = default;
	
	void Reload() override {}
	void PostLoad() override;

	RPROPERTY()
	std::vector<SMeshData::SVertice> Vertices;

	RPROPERTY()
	std::vector<uint32_t> Triangles;

	RPROPERTY()
	std::vector<SMaterial*> Materials;
};
