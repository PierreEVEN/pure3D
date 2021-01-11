#include "Shader.h"
#include "RenderApi.h"

SMaterial::SMaterial(const String& VertexShader, const String& FragmentShader) :
	ShaderHandle(SRendererApi::Get()->CompileShader(VertexShader, FragmentShader)) {
}

