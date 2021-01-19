#include "Renderer.h"
#include "IO/Log.h"
#include "Scene/PrimitiveComponent.h"
#include "RenderPass.h"
#include "PrimitiveProxy.h"
#include "Camera.h"
#include <chrono>


SRenderer::SRenderer() {}

void SRenderer::DrawFrame()
{
	RegisterNewProxies();
	RebuildDirtyProxyData();

	BeginFrame();
	UpdateUniformBuffers();
	CopyUniformBufferData();

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

auto StartTime = std::chrono::steady_clock::now();

void SRenderer::UpdateUniformBuffers()
{
	UniformBuffer.Time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - StartTime).count() / 1000.f;
	UniformBuffer.ViewMatrix = Camera.GetView();
	UniformBuffer.framebufferResolution = GetFramebufferSize();
	UniformBuffer.ProjectionMatrix = Camera.GetPerspective(UniformBuffer.framebufferResolution.x / UniformBuffer.framebufferResolution.y);
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
	if (PassProxies.empty()) return;
	RClass* LastClass = PassProxies[0]->GetClass();
	size_t Begin = 0;
	for (int i = 0; i < PassProxies.size(); ++i) {
		if (PassProxies[i]->GetClass() != LastClass || i == PassProxies.size() - 1) {
			Context->CommandBuilder->GenerateCommands(LastClass, PassProxies.begin() + Begin, PassProxies.begin() + i - 1);
			LastClass = PassProxies[i]->GetClass();
			Begin = i;
		}
	}
}
