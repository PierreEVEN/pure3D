#include "Scene/PrimitiveComponent.h"
#include "Renderer.h"
#include "IO/Log.h"
#include "PrimitiveProxy.h"

SPrimitiveComponent::SPrimitiveComponent(SRenderer* inContext)
	: Context(nullptr) {
	SetRenderContext(inContext);
}

void SPrimitiveComponent::MarkProxiesDirty() {
	for (const auto& Proxy : Proxies) Proxy->IsDirty = true;
}

void SPrimitiveComponent::SetRenderContext(SRenderer* inContext) {
	if (Context) for (auto& Proxy : Proxies) Context->FreeProxy(Proxy);
	Context = inContext;
	for (auto& Proxy : Proxies) Context->AddProxy(Proxy);
}

void SPrimitiveComponent::AddProxy(IPrimitiveProxy* NewProxy) {
	Proxies.push_back(NewProxy);
	Context->AddProxy(NewProxy);
	MarkProxiesDirty();
}

void SPrimitiveComponent::ClearProxies() {
	for (auto& Proxy : Proxies) Context->FreeProxy(Proxy);
	Proxies.clear();
	MarkProxiesDirty();
}

void SPrimitiveComponent::UpdateProxies_Internal() {
	UpdateProxies();
	for (const auto& Proxy : Proxies) Proxy->IsDirty = false;
}
