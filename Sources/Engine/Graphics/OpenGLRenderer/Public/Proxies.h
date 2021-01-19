#pragma once

#include "Mesh.h"

#include "Proxies.refl.h"

class SRenderer;

REFLECT()
struct SOpenGLMeshProxy : public SMeshProxy {
	REFLECT_BODY()

		RCONSTRUCTOR()
		SOpenGLMeshProxy() {}
public:
	virtual void Render(SRenderer* Context) {}
};