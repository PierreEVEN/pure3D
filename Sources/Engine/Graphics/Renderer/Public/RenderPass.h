#pragma once
#include <vector>
#include <stdint.h>

class SRenderer;

struct IPrimitiveProxy;

enum class ERenderPass {
	ERenderPass_COLOR = 1,
	ERenderPass_NORMAL = 1 << 1,
	ERenderPass_SHADOWS = 1 << 2,
	ERenderPass_PICKING = 1 << 3,
	ERenderPass_POSTPROCESS = 1 << 4
};

struct SRenderPass {

	SRenderPass(uint32_t inRenderPass) : RenderPass(inRenderPass) {}

	void Begin() {}
	void End() {}

	virtual void QueryRenderedProxies(const std::vector<IPrimitiveProxy*>& AvailableProxies);
	virtual void SortProxies() {}
	virtual void DrawProxies(SRenderer* Context);

private:
	uint32_t RenderPass;
	std::vector<IPrimitiveProxy*> PassProxies;
};