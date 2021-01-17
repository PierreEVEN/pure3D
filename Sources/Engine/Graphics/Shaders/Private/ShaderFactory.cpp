
#include "ShaderFactory.h"

#include "Shader.h"

SAsset* SShaderFactory::CreateFromData_Internal(const String& VertexShader, const String& FragmentShader) {
	SMaterial* Material = new SMaterial();
	Material->VertexShaderData = VertexShader;
	Material->FragmentShaderData = FragmentShader;
	return Material;
}
