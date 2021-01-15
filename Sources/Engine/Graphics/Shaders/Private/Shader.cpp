#include "Shader.h"
#include "RenderApi.h"

SMaterial::SMaterial(const String& VertexShader, const String& FragmentShader) :
	ShaderHandle(IRendererApi::Get()->CompileShader(VertexShader, FragmentShader)) {
}

