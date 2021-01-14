#include "Renderer.h"
#include "IO/Log.h"
#include "Scene/PrimitiveComponent.h"
#include "RenderPass.h"
#include "PrimitiveProxy.h"
#include "Camera.h"


SRenderer::SRenderer()
{
	Camera = new SCamera();
}

void SRenderer::DrawFrame()
{
	RegisterNewProxies();
	RebuildDirtyProxyData();

	BeginFrame();
	DrawRenderPasses();

	EndFrame();

	FlushOutdatedProxies();
}

void SRenderer::DrawRenderPasses() {
	for (const auto& RenderPasses : RenderPasses) {
		RenderPasses->Begin();
		RenderPasses->QueryRenderedProxies(RendererProxies);
		RenderPasses->SortProxies();
		RenderPasses->DrawProxies(this);
		RenderPasses->End();
	}
}


void SRenderer::RegisterNewProxies() {
	RendererProxies.insert(RendererProxies.end(), PendingRegistrationProxies.begin(), PendingRegistrationProxies.end());
	PendingRegistrationProxies.clear();
}

void SRenderer::RebuildDirtyProxyData() {
	for (const auto& Proxy : RendererProxies) if (Proxy->IsDirty) Proxy->ParentComponent->UpdateProxies_Internal();
}

void SRenderer::FlushOutdatedProxies() {
	for (const auto& Proxy : OutdatedProxies) RendererProxies.erase(std::find(RendererProxies.begin(), RendererProxies.end(), Proxy));
	OutdatedProxies.clear();
}

void SRenderPass::QueryRenderedProxies(const std::vector<IPrimitiveProxy*>& AvailableProxies) {
	PassProxies.clear();
	for (const auto& Proxy : AvailableProxies) if (Proxy->RenderPass & RenderPass) PassProxies.push_back(Proxy);
}

void SRenderPass::DrawProxies(SRenderer* Context) {
	for (const auto& Proxy : PassProxies) {
		Proxy->Render(Context);
	}
}
