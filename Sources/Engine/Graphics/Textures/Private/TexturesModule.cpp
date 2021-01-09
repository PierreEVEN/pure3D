
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "TextureFactory.h"
#include "Texture.h"
#include "IO/Log.h"

void STextureFactory::CreateFromData(const String& AssetPath, const String& SourcePath)
{
	int SizeX, SizeY, Channels;
	stbi_uc* TextureData = stbi_load(SourcePath.GetData(), &SizeX, &SizeY, &Channels, 4);
	if (!TextureData)
		LOG_WARNING("Failed to load texture data from %s", SourcePath.GetData());

}

void STextureFactory::PostLoadData(SAsset* InAsset)
{

}
