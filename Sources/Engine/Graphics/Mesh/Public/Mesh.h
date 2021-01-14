#pragma once

#include "Types/Transform.h"
#include "Renderer.h"
#include "Asset.h"
#include "Types/Color.h"

#include "RenderApi.h"
#include "Shader.h"

#include "PrimitiveProxy.h"
#include "Mesh.refl.h"

struct SMeshData {
	struct SVertice {
		SVertice(const SVector& inPosition) : Position(inPosition) {}
		SVertice(const SVector& inPosition, SVector2D inUVS) : Position(inPosition), UV(inUVS) {}
		SVector Position;
		SVector2D UV;
		SVector Normal;
		SLinearColor Color;
		SVector Tangent;
	};

	SMaterial* Material;
	std::vector<SVertice> Mesh;
	std::vector<uint32_t> Triangles;
};


REFLECT()
struct IMesh : public SAsset {

	REFLECT_BODY()

public:

	struct SMeshSection {
		SMeshSection(const SMeshData& inMeshData, const std::shared_ptr<SMeshHandle>& inMeshHandle) : MeshData(inMeshData), MeshHandle(inMeshHandle) {}
		SMeshData MeshData;
		std::shared_ptr<SMeshHandle> MeshHandle;
	};

	IMesh(std::vector<SMeshSection> InSections) : Sections(InSections) {}

	inline const std::vector<SMeshSection>& GetSections() const { return Sections; }

private:
	std::vector<SMeshSection> Sections;
};

REFLECT()
struct SMeshProxy : public IPrimitiveProxy {

	REFLECT_BODY()
public:

	STransform Transform;

	std::shared_ptr<SShaderHandle> MaterialHandle;
	std::shared_ptr<SMeshHandle> MeshHandle;
};
