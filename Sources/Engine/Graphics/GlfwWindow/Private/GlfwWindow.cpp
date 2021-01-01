
#include "GlfwWindow.h"
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "IO/Log.h"


void GlfwErrorCallback(int error, const char* description) {
	LOG_ASSERT("glfw Error (%d) : %s", error, description);
}

SGlfwWindow::SGlfwWindow(void* Handle, uint32_t inSizeX /*= 800*/, uint32_t inSizeY /*= 600*/, bool inFullscreen /*= false*/)
	: IWindow(Handle, inSizeX, inSizeY, inFullscreen) {
	glfwSetFramebufferSizeCallback(GetHandle<GLFWwindow>(), &SGlfwOpenGLWindow::OnResizeFrameBuffer_Internal);
	glfwSetErrorCallback(GlfwErrorCallback);
// 	glfwSetKeyCallback(GetHandle<GLFWwindow>(), glfwKeyCallback);
// 	glfwSetMouseButtonCallback(GetHandle<GLFWwindow>(), glfwMouseButtonCallback);
// 	glfwSetCursorPosCallback(GetHandle<GLFWwindow>(), glfwCursorPosCallback);
}

void SGlfwWindow::CaptureInputs() {
	glfwPollEvents();
}

void SGlfwWindow::NextFrame() {
	glfwSwapBuffers(GetHandle<GLFWwindow>());
}

bool SGlfwWindow::ShouldClose() {
	return glfwWindowShouldClose(GetHandle<GLFWwindow>());
}


void SGlfwWindow::OnResizeFrameBuffer_Internal(GLFWwindow* Handle, int Width, int Height) {
	GetWindow<SGlfwOpenGLWindow>(Handle)->OnResizeFrameBuffer.Execute(static_cast<uint32_t>(Width), static_cast<uint32_t>(Height));
}

/************************************************************************/
/* OpenGL                                                               */
/************************************************************************/
SGlfwOpenGLWindow::SGlfwOpenGLWindow(int MajorVersion, int MinorVersion, uint32_t inSizeX /*= 800*/, uint32_t inSizeY /*= 600*/, bool inFullscreen /*= false*/)
	: SGlfwWindow(CreateWindowHandle(MajorVersion, MinorVersion, inSizeX, inSizeY, inFullscreen), inSizeX, inSizeY, inFullscreen) {}

void* SGlfwOpenGLWindow::CreateWindowHandle(int MajorVersion, int MinorVersion, uint32_t inSizeX, uint32_t inSizeY, bool inFullscreen) {
	LOG("Initializing GLFW for OpenGL");
	if (!glfwInit()) LOG_ASSERT("Failed to create glfw window");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, inFullscreen ? GLFW_TRUE : GLFW_FALSE);
	
	
	GLFWwindow* WindowHandle = glfwCreateWindow(inSizeX, inSizeY, "test", 0, 0);
	if (!WindowHandle) LOG_ASSERT("Failed to create Glfw window handle");
	glfwMakeContextCurrent(WindowHandle);
	return WindowHandle;
}


/************************************************************************/
/* VUlkan                                                               */
/************************************************************************/

SGlfwVulkanWindow::SGlfwVulkanWindow(uint32_t inSizeX /*= 800*/, uint32_t inSizeY /*= 600*/, bool inFullscreen /*= false*/)
	: SGlfwWindow(CreateWindowHandle(inSizeX, inSizeY, inFullscreen), inSizeX, inSizeY, inFullscreen) {}

void* SGlfwVulkanWindow::CreateWindowHandle(uint32_t inSizeX, uint32_t inSizeY, bool inFullscreen) {
	LOG("Initializing GLFW for Vulkan");
	if (!glfwInit()) LOG_ASSERT("Failed to create glfw window");
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	GLFWwindow* WindowHandle = glfwCreateWindow(inSizeX, inSizeY, "test", 0, 0);
	if (!WindowHandle) LOG_ASSERT("Failed to create Glfw window handle");
	glfwMakeContextCurrent(WindowHandle);
	return WindowHandle;
}
