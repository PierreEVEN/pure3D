#pragma once

#include "Asset.h"

#include "RenderApi.h"
#include "Shader.refl.h"

struct SShaderHandle
{
	
};

REFLECT()
class SMaterial : public SAsset {
	REFLECT_BODY()

		friend class SShaderFactory;
public:
	[[nodiscard]] const std::shared_ptr<SShaderHandle>& GetHandle() const { return ShaderHandle; }

protected:

	RCONSTRUCTOR()
		SMaterial() = default;

	void PostLoad() override {}

	void Reload() override {}

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
