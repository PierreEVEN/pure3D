#include "IO/Log.h"
#include "ModuleManager.h"
#include "UniformMacros.h"
#if _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#endif


#if _WIN32
String GetSharedLibExtension() { return "dll"; }
void* LoadModuleHandle(const String& Path) { return (void*)LoadLibraryA(Path.GetData()); }
void* GetHandleSymbol(void* Handle, const String& SymbolName) { return GetProcAddress(static_cast<HMODULE>(Handle), SymbolName.GetData()); }
void FreeModuleHandle(void* Handle) { FreeLibrary((HMODULE)Handle); }
String ReadLastError() {
	DWORD ErrorMessage = GetLastError();
	LPSTR Buffer = nullptr;
	size_t MessageSize = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM	| FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
		ErrorMessage, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&Buffer, 0, NULL);
	std::string Message(Buffer, MessageSize);
	LocalFree(Buffer);
	return Message.c_str();
}
#else
void* LoadModuleHandle(const String& Path) { return dlopen(path.c_str(), RTLD_LAZY); }
String GetSharedLibExtension() { return "so"; }
void* GetHandleSymbol(void* Handle, const String& SymbolName) { return dlsym(Handle, SymbolName.GetData()); }
void FreeModuleHandle(void* Handle) { dlclose(Handle); }
#endif



SEngineModuleBase* ModuleManager::LoadModule(const String& ModuleName) {

	String ModulePath = DEPENDENCIES_ROOT_PATH / ModuleName + "." + GetSharedLibExtension();
	LOG("Load module '%s'", ModuleName.GetData());

	void* handle = LoadModuleHandle(ModulePath.GetData());
	if (!handle) {		
		LOG_ERROR("Failed to load module %s (%s) : %s", ModuleName.GetData(), ModulePath.GetData(), ReadLastError().GetData());
		return nullptr;
	}

	FEngineModuleFunc ModuleConstructor = (FEngineModuleFunc)GetHandleSymbol(handle, STRINGIFY(MODULE_FUNC_NAME));
	if (!ModuleConstructor) {
		LOG_ASSERT("Failed to load module '%s' : missing MODULE_CONSTRUCTOR.", ModuleName.GetData());
		return nullptr;
	}
	SEngineModuleBase* Module = ModuleConstructor();
	if (!Module) {
		LOG_ASSERT("Failed to load module '%s' : failed to instanciate module.", ModuleName.GetData());
		return nullptr;
	}
	Module->ModuleHandle = handle;
	Module->ModuleName = ModuleName;
	EngineModules[ModuleName] = Module;
	return Module;
}

bool ModuleManager::UnloadModule(const String& ModuleName) {
	auto Module = EngineModules.find(ModuleName);
	if (Module == EngineModules.end()) {
		LOG_ERROR("Failed to find module named %s : module should be loaded first", ModuleName.GetData());
		return false;
	}
	FreeModuleHandle(Module->second->GetHandle());
	delete Module->second;
	EngineModules.erase(ModuleName);
	LOG("Unloaded module %s", ModuleName.GetData());
	return true;
}