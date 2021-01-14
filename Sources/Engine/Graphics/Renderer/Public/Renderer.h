#pragma once

#include <cstdint>
#include <unordered_map>
#include "Types/Matrix.h"
#include "Types/Vector.h"
#include "Camera.h"

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

	inline SCamera& GetCamera() { return Camera; }


	struct UniformBufferData {
		SMatrix4 ViewMatrix;
		SMatrix4 ProjectionMatrix;
		SVector4 cameraPosition;
		SVector4 cameraDirection;
		SVector2D framebufferResolution;
		float Time;
	};

protected:

	SRenderer();

	/**
	 * Before new frame
	 */
	virtual void RegisterNewProxies();
	virtual void RebuildDirtyProxyData();

	virtual void BeginFrame() {}
	virtual void UpdateUniformBuffers();
	virtual void CopyUniformBufferData() {}
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

	const UniformBufferData& GetUniformBuffer() const { return UniformBuffer; }

	virtual SVector2D GetFramebufferSize() = 0;

private:
	SCamera Camera;
	UniformBufferData UniformBuffer;

	std::vector<IPrimitiveProxy*> PendingRegistrationProxies;
	std::vector<IPrimitiveProxy*> RendererProxies;
	std::vector<IPrimitiveProxy*> OutdatedProxies;
	std::vector<SRenderPass*> RenderPasses;
};


