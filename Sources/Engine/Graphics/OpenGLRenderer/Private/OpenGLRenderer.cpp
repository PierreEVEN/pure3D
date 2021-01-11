#include "OpenGLRenderer.h"
#include <GL/gl3w.h>
#include "Mesh.h"
#include "IO/Log.h"


void SOpenGLRenderer::PreDraw() {
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SOpenGLRenderer::PostDraw() {
	glFlush();
}

SOpenGLRenderer::SOpenGLRenderer() {


	/**
	 * Register render functions
	 */
	RegisterRenderFunction<SMeshProxy>(&RenderMesh);
}


void SOpenGLRenderer::RenderMesh(SRenderer* Renderer, IPrimitiveProxy* Proxy)
{
	SRendererApi::Get()->DrawMesh(Renderer, Proxy);
}