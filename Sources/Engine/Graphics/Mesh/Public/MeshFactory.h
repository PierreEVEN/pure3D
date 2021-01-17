#pragma once
#include "AssetFactory.h"
#include "Mesh.h"
#include "MeshFactory.refl.h"

class SMaterial;

REFLECT()
class SStaticMeshFactory : public SAssetFactory
{
	REFLECT_BODY()


public:
		RFUNCTION()
		SAsset* CreateFromData_Internal(const std::vector<SMeshData::SVertice>& Vertices, const std::vector<uint32_t>& Triangles, const std::vector<SMaterial*>& Materials);
};

