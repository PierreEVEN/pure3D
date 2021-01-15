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

	virtual std::shared_ptr<SShaderHandle> CompileShader(const String& VertexShader, const String& FragmentShader);
	virtual std::shared_ptr<STextureHandle> CreateTexture(const uint8_t* TextureData, uint32_t Width, uint32_t Height, uint32_t Channels);
	virtual std::shared_ptr<SMeshHandle> CreateMesh(const struct SMeshData* Data);

private:

	void StartOpenGL();

};

