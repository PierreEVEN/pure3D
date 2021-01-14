#pragma once

#include <cstdint>
#include <unordered_map>

class SceneRootComponent;
struct RCLass;
struct SRenderPass;
struct IPrimitiveProxy;
class SCamera;


class SRenderer {

public:

	virtual void DrawFrame();

	inline void AddProxy(IPrimitiveProxy* Proxy) { PendingRegistrationProxies.push_back(Proxy); }
	inline void FreeProxy(IPrimitiveProxy* Proxy) { OutdatedProxies.push_back(Proxy); }

	inline SCamera* GetCamera() const { return Camera; }

protected:

	SRenderer();

	/**
	 * Before new frame
	 */
	virtual void RegisterNewProxies();
	virtual void RebuildDirtyProxyData();

	virtual void BeginFrame() {}
	virtual void EndFrame() {}

	/**
	 * After frame end
	 */
	virtual void FlushOutdatedProxies();

	/**
	 * Render pass
	 */
	virtual void DrawRenderPasses();

	inline void AddRenderPass(SRenderPass* RenderPass) { RenderPasses.push_back(RenderPass); }

private:
	SCamera* Camera;

	std::vector<IPrimitiveProxy*> PendingRegistrationProxies;
	std::vector<IPrimitiveProxy*> RendererProxies;
	std::vector<IPrimitiveProxy*> OutdatedProxies;
	std::vector<SRenderPass*> RenderPasses;
};


