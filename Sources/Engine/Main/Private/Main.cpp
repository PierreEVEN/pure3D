#include "ModuleManager.h"

int main(int argc, char* argv[]) {
	ModuleManager::LoadModule("Editor");
	ModuleManager::UnloadModule("Editor");
}