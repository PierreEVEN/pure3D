#pragma once
#include "WindowInterface.h"

struct GLFWwindow;

class SGlfwWindow : public IWindow {

public:

	SGlfwWindow(void* Handle, uint32_t inSizeX = 800, uint32_t inSizeY = 600, bool inFullscreen = false);
	virtual bool ShouldClose();

protected:

	virtual void CaptureInputs() override;
	virtual void NextFrame() override;

private:

	static void OnResizeFrameBuffer_Internal(GLFWwindow* Handle, int Width, int Height);
};



class SGlfwOpenGLWindow : public SGlfwWindow {
public:
	SGlfwOpenGLWindow(int MajorVersion, int MinorVersion, uint32_t inSizeX = 800, uint32_t inSizeY = 600, bool inFullscreen = false);
protected:

	virtual void* CreateWindowHandle(int MajorVersion, int MinorVersion, uint32_t inSizeX, uint32_t inSizeY, bool inFullscreen);
private:

};

class SGlfwVulkanWindow : public SGlfwWindow {
public:
	SGlfwVulkanWindow(uint32_t inSizeX = 800, uint32_t inSizeY = 600, bool inFullscreen = false);
protected:

	virtual void* CreateWindowHandle(uint32_t inSizeX, uint32_t inSizeY, bool inFullscreen);
};