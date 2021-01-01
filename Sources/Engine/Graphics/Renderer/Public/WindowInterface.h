#pragma once
#include "Events/EventManager.h"
#include <unordered_map>
#include "IO/Log.h"


DECLARE_DELEGATE_MULTICAST(DOnResizeFramebuffer, uint32_t, uint32_t);


class IWindow {

public:

	friend class SRenderer;

	IWindow() = delete;
	IWindow(void* InWindowHandle, uint32_t inSizeX = 800, uint32_t inSizeY = 600, bool inFullscreen = false)
		: WindowHandle(InWindowHandle), SizeX(inSizeX), SizeY(inSizeY), IsFullscreen(inFullscreen) {
		WindowHandleMap[InWindowHandle] = this;
	}

	DOnResizeFramebuffer OnResizeFrameBuffer;

	template<typename InterfaceClass = IWindow>
	inline static InterfaceClass* GetWindow(void* Handle) {
		auto FoundElement = WindowHandleMap.find(Handle);
		if (FoundElement == WindowHandleMap.end()) {
			LOG_ERROR("Failed to get window handle");
			return nullptr;
		}
		return static_cast<InterfaceClass*>(FoundElement->second);
	}


	template<typename HandleClass = void>
	inline HandleClass* GetHandle() { return static_cast<HandleClass*>(WindowHandle); }

	virtual bool ShouldClose() = 0;

protected:

	virtual void CaptureInputs() {}
	virtual void NextFrame() = 0;

private:

	inline static std::unordered_map<void*, IWindow*> WindowHandleMap;

	uint32_t SizeX;
	uint32_t SizeY;
	bool IsFullscreen;
	void* WindowHandle;
};
