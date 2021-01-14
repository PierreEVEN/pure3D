#pragma once
#include "Types/String.h"

#include "RenderApi.refl.h"

class SRenderer;
struct IPrimitiveProxy;
class SPrimitiveComponent;

struct SShaderHandle {
	virtual ~SShaderHandle() = default;
};

struct STextureHandle {
	virtual ~STextureHandle() = default;
};

struct SMeshHandle {
	virtual ~SMeshHandle() = default;
};

REFLECT()
class SRendererApi {
	REFLECT_BODY()

public:

	virtual std::shared_ptr<SShaderHandle> CompileShader(const String& VertexShader, const String& FragmentShader) = 0;
	virtual std::shared_ptr<STextureHandle> CreateTexture(const uint8_t* TextureData, uint32_t Width, uint32_t Height, uint32_t Channels) = 0;
	virtual std::shared_ptr<SMeshHandle> CreateMesh(const struct SMeshData* Data) = 0;

	template<typename T>
	inline static void Create() { SetInstance(new T()); }
	static SRendererApi* Get();

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	IPrimitiveProxy* CreateProxyFor(RClass* ProxyType, SPrimitiveComponent* inParentComponent, uint32_t inRenderPass);

protected:

	template<typename ProxyType, typename ProxyClass>
	void AddProxyType() {
		ProxyTypes[ProxyType::GetStaticClass()] = ProxyClass::GetStaticClass();
	}

private:
	static void SetInstance(SRendererApi* NewInstance);
	static SRendererApi* RenderApiInstance;

	std::unordered_map<RClass*, RClass*> ProxyTypes;
};