#pragma once

#include "SceneComponent.h"
#include "Scene/PrimitiveComponent.refl.h"

struct IPrimitiveProxy;
struct SRenderer;

REFLECT()
class SPrimitiveComponent : public SSceneComponent {

	REFLECT_BODY()
public:

	SPrimitiveComponent(SRenderer* inContext);

	inline void MarkProxiesDirty() { 
		bAreProxiesDirty = true;
		UpdateProxies_Internal(); // @TODO Remove
	}

	// Mark proxies dirty on component moved
	inline virtual void SetRelativeTransform(const STransform& inTransform) { SSceneComponent::SetRelativeTransform(inTransform); MarkProxiesDirty(); }

	void SetRenderContext(SRenderer* Context);

protected:

	void AddProxy(IPrimitiveProxy* NewProxy);
	void ClearProxies();

	virtual void UpdateProxy(IPrimitiveProxy* Proxy, size_t ProxyID) = 0;

private:

	inline void UpdateProxies_Internal() {
		for (int i = 0; i < Proxies.size(); ++i) UpdateProxy(*(Proxies.begin() + i), i);
		bAreProxiesDirty = false;
	}

	SRenderer* Context;
	std::vector<IPrimitiveProxy*> Proxies;
	bool bAreProxiesDirty;
};