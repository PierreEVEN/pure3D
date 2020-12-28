#pragma once
#include "Types/String.h"
#include <unordered_map>
#include <functional>
#include "Reflection/ReflectionUtilities.h"


using EngineModuleFunc = std::function<void()>;

struct EngineModuleBase {
	EngineModuleBase(const String& InModuleName, const EngineModuleFunc& InRegisterFunc, const EngineModuleFunc& InUnRegisterFunc)
		: ModuleName(InModuleName), RegisterFunc(InRegisterFunc), UnRegisterFunc(InUnRegisterFunc), IsLoaded(false) {}

	inline bool Load() {
		if (IsLoaded) return false;
		IsLoaded = true;
		RegisterFunc();
		return LoadInternal();
	}

	inline bool UnLoad() {
		if (!IsLoaded) return false;
		IsLoaded = false;
		UnRegisterFunc();
		return UnLoadInternal();
	}

protected:

	virtual bool LoadInternal() { return true; }
	virtual bool UnLoadInternal() { return true; }

private:

	bool IsLoaded;
	const String ModuleName;
	const EngineModuleFunc RegisterFunc;
	const EngineModuleFunc UnRegisterFunc;
};

class ModuleManager {
public:

	static void LoadModuleChecked(const String& ModuleName);
	static void UnloadModuleChecked(const String& ModuleName);
	static bool LoadModule(const String& ModuleName);
	static bool UnloadModule(const String& ModuleName);

protected:

	template<typename ModuleClass = EngineModuleBase>
	inline static void RegisterModule(const String& ModuleName, const EngineModuleFunc& RegisterFunc, const EngineModuleFunc& UnRegisterFunc) {
		if (!ModuleRegisterFunctions) ModuleRegisterFunctions = new std::unordered_map<RUID, EngineModuleBase*>;
		if (UniqueIDAlreadyExist<EngineModuleBase*>(ModuleName, *ModuleRegisterFunctions)) LOG_ASSERT("Cannot register module with given name twice");
		(*ModuleRegisterFunctions)[MakeUniqueID(ModuleName)] = new ModuleClass(ModuleName, RegisterFunc, UnRegisterFunc);
		std::cout << "register module " << ModuleName.GetData() << std::endl;
	}
private:
	inline static std::unordered_map<RUID, EngineModuleBase*>* ModuleRegisterFunctions = nullptr;
};

#define MODULE_CONSTRUCTOR(Name, ...) \
void _Module_Register_Function_##Name(); \
void _Module_UnRegister_Function(); \
struct _Module_Registerer : ModuleManager { \
	_Module_Registerer() { \
		RegisterModule<__VA_ARGS__>(#Name, &_Module_Register_Function_##Name, &_Module_UnRegister_Function); \
	} \
}; \
inline _Module_Registerer _Module_Registerer_Object; \
void _Module_Register_Function_##Name()


#define MODULE_DESTRUCTOR() \
void _Module_UnRegister_Function()
