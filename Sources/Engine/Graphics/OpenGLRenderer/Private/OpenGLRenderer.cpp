#include "OpenGLRenderer.h"
#include <GL/gl3w.h>
#include "Mesh.h"
#include "IO/Log.h"
#include "RenderPass.h"

SOpenGLRenderer::SOpenGLRenderer() {
	AddRenderPass(new SRenderPass((uint32_t)ERenderPass::ERenderPass_COLOR));
}

void SOpenGLRenderer::BeginFrame() {
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
