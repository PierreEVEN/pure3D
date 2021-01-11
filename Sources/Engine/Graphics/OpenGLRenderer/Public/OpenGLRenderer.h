#pragma once

#include "Renderer.h"

struct SOpenGLRenderer : public SRenderer {
	SOpenGLRenderer();

protected:

	virtual void PreDraw();
	virtual void PostDraw();

private:
	static void RenderMesh(SRenderer* Renderer, IPrimitiveProxy* Proxy);

};