#pragma once
#include "RenderApi.h"
#include <GL/gl3w.h>
#include "GLFW/glfw3.h"
#include "Mesh.h"
#include <memory>

#include "OpenGLRenderApi.refl.h"


REFLECT()
class SOpenGlRenderApi : public IRendererApi {
	friend IRendererApi;

	REFLECT_BODY()
public:

	virtual void BeginFrame();
	virtual void EndFrame();

	struct GLFWwindow* WindowHandle;
	virtual bool ShouldCloseWindow();
	
protected:
	SOpenGlRenderApi();
	
private:

	void StartOpenGL();

};

