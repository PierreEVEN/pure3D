
#include "ModuleManager.h"
#include "GlfwWindow.h"
#include "OpenGLRenderer.h"
#include "VulkanRenderer.h"
#include "Renderer.h"


MODULE_CONSTRUCTOR() {
	SRenderer EditorRenderer;
	EditorRenderer.CreateWindow<SGlfwOpenGLWindow>(4, 6, 800, 600, true);
	EditorRenderer.CreateRenderer<OpenGLRenderer>(4, 6);
	EditorRenderer.StartRenderer();
}