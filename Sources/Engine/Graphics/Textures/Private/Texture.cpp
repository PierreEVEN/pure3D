#include "Texture.h"

STexture2D::STexture2D()
	: SizeX(0), SizeY(0), Channels(0) {
	TextureData.SetData(nullptr, 0);
}
