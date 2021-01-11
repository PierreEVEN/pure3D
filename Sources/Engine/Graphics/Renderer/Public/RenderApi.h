#pragma once
#include "Types/String.h"

#include "RenderApi.refl.h"
#include "IO/Log.h"

#define REGISTER_RENDERAPI_FUNCTION(FunctionName, Return, ...) \
SRendererApi::GetStaticClass()->AddFunction(RFunction::MakeFunction<Return, SOpenGlRenderApi, __VA_ARGS__>(#FunctionName, &SOpenGlRenderApi::FunctionName))

struct SShaderHandle {};

struct STextureHandle {};

struct SMeshHandle {};

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


	template<typename T>
	inline static void Create() { SetInstance(new T()); }
	static SRendererApi* Get();

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

private:
	static void SetInstance(SRendererApi* NewInstance);
	static SRendererApi* RenderApiInstance;
};