#pragma once

#include <stdint.h>
#include <functional>

#include "PrimitiveProxy.refl.h"

class SRenderer;
class SPrimitiveComponent;

REFLECT()
struct IPrimitiveProxy {
	REFLECT_BODY()

public:

	friend SRenderer;
	friend SPrimitiveComponent;

	using FProxyUpdateFunc = std::function<void()>;

	IPrimitiveProxy() = default;

	uint32_t RenderPass = 0;
	SPrimitiveComponent* ParentComponent;

	virtual void Render(SRenderer* Context) = 0;

private:
	bool IsOutOfDate = false;
	bool IsDirty = true;
};
