#pragma once
#include "AssetFactory.h"
#include "Types/String.h"

class STexture2D;

class STextureFactory : public SAssetFactories {

	virtual void PostLoadData(SAsset* InAsset);


	void CreateFromData(const String& AssetPath, const String& SourcePath);




};