#include "GlfwInputs.h"
#include "OpenGLRenderApi.h"

SGlfwInputManager::SGlfwInputManager() {
	BindInputs();
}


void SGlfwInputManager::Update() {
	glfwPollEvents();
	IInputManager::Update();
}

void SGlfwInputManager::BindInputs() {
	GLFWwindow* Handle = ((SOpenGlRenderApi*)IRendererApi::Get())->WindowHandle;
	glfwSetKeyCallback(Handle, [](GLFWwindow* Handle, int keycode, int scancode, int action, int mods) -> void { IInputManager::Get()->KeyCallback(keycode, scancode, action, mods); });		
	glfwSetCharCallback(Handle, [](GLFWwindow* Handle, unsigned int CharValue) -> void { IInputManager::Get()->CharCallback(CharValue); });
	glfwSetMouseButtonCallback(Handle, [](GLFWwindow* Handle, int ButtonID, int Action, int Modifiers) -> void { IInputManager::Get()->MouseButtonCallback(ButtonID, Action, Modifiers); });
	glfwSetScrollCallback(Handle, [](GLFWwindow* Handle, double Xscroll, double Yscroll) -> void { IInputManager::Get()->ScrollCallback(Xscroll, Yscroll); });
	glfwSetCursorPosCallback(Handle, [](GLFWwindow* Handle, double XPosition, double YPosition) -> void { IInputManager::Get()->CursorPosCallback(XPosition, YPosition); });	
}

