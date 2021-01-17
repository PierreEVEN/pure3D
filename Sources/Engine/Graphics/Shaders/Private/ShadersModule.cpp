
#define STB_IMAGE_IMPLEMENTATION
#include "AssetFactory.h"
#include "stb_image.h"
#include "Texture.h"
#include "IO/Log.h"

#include "ModuleManager.h"
#include "Shader.h"
#include "ShaderFactory.h"

MODULE_CONSTRUCTOR()
{

	SAssetFactory::RegisterAssetFactory<SShaderFactory, SMaterial>();

	
}

