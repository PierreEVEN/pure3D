#include "ModuleManager.h"

int main(int argc, char* argv[]) {

	ModuleManager::LoadModule("AssetManager");
	ModuleManager::UnloadModule("AssetManager");

}