#include "OpenGLRenderer.h"
#include <GL/gl3w.h>
#include "Mesh.h"
#include "IO/Log.h"


void SOpenGLRenderer::BeginFrame() {
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SOpenGLRenderer::EndFrame() {}

SOpenGLRenderer::SOpenGLRenderer() {
	AddRenderPass(new SOpenGLRenderPass((uint32_t)ERenderPass::ERenderPass_COLOR));
}



void RenderApi() {


}


void SOpenGLRenderer::RenderMesh(SRenderer* Renderer, IPrimitiveProxy* Proxy) {
	SRendererApi::Get()->DrawMesh(Renderer, Proxy);
}

void SOpenGLMeshProxy::Render(SRenderer* Context)
{
	std::shared_ptr<SOpenGLMeshHandle> GLHandle = dynamic_pointer_cast<SOpenGLMeshHandle>(MeshHandle);
	//Meshproxy->Transform; // Material->setMat4

	if (!MeshHandle) return;

	glUseProgram(dynamic_pointer_cast<SOpenGLShaderHandle>(Meshproxy->MaterialHandle)->ShaderHandle);
	

	glBindVertexArray(MeshHandle
		MeshHandle->Vbo);
	if (MeshHandle->Triangles > 0)
		glDrawElements(GL_TRIANGLES, MeshHandle->Triangles, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, MeshHandle->Vertices);
}
