
#include "EngineModule.h"

int main() {


	ModuleManager::LoadModuleChecked("AssetManager");

	ModuleManager::UnloadModuleChecked("AssetManager");

}