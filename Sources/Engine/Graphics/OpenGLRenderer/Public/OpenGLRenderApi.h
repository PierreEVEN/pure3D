#pragma once
#include "RenderApi.h"
#include <GL/gl3w.h>
#include "GLFW/glfw3.h"

#include "OpenGLRenderApi.refl.h"

REFLECT()
class SOpenGlRenderApi : public SRendererApi {
	friend SRendererApi;

	REFLECT_BODY()
public:
// 	virtual GApiHandle CreateShader(const String& VertexData, const String& FragmentData);
// 	virtual void CreateMesh(GApiHandle& VertexBuffer, GApiHandle& IndexBuffer, void* MeshData);
// 	virtual SBuffer* CreateBuffer(const SBufferProperty& Properties) = 0;
// 	virtual void UpdateBuffer(void* BufferData, size_t DataLength) = 0;

	virtual void BeginFrame();
	virtual void EndFrame();

protected:
	SOpenGlRenderApi();

private:

	void StartOpenGL();


	SShaderHandle CompileShader(const String& VertexShader, const String& FragmentShader);
	STextureHandle CreateTexture(const uint8_t* TextureData, uint32_t Width, uint32_t Height, uint32_t Channels);
	SMeshHandle CreateMesh(const struct SMeshData* Data);

	struct GLFWwindow* WindowHandle;

};
