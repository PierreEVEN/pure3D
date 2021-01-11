#include "RenderApi.h"
#include "IO/Log.h"

SRendererApi* SRendererApi::RenderApiInstance = nullptr;

SRendererApi* SRendererApi::Get() {
	if (!SRendererApi::RenderApiInstance) LOG_ASSERT("Render api is not initialized (ie : SRendererApi::Create<SOpenGlRenderApi>(); )");
	return SRendererApi::RenderApiInstance;
}

void SRendererApi::SetInstance(SRendererApi* NewInstance) {
	if (SRendererApi::RenderApiInstance) delete SRendererApi::RenderApiInstance;
	SRendererApi::RenderApiInstance = NewInstance;
}

