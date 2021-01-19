#pragma once
#include <functional>
#include <vector>
#include <cstdint>
#include <unordered_map>

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

	/**
	 * On trie d'abord par priorité, puis pour chaque sous groupe, on trie par class de proxy, puis pour chaque class on lance la fonction de tri interne
	 */	
	virtual void SortProxies();
	virtual void DrawProxies(SRenderer* Context);

	using SortFunction = std::function<void(std::vector<IPrimitiveProxy*>::const_iterator, std::vector<IPrimitiveProxy*>::const_iterator)>;	
	std::unordered_map<class RClass*, SortFunction> SortFunctions;

private:
	uint32_t RenderPass;
	std::vector<IPrimitiveProxy*> PassProxies;
};