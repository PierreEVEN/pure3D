#pragma once

#include "Types/Matrix.h"
#include "Types/Vector.h"
#include "Camera.h"

class SceneRootComponent;
class RCLass;
struct SRenderPass;
struct IPrimitiveProxy;
class SCamera;


// A reimplementer a chaque fois
struct ICommandBuilder {
	virtual void GenerateCommands(class RClass* ProxyClass, std::vector<IPrimitiveProxy*>::iterator FirstItem, std::vector<IPrimitiveProxy*>::iterator LastItem) = 0;
};

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

	ICommandBuilder* CommandBuilder;
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


