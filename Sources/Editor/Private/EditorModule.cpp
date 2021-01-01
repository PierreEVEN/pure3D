
#include "ModuleManager.h"
#include "GlfwWindow.h"
#include "OpenGLRenderer.h"
#include "VulkanRenderer.h"


MODULE_CONSTRUCTOR() {
	SRenderer EditorRenderer;
	EditorRenderer.CreateWindow<SGlfwOpenGLWindow>(800, 600, true);
	EditorRenderer.CreateRenderer<OpenGLRenderer>();
	EditorRenderer.StartRenderer();
}