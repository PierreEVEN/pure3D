#pragma once

#include "Renderer.h"

#include <GL/gl3w.h>
#include "Types/String.h"

struct GLUniformBufferData {

	float DeltaTime;

};

#define GL_CHECK_ERROR() SOpenGLRenderer::CheckGLError(__LINE__, __FILE__)

struct SOpenGLRenderer : public SRenderer {
public:
	SOpenGLRenderer();

	static void CheckGLError(int64_t Line, const String& File);

protected:
	virtual void BeginFrame();

	virtual void UpdateUniformBuffers();

private:

	void GenerateUniformBuffer();

	bool bIsRendererUniformBufferValid;
	GLuint RendererUniformBuffer;
	GLUniformBufferData UniformBufferData;
};