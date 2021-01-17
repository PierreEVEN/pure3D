#pragma once
#include "AssetFactory.h"

#include "ShaderFactory.refl.h"

REFLECT()
class SShaderFactory : public SAssetFactory
{
	REFLECT_BODY()

		RFUNCTION()
		SAsset* CreateFromData_Internal(const String& VertexShader, const String& FragmentShader);
};
