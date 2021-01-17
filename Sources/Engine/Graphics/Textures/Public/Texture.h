#pragma once

#include "Asset.h"
#include "Texture.refl.h"

class STextureFactory;

REFLECT()
class STexture2D : public SAsset {

	REFLECT_BODY()

public:

	friend STextureFactory;

protected:

	RCONSTRUCTOR()
		STexture2D();

	void PostLoad() override {}

	void Reload() override {}

private:

	RPROPERTY()
		ByteArray TextureData;

	RPROPERTY()
		uint8_t Channels;

	RPROPERTY()
		uint32_t SizeX;

	RPROPERTY()
		uint32_t SizeY;
};