#pragma once
#include "Asset.h"

#include "RenderApi.h"
#include "Shader.refl.h"

REFLECT()
class SMaterial : public SAsset {
	REFLECT_BODY()

public:
		SMaterial(const String& VertexShader, const String& FragmentShader);

		inline const SShaderHandle& GetHandle() const { return ShaderHandle; }

private:

	SShaderHandle ShaderHandle;
};
