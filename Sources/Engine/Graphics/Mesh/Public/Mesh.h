#pragma once

#include "Types/Transform.h"
#include "Renderer.h"
#include "Asset.h"
#include "Types/Color.h"

#include "RenderApi.h"

#include "Mesh.refl.h"

struct SMeshData {
	struct SVertice {
		SVertice(const SVector& inPosition) : Position(inPosition) {}
		SVector Position;
		SVector2D UV;
		SVector Normal;
		SLinearColor Color;
		SVector Tangent;
	};

	std::vector<SVertice> Mesh;
	std::vector<uint32_t> Triangles;
};


REFLECT()
struct IMesh : public SAsset {

	IMesh(const SMeshData& InMeshData, SMeshHandle* inMeshHandle) : MeshData(InMeshData), MeshHandle(inMeshHandle) {}

	REFLECT_BODY()

public:

	inline SMeshHandle* GetHandle() const { return MeshHandle; }

private:

	SMeshData MeshData;
	SMeshHandle* MeshHandle;
};

REFLECT()
struct SMeshProxy : public IPrimitiveProxy {

	REFLECT_BODY()
public:

	STransform Transform;

	SMeshHandle* MeshHandle;
};
