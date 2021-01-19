#pragma once

#include <cstdint>
#include <functional>

#include "PrimitiveProxy.refl.h"

class SRenderer;
class SPrimitiveComponent;
class IRendererHelper;

REFLECT()
struct IPrimitiveProxy {
	REFLECT_BODY();

	friend SRenderer;
	friend SPrimitiveComponent;

public:

	IPrimitiveProxy() = default;
	virtual void Render(SRenderer * Context) = 0;

	uint32_t RenderPass = 0;
	SPrimitiveComponent* ParentComponent;
	IRendererHelper* RenderHelper = nullptr;
	int8_t Priority = 0;


private:

	bool IsOutOfDate = false;
	bool IsDirty = true;
};
