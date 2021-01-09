
#include "Renderer.h"
#include "Mesh.h"
#include "IO/Log.h"

struct SOpenGLRenderer : public SRenderer {
	SOpenGLRenderer();

	static void CreateOpenGLContext();
	static void CloseOpenGLContext();

protected:

	virtual void PreDraw();
	virtual void PostDraw();

private:
	inline static struct GLFWwindow* WindowHandle;
	static void RenderMesh(SRenderer* Renderer, IPrimitiveProxy* Proxy);
};