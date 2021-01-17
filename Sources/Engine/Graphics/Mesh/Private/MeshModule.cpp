#include "AssetFactory.h"
#include "MeshFactory.h"
#include "ModuleManager.h"
#include "StaticMesh.h"

MODULE_CONSTRUCTOR() {

	SAssetFactory::RegisterAssetFactory<SStaticMeshFactory, SStaticMesh>();


}