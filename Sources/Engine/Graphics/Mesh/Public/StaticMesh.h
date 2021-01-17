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

private:
	
	std::vector<SMeshSectionData> SectionsData;
};
