#include "OpenGLRenderer.h"
#include <GL/gl3w.h>
#include "GLFW/glfw3.h"


void SOpenGLRenderer::PreDraw() {
	glfwPollEvents();
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SOpenGLRenderer::PostDraw() {
	glFlush();
	glfwSwapBuffers(WindowHandle);
}

void SOpenGLRenderer::RenderMesh(SRenderer* Renderer, IPrimitiveProxy* Proxy)
{
	LOG_WARNING("Render mesh : " + Proxy->GetClass()->GetName());
}

SOpenGLRenderer::SOpenGLRenderer() {

	/**
	 * Register render functions
	 */
	RegisterRenderFunction<MeshProxy>(&RenderMesh);
}

void SOpenGLRenderer::CreateOpenGLContext()
{
	if (!glfwInit()) LOG_ASSERT("Failed to create glfw window");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);


	WindowHandle = glfwCreateWindow(800, 600, "test", 0, 0);
	if (!WindowHandle) LOG_ASSERT("Failed to create Glfw window handle");
	glfwMakeContextCurrent(WindowHandle);

	LOG("Initialize OpenGL context");
	if (gl3wInit()) {
		LOG_ASSERT("failed to initialize OpenGL");
	}
	if (!gl3wIsSupported(3, 2)) {
		LOG_ASSERT("OpenGL 3.2 not supported");
	}
	LOG("OpenGL %s, GLSL %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void SOpenGLRenderer::CloseOpenGLContext()
{
}
