#pragma once
#include "Types/String.h"

#include "RenderApi.refl.h"
#include "IO/Log.h"

#define REGISTER_RENDERAPI_FUNCTION(FunctionName, Return, ...) \
SRendererApi::GetStaticClass()->AddFunction(RFunction::MakeFunction<Return, SOpenGlRenderApi, __VA_ARGS__>(#FunctionName, &SOpenGlRenderApi::FunctionName))

struct SShaderHandle {
	virtual ~SShaderHandle() = default;
};

struct STextureHandle {
	virtual ~STextureHandle() = default;
};

struct SMeshHandle {
	virtual ~SMeshHandle() = default;
};

struct SRenderer;
struct IPrimitiveProxy;

REFLECT()
class SRendererApi {
	REFLECT_BODY()

public:

	template<typename Return, typename ... Arguments>
	inline static Return ExecuteFunction(const String& FunctionName, Arguments... Args) {
		if (RFunction* Function = SRendererApi::GetStaticClass()->GetFunction<Return, Arguments...>(FunctionName)) {
			return Function->Execute<Return, Arguments...>(SRendererApi::Get(), std::forward<Arguments>(Args)...);
		}
		LOG_ASSERT("Invalid operation : " + FunctionName);
	}

	virtual std::shared_ptr<SShaderHandle> CompileShader(const String& VertexShader, const String& FragmentShader) = 0;
	virtual std::shared_ptr<STextureHandle> CreateTexture(const uint8_t* TextureData, uint32_t Width, uint32_t Height, uint32_t Channels) = 0;
	virtual std::shared_ptr<SMeshHandle> CreateMesh(const struct SMeshData* Data) = 0;

	template<typename T>
	inline static void Create() { SetInstance(new T()); }
	static SRendererApi* Get();

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

private:
	static void SetInstance(SRendererApi* NewInstance);
	static SRendererApi* RenderApiInstance;
};