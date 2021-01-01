#pragma once

#include <cstdint>
#include <unordered_map>

class SRenderer final {

public:

	template<typename TWindowInterface, typename ... Args>
	void CreateWindow(Args&&... Arguments) {
		WindowInterface = new TWindowInterface(std::forward<Args>(Arguments)...);
	}

	template<typename TRendererInterface, typename ... Args>
	void CreateRenderer(Args&&... Arguments) {
		RendererInterface = new TRendererInterface(std::forward<Args>(Arguments)...);
	}

	void StartRenderer();

	SRenderer() : WindowInterface(nullptr), RendererInterface(nullptr) {}
	~SRenderer();

private:

	class IWindow* WindowInterface;
	class IRenderer* RendererInterface;
};




