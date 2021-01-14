#pragma once

#include "Renderer.h"

#include <GL/gl3w.h>
#include "Types/String.h"


#define GL_CHECK_ERROR() SOpenGLRenderer::CheckGLError(__LINE__, __FILE__)

struct SOpenGLRenderer : public SRenderer {
public:
	SOpenGLRenderer();

	static void CheckGLError(int64_t Line, const String& File);

protected:
	virtual void BeginFrame();

	virtual void CopyUniformBufferData();

	virtual SVector2D GetFramebufferSize();

private:

	void GenerateUniformBuffer();

	bool bIsRendererUniformBufferValid;
	GLuint RendererUniformBuffer;
};