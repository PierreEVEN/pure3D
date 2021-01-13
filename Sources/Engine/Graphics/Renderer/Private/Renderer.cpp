#include "Renderer.h"
#include "IO/Log.h"
#include "Reflection/RClass.h"

void SRenderer::NextFrame() {
	PreDraw();
	RenderDrawList(BuildDrawList());
	PostDraw();
}

void SRenderer::RenderDrawList(const std::vector<IPrimitiveProxy*>& DrawList) {
	for (const auto& Item : DrawList) {
		RenderItem(Item);
	}
}

std::vector<IPrimitiveProxy*> SRenderer::BuildDrawList()
{
	return Proxies;
}

void SRenderer::RenderItem(IPrimitiveProxy* Proxy) {
	auto RenderFunction = RenderFunctions.find(Proxy->GetClass());
	if (RenderFunction == RenderFunctions.end()) {
		LOG_ERROR("invalid operation : %s can't be rendered in current context", Proxy->GetClass()->GetName().GetData());
		return;
	}
	RenderFunction->second(this, Proxy);
}
