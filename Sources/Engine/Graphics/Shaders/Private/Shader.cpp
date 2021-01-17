#include "Shader.h"
#include "RenderApi.h"


void SMaterial::PostLoad() {	
	ShaderHandle = IRendererApi::Get()->CompileShader(VertexShaderData, FragmentShaderData);
}
