#pragma once
#include "Mesh.h"

#include "StaticMesh.refl.h"

REFLECT()
struct SStaticMesh : public IMesh {

	SStaticMesh(const SMeshData& InMeshData);

	REFLECT_BODY()


	
};
