#include "OpenGLRenderer.h"
#include "GL/gl3w.h"
#include "IO/Log.h"


OpenGLRenderer::OpenGLRenderer(int MajorVersion, int MinorVersion)
{
	LOG("Initialize OpenGL context");
	if (gl3wInit()) {
		LOG_ASSERT("failed to initialize OpenGL");
	}
	if (!gl3wIsSupported(MajorVersion, MinorVersion)) {
		LOG_ASSERT("OpenGL 3.2 not supported");
	}
	LOG("OpenGL %s, GLSL %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void OpenGLRenderer::NextFrame()
{
	glClearColor(0, 0, 1, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
