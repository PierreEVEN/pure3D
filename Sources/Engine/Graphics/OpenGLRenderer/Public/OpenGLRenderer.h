#pragma once

#include "Renderer.h"

#include "OpenGLRenderer.refl.h"

struct SOpenGLRenderer : public SRenderer {
	SOpenGLRenderer();

protected:

	virtual void BeginFrame();
	virtual void EndFrame();

private:
	static void RenderMesh(SRenderer* Renderer, IPrimitiveProxy* Proxy);

};

REFLECT()
struct SOpenGLMeshProxy : public SMeshProxy {
	REFLECT_BODY()
public:
	virtual void Render(SRenderer* Context);
};

struct SOpenGLRenderPass : public IRenderPass {

	using IRenderPass::IRenderPass;


};