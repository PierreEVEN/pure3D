#include "IO/Log.h"
#include "EngineModule.h"

void ModuleManager::LoadModuleChecked(const String& ModuleName) {
	if (!LoadModule(ModuleName)) {
		LOG_ASSERT("Unknown module : %s", ModuleName.GetData())
	}
}

void ModuleManager::UnloadModuleChecked(const String& ModuleName) {
	if (!UnloadModule(ModuleName)) {
		LOG_ASSERT("Unknown module : %s", ModuleName.GetData())
	}
}

 bool ModuleManager::LoadModule(const String& ModuleName) {
	 if (!ModuleRegisterFunctions) return false;
	const auto FoundModule = ModuleRegisterFunctions->find(MakeUniqueID(ModuleName));
	if (FoundModule == ModuleRegisterFunctions->end()) return false;
	LOG("Load module '%s'", ModuleName.GetData());
	return FoundModule->second->Load();
}

 bool ModuleManager::UnloadModule(const String& ModuleName) {
	 if (!ModuleRegisterFunctions) return false;
	const auto FoundModule = ModuleRegisterFunctions->find(MakeUniqueID(ModuleName));
	if (FoundModule == ModuleRegisterFunctions->end()) return false;
	LOG("Unload module '%s'", ModuleName.GetData());
	return FoundModule->second->UnLoad();
}