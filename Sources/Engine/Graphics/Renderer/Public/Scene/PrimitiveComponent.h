#pragma once

#include "SceneComponent.h"
#include "Scene/PrimitiveComponent.refl.h"

struct IPrimitiveProxy;
class SRenderer;

REFLECT()
class SPrimitiveComponent : public SSceneComponent {

	friend SRenderer;

	REFLECT_BODY()
public:

	SPrimitiveComponent(SRenderer* inContext);

	void MarkProxiesDirty();

	// Mark proxies dirty on component moved
	inline virtual void SetRelativeTransform(const STransform& inTransform) { SSceneComponent::SetRelativeTransform(inTransform); MarkProxiesDirty(); }

	void SetRenderContext(SRenderer* Context);

protected:

	void AddProxy(IPrimitiveProxy* NewProxy);
	inline const std::vector<IPrimitiveProxy*>& GetProxies() const { return Proxies; }
	void ClearProxies();

	virtual void UpdateProxies() = 0;

private:

	void UpdateProxies_Internal();

	SRenderer* Context;
	std::vector<IPrimitiveProxy*> Proxies;
};