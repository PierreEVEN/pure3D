#pragma once

#include "Types/Transform.h"
#include "Renderer.h"
#include "Mesh.refl.h"


struct IMesh {};


struct MeshData {
	// Vertices tangent uv etc...
	int UBoVboEbo;
	// Material
};


REFLECT()
struct MeshProxy : public IPrimitiveProxy {

	REFLECT_BODY()

	STransform Transform;
	MeshData Data;
};
