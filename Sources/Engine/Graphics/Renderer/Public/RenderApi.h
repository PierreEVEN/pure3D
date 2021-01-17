#pragma once
#include "Types/String.h"

#include "RenderApi.refl.h"
#include "Types/Matrix.h"

class SRenderer;
struct IPrimitiveProxy;
class SPrimitiveComponent;

class IRendererHelper {};

REFLECT()
class IRendererApi {
	REFLECT_BODY()

public:
	
	template<typename T>
	inline static void Create() { SetInstance(new T()); }
	static IRendererApi* Get();

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual bool ShouldCloseWindow() { return false; }

	IPrimitiveProxy* CreateProxyFor(RClass* ProxyType, SPrimitiveComponent* inParentComponent, uint32_t inRenderPass);
	IRendererHelper* GetHelperForProxy(RClass* ProxyType);

protected:

	template<typename ProxyType, typename ProxyClass, typename HelperClass>
	void AddProxyType() {
		ProxyTypes[ProxyType::GetStaticClass()] = ProxyClass::GetStaticClass();
		ProxyHelpers[ProxyType::GetStaticClass()] = new HelperClass();
	}

private:
	static void SetInstance(IRendererApi* NewInstance);
	static IRendererApi* RenderApiInstance;

	std::unordered_map<RClass*, IRendererHelper*> ProxyHelpers;
	std::unordered_map<RClass*, RClass*> ProxyTypes;
};