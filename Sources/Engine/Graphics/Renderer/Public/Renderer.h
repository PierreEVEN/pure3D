#pragma once

#include <cstdint>
#include <unordered_map>

#include "Types/Transform.h"

#include "Renderer.refl.h"

class SceneRootComponent;
struct SRenderer;
struct IPrimitiveProxy;
struct RCLass;

REFLECT()
struct IPrimitiveProxy {
	REFLECT_BODY()

	uint32_t RenderPass; // Bitmask for used render pass
};

struct SRenderer {

	using FRenderFunction = std::function<void(SRenderer*, IPrimitiveProxy*)>;

	SRenderer() = default;

	virtual void NextFrame();

	inline void AddProxy(IPrimitiveProxy* Proxy) { Proxies.push_back(Proxy); }

	template<typename Class>
	inline void RegisterRenderFunction(FRenderFunction&& RenderOperation) {
		RenderFunctions[Class::GetStaticClass()] = std::move(RenderOperation);
	}

	void RenderItem(IPrimitiveProxy* Proxy);

protected:
	virtual void PreDraw() = 0;
	virtual void RenderDrawList(const std::vector<IPrimitiveProxy*>& DrawList);
	virtual void PostDraw() = 0;

private:

	std::vector<IPrimitiveProxy*> BuildDrawList();

	std::vector<IPrimitiveProxy*> Proxies; // Proxies array
	std::unordered_map<RClass*, FRenderFunction> RenderFunctions;
};


