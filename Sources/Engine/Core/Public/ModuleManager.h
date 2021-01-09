#pragma once
#include <unordered_map>
#include <functional>
#include "Types/String.h"

#define MODULE_FUNC_NAME _Module_Register_Function

struct SEngineModuleBase {

	friend class ModuleManager;

	inline void* GetHandle() const { return ModuleHandle; }
	inline const String& GetName() const { return ModuleName; }

private:

	String ModuleName;
	void* ModuleHandle;
};

using FEngineModuleFunc = SEngineModuleBase * (*)();
using FEngineModuleInitFunc = void(*)();


class ModuleManager final {
public:
	static SEngineModuleBase* LoadModule(const String& ModuleName);
	static bool UnloadModule(const String& ModuleName);

	template<typename Module = SEngineModuleBase>
	inline static Module* CreateModule() { return new SEngineModuleBase(); }

private:
	inline static std::unordered_map<String, SEngineModuleBase*> EngineModules;
};

#define MODULE_CONSTRUCTOR(...) \
void _Construct_Module_Function(); \
extern "C" SEngineModuleBase* MODULE_FUNC_NAME() { \
	_Construct_Module_Function(); \
	return ModuleManager::CreateModule<__VA_ARGS__>(); \
} \
void _Construct_Module_Function()