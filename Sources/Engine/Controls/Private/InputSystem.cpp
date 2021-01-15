#include "InputSystem.h"
#include "IO/Log.h"

IInputManager* IInputManager::Instance = nullptr;

IInputManager* IInputManager::Get() {
	return Instance;
}

void IInputManager::SetInstance(IInputManager* InInstance) { 
	if (Instance) delete Instance;
	Instance = InInstance;
}

IInputListener::IInputListener() { IInputManager::Get()->BindObject(this); }
IInputListener::~IInputListener() { IInputManager::Get()->UnbindObject(this); }