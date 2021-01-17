#pragma once

#include "Types/Transform.h"
#include "Renderer.h"
#include "Asset.h"
#include "Types/Color.h"

#include "RenderApi.h"
#include "Shader.h"

#include "PrimitiveProxy.h"
#include "Mesh.refl.h"

struct SVertex {
	SVertex(const SVector& inPosition) : Position(inPosition) {}
	SVertex(const SVector& inPosition, SVector2D inUVS) : Position(inPosition), Uv(inUVS) {}
	SVector Position;
	SVector2D Uv;
	SVector Normal;
	SLinearColor Color;
	SVector Tangent;
};

struct SMeshSectionData {
	struct Lod {
		/**
		 * Lod material
		 */
		SMaterial* Material;
		
		/**
		 * Lod mesh
		 */
		std::vector<SVertex> Mesh;

		/**
		 * Lod triangles
		 */
		std::vector<uint32_t> Triangles;
	};
	
	/**
	 * Section transform
	 */
	SMatrix4Double Transformation;
	
	/**
	 * Lods data
	 */
	std::vector<Lod> Lods;
};

struct SMeshHandle {};

class SMeshRenderHelper : public IRendererHelper {
public:
	virtual std::shared_ptr<SMeshHandle> CreateMesh() = 0;
	virtual void DrawMesh(const SMeshHandle& Handle) = 0;
};

/**
 * Contains everything needed to draw a mesh section on screen
 */
REFLECT()
struct SMeshProxy : public IPrimitiveProxy {

	REFLECT_BODY();

public:
	
	struct Lod
	{
		/**
		 * Lod mesh
		 */
		std::shared_ptr<SMeshHandle> MeshHandle;

		/**
		 * Lod material
		 */
		std::shared_ptr<SShaderHandle> MaterialHandle;		
	};

	/**
	 * Section transform
	 */
	SMatrix4Double Transformation;
	
	/**
	 * Section Lods
	 */
	std::vector<Lod> Lods;

	size_t GetLOD() { return 0; }

	void Render(SRenderer* Context) override;
};

/**
 * Mesh base structure
 */
REFLECT()
class IMesh : public SAsset {

	REFLECT_BODY();

public:

	[[nodiscard]] const std::vector<SMeshProxy>& GetProxies() const { return LodProxies; }

protected:
	/**
	 * Mesh handles
	 */
	std::vector<SMeshProxy> LodProxies;	
};