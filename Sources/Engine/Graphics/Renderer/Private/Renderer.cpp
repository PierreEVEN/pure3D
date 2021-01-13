#include "Renderer.h"
#include "IO/Log.h"
#include "Reflection/RClass.h"
#include "Scene/PrimitiveComponent.h"

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
		RenderPasses->DrawProxies();
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

void IRenderPass::QueryRenderedProxies(const std::vector<IPrimitiveProxy*>& AvailableProxies) {
	PassProxies.clear();
	for (const auto& Proxy : AvailableProxies) if (Proxy->RenderPass & RenderPass) PassProxies.push_back(Proxy);
}

void IRenderPass::DrawProxies() {
	for (const auto& Proxy : PassProxies) {
		LOG("draw proxy");
	}
}
