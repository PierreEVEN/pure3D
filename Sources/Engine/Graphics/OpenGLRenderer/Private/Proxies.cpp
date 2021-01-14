#include "Proxies.h"
#include <GL/gl3w.h>
#include "Handles.h"

void SOpenGLMeshProxy::Render(SRenderer* Context)
{
	std::shared_ptr<SOpenGLMeshHandle> GLHandle = dynamic_pointer_cast<SOpenGLMeshHandle>(MeshHandle);
	//Meshproxy->Transform; // Material->setMat4

	if (!GLHandle) return;

	glUseProgram(dynamic_pointer_cast<SOpenGLShaderHandle>(MaterialHandle)->ShaderHandle);


	glBindVertexArray(GLHandle->Vbo);
	if (GLHandle->Triangles > 0)
		glDrawElements(GL_TRIANGLES, GLHandle->Triangles, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, GLHandle->Vertices);
}
