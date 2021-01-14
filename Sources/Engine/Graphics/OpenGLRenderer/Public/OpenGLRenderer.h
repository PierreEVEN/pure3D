#pragma once

#include "Renderer.h"

#include "OpenGLRenderer.refl.h"

struct SOpenGLRenderer : public SRenderer {
	SOpenGLRenderer();
protected:
	virtual void BeginFrame();
};