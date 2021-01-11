#include "Shader.h"
#include "RenderApi.h"
//#include <GL/gl3w.h>

SMaterial::SMaterial(const String& VertexShader, const String& FragmentShader) :
	ShaderHandle(SRendererApi::ExecuteFunction<SShaderHandle, const String&, const String&>("CompileShader", VertexShader, FragmentShader)) {

}

