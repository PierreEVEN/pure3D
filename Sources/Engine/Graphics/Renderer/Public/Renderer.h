#pragma once

#include <cstdint>
#include <unordered_map>

#include "Types/Transform.h"

#include "Renderer.refl.h"

class SceneRootComponent;
struct SRenderer;
struct IPrimitiveProxy;
struct RCLass;

enum class ERenderPass {
	ERenderPass_COLOR = 1,
	ERenderPass_NORMAL = 1 << 1,
	ERenderPass_SHADOWS = 1 << 2,
	ERenderPass_PICKING = 1 << 3,
	ERenderPass_POSTPROCESS = 1 << 4
};

class SPrimitiveComponent;

REFLECT()
struct IPrimitiveProxy {
	REFLECT_BODY()

public:

	friend struct SRenderer;
	friend class SPrimitiveComponent;
	using FProxyUpdateFunc = std::function<void()>;

	IPrimitiveProxy(SPrimitiveComponent* inParentComponent, uint32_t inRenderPass) : ParentComponent(inParentComponent), RenderPass(inRenderPass) {}

	uint32_t RenderPass = 0;

	virtual void Render(SRenderer* Context) = 0;

private:
	bool IsOutOfDate = false;
	bool IsDirty = true;
	SPrimitiveComponent* ParentComponent;
};

struct IRenderPass {
	
	IRenderPass(uint32_t inRenderPass) : RenderPass(inRenderPass) {}

	void Begin() {}
	void End() {}

	virtual void QueryRenderedProxies(const std::vector<IPrimitiveProxy*>& AvailableProxies);
	virtual void SortProxies() {}
	virtual void DrawProxies();

private:
	uint32_t RenderPass;
	std::vector<IPrimitiveProxy*> PassProxies;
};

struct SRenderer {

	SRenderer() = default;

public:

	virtual void DrawFrame();

	inline void AddProxy(IPrimitiveProxy* Proxy) { PendingRegistrationProxies.push_back(Proxy); }
	inline void FreeProxy(IPrimitiveProxy* Proxy) { OutdatedProxies.push_back(Proxy); }

protected:

	/**
	 * Before new frame
	 */
	virtual void RegisterNewProxies();
	virtual void RebuildDirtyProxyData();

	/**
	 * After frame end
	 */
	virtual void FlushOutdatedProxies();

	virtual void BeginFrame() {}
	virtual void EndFrame() {}

	/**
	 * Render pass
	 */
	virtual void DrawRenderPasses();

	inline void AddRenderPass(IRenderPass* RenderPass) { RenderPasses.push_back(RenderPass); }

private:

	std::vector<IPrimitiveProxy*> PendingRegistrationProxies;
	std::vector<IPrimitiveProxy*> RendererProxies;
	std::vector<IPrimitiveProxy*> OutdatedProxies;
	std::vector<IRenderPass*> RenderPasses;
};


