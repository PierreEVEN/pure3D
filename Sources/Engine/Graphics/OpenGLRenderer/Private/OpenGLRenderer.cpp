#include "OpenGLRenderer.h"
#include <GL/gl3w.h>
#include "Mesh.h"
#include "IO/Log.h"
#include "RenderPass.h"
#include "GLFW/glfw3.h"
#include "OpenGLRenderApi.h"

void SOpenGLRenderer::CheckGLError(int64_t Line, const String& File) {
	int errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR)
	{
		String errorCode;
		switch (errCode) {
		case GL_INVALID_ENUM: errorCode = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE: errorCode = "GL_INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: errorCode = "GL_INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW: errorCode = "GL_STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW: errorCode = "GL_STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY: errorCode = "GL_OUT_OF_MEMORY"; break;
		case 0x506: errorCode = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
		case 0x0507: errorCode = "GL_CONTEXT_LOST"; break;
		case 0x8031: errorCode = "GL_TABLE_TOO_LARGE1"; break;
		default: errorCode = "unknown : " + errCode; break;
		}
		LOG_ASSERT("GL error : " + errorCode + " -> " + File + ":" + String(Line));
	}
}

SOpenGLRenderer::SOpenGLRenderer() : bIsRendererUniformBufferValid (false) {
	AddRenderPass(new SRenderPass((uint32_t)ERenderPass::ERenderPass_COLOR));
}

void SOpenGLRenderer::BeginFrame() {
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_CHECK_ERROR();
}

void SOpenGLRenderer::CopyUniformBufferData()
{
	GenerateUniformBuffer();
	glBindBuffer(GL_UNIFORM_BUFFER, RendererUniformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SRenderer::UniformBufferData), &SRenderer::GetUniformBuffer());
	GL_CHECK_ERROR();
}


SVector2D SOpenGLRenderer::GetFramebufferSize() {
	int SizeX, SizeY;
	glfwGetFramebufferSize(((SOpenGlRenderApi*)IRendererApi::Get())->WindowHandle, &SizeX, &SizeY);
	return SVector2D((float)SizeX, (float)SizeY);
}

void SOpenGLRenderer::GenerateUniformBuffer() {
	if (bIsRendererUniformBufferValid) return;
	bIsRendererUniformBufferValid = true;
	glGenBuffers(1, &RendererUniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, RendererUniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SRenderer::UniformBufferData), &SRenderer::GetUniformBuffer(), GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, RendererUniformBuffer, 0, sizeof(SRenderer::UniformBufferData));
	GL_CHECK_ERROR();
}
