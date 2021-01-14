#include "RenderApi.h"
#include "IO/Log.h"
#include "Renderer.h"
#include "PrimitiveProxy.h"

SRendererApi* SRendererApi::RenderApiInstance = nullptr;

SRendererApi* SRendererApi::Get() {
	if (!SRendererApi::RenderApiInstance) LOG_ASSERT("Render api is not initialized (ie : SRendererApi::Create<SOpenGlRenderApi>(); )");
	return SRendererApi::RenderApiInstance;
}

IPrimitiveProxy* SRendererApi::CreateProxyFor(RClass* ProxyType, SPrimitiveComponent* inParentComponent, uint32_t inRenderPass) {
	auto FoundProxyClass = ProxyTypes.find(ProxyType);
	if (FoundProxyClass == ProxyTypes.end()) LOG_ASSERT("%s is not a suported proxy type : register proxy type with AddProxyType<ProxyType, ProxyClass>()", ProxyType->GetName().GetData());
	IPrimitiveProxy* Proxy = NewObject<IPrimitiveProxy>(FoundProxyClass->second);
	Proxy->ParentComponent = inParentComponent;
	Proxy->RenderPass = inRenderPass;
	if (!Proxy) LOG_ASSERT("Failed to create proxy for %s", ProxyType->GetName().GetData());
	return Proxy;
}

void SRendererApi::SetInstance(SRendererApi* NewInstance) {
	if (SRendererApi::RenderApiInstance) delete SRendererApi::RenderApiInstance;
	SRendererApi::RenderApiInstance = NewInstance;
}

