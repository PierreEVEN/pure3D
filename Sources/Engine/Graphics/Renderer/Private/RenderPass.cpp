

#include "RenderPass.h"
#include "PrimitiveProxy.h"

#include <algorithm>

bool SortByPriorities(IPrimitiveProxy* ProxyA, IPrimitiveProxy* ProxyB) { return ProxyA->Priority < ProxyB->Priority; }
bool SortByClass(IPrimitiveProxy* ProxyA, IPrimitiveProxy* ProxyB) { return ProxyA->GetClass() < ProxyB->GetClass(); }

void SRenderPass::SortProxies()
{
	if (PassProxies.empty()) return;
	// Sort by priority
	std::sort(PassProxies.begin(), PassProxies.end(), SortByPriorities);

	// Sort by proxy class
	size_t Begin = 0;
	int8_t LastPriority = PassProxies[0]->Priority;
	for (size_t i = 0; i < PassProxies.size(); ++i)
	{
		if (PassProxies[i]->Priority != LastPriority)
		{
			std::sort(PassProxies.begin() + Begin, PassProxies.begin() + i - 1, SortByClass);
			Begin = i;
			LastPriority = PassProxies[i]->Priority;
		}
	}

	// Sort by internal behavior
	Begin = 0;
	RClass* LastType = PassProxies[0]->GetClass();
	for (size_t i = 0; i < PassProxies.size(); ++i)
	{
		RClass* ProxyClass = PassProxies[i]->GetClass();
		if (ProxyClass != LastType || i == PassProxies.size() - 1)
		{
			auto SortFunction = SortFunctions.find(ProxyClass);
			// Call sort function if sort function exists
			if (SortFunction != SortFunctions.end()) SortFunction->second(PassProxies.begin() + Begin, PassProxies.begin() + i - 1);			
			std::sort(PassProxies.begin() + Begin, PassProxies.begin() + i - 1, SortByClass);
			Begin = i;
			LastType = ProxyClass;
		}
	}
}
