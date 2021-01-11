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
	SMeshProxy* Meshproxy = static_cast<SMeshProxy*>(Proxy);


// 	//Meshproxy->Transform; // Material->setMat4
// 	Meshproxy->MeshHandle;
// 	glBindVertexArray(static_cast<SOpenGLMeshHandle>(Meshproxy->MeshHandle));
// 	if (GetSectionData()->indicesCount > 0)
// 		glDrawElements(GL_TRIANGLES, GetSectionData()->indicesCount, GL_UNSIGNED_INT, 0);
// 	else
// 		glDrawArrays(GL_TRIANGLES, 0, GetSectionData()->verticesCount);
}