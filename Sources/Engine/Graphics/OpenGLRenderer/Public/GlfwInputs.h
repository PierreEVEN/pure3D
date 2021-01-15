#pragma once
#include "InputSystem.h"

class SGlfwInputManager : public IInputManager {
public:

	SGlfwInputManager();

public:

	virtual void Update();

private:

	void BindInputs();
};