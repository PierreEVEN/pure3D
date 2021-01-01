#include "Renderer.h"
#include "WindowInterface.h"
#include "RendererInterface.h"

void SRenderer::StartRenderer() {
	while (WindowInterface && !WindowInterface->ShouldClose()) {
		WindowInterface->CaptureInputs();
		if (RendererInterface) RendererInterface->NextFrame();
		WindowInterface->NextFrame();
	}
}

SRenderer::~SRenderer() {
	if (WindowInterface) delete WindowInterface;
	if (RendererInterface) delete RendererInterface;
}