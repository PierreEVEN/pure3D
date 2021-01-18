#pragma once
#include <unordered_map>

#include "RenderHelper.refl.h"

REFLECT()
class IRenderHelper
{
REFLECT_BODY();
public:

	template<typename HelperClass>
	HelperClass* GetHelper() {
		auto Helper = Helpers.find(HelperClass::GetStaticClass());
		if (Helper == Helpers.end()) return nullptr;
		return static_cast<HelperClass*>(Helper->second);
	}

	template<typename HelperClass, typename HelperImplementation>
	void RegisterHelper() {
		Helpers[HelperClass::GetStaticClass()] = new HelperImplementation();
	}

private:
	
	std::unordered_map<RClass, IRenderHelper*> Helpers;	
};
