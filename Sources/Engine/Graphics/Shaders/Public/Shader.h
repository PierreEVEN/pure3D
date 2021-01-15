#pragma once

#include "Asset.h"

#include "RenderApi.h"
#include "Shader.refl.h"









REFLECT()
class SMaterial : public SAsset {
	REFLECT_BODY()

public:
		SMaterial(const String& VertexShader, const String& FragmentShader);

		inline const std::shared_ptr<SShaderHandle>& GetHandle() const { return ShaderHandle; }

private:

	std::shared_ptr<SShaderHandle> ShaderHandle;

	RPROPERTY()
		String VertexShaderSourcePath;

	RPROPERTY()
		String FragmentShaderSourcePath;

	RPROPERTY()
		String VertexShaderData;

	RPROPERTY()
		String FragmentShaderData;
};
