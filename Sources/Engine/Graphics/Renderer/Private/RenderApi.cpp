#include "RenderApi.h"
#include "IO/Log.h"
#include "Renderer.h"
#include "PrimitiveProxy.h"

IRendererApi* IRendererApi::RenderApiInstance = nullptr;

IRendererApi* IRendererApi::Get() {
	if (!IRendererApi::RenderApiInstance) LOG_ASSERT("Render api is not initialized (ie : SRendererApi::Create<SOpenGlRenderApi>(); )");
	return IRendererApi::RenderApiInstance;
}

IPrimitiveProxy* IRendererApi::CreateProxyFor(RClass* ProxyType, SPrimitiveComponent* inParentComponent, uint32_t inRenderPass) {
	auto FoundProxyClass = ProxyTypes.find(ProxyType);
	if (FoundProxyClass == ProxyTypes.end()) LOG_ASSERT("%s is not a suported proxy type : register proxy type with AddProxyType<ProxyType, ProxyClass>()", ProxyType->GetName().GetData());
	IPrimitiveProxy* Proxy = NewObject<IPrimitiveProxy>(FoundProxyClass->second);
	Proxy->ParentComponent = inParentComponent;
	Proxy->RenderPass = inRenderPass;
	if (!Proxy) LOG_ASSERT("Failed to create proxy for %s", ProxyType->GetName().GetData());
	return Proxy;
}

void IRendererApi::SetInstance(IRendererApi* NewInstance) {
	if (IRendererApi::RenderApiInstance) delete IRendererApi::RenderApiInstance;
	IRendererApi::RenderApiInstance = NewInstance;
}

