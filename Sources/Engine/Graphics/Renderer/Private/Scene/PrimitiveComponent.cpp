#include "Scene/PrimitiveComponent.h"
#include "Renderer.h"

SPrimitiveComponent::SPrimitiveComponent(SRenderer* inContext)
	: Context(nullptr), bAreProxiesDirty(true) {
	SetRenderContext(inContext);
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