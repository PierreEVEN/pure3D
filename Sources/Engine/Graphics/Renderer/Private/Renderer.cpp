#include "Renderer.h"

void SRenderer::StartRenderer() {
	while (WindowInterface && !WindowInterface->ShouldClose()) {
		WindowInterface->CaptureInputs();
		if (RendererInterface) RendererInterface->NextFrame();
		WindowInterface->NextFrame();
	}
}
