#pragma once
#include <vector>
#include "IO/Log.h"

struct IInputListener {
	friend class IInputManager;
public:
	IInputListener();
	virtual ~IInputListener();

protected:
	virtual void Update() = 0;
	virtual void KeyCallback(int keycode, int scancode, int action, int mods) = 0;
	virtual void CharCallback(int chr) = 0;
	virtual void MouseButtonCallback(int button, int action, int mods) = 0;
	virtual void ScrollCallback(double xOffset, double yOffset) = 0;
	virtual void CursorPosCallback(double x, double y) = 0;
};

class IInputManager {

	friend struct IInputListener;
public:

	IInputManager() = default;

	template<typename Class>
	inline static void Create() { SetInstance(new Class()); }

	static IInputManager* Get();

	inline virtual void Update() { for (int64_t i = Listeners.size() - 1; i >= 0; --i) Listeners[i]->Update(); }
	inline void KeyCallback(int KeyCode, int ScanCode, int Action, int Modifiers) { for (int64_t i = Listeners.size() - 1; i >= 0; --i) Listeners[i]->KeyCallback(KeyCode, ScanCode, Action, Modifiers); }
	inline void CharCallback(int CharValue) { for (int64_t i = Listeners.size() - 1; i >= 0; --i) Listeners[i]->CharCallback(CharValue); }
	inline void MouseButtonCallback(int ButtonID, int Action, int Modifiers) { for (int64_t i = Listeners.size() - 1; i >= 0; --i) Listeners[i]->MouseButtonCallback(ButtonID, Action, Modifiers); }
	inline void ScrollCallback(double Xscroll, double Yscroll) { for (int64_t i = Listeners.size() - 1; i >= 0; --i) Listeners[i]->ScrollCallback(Xscroll, Yscroll); }
	inline void CursorPosCallback(double XPosition, double YPosition) { for (int64_t i = Listeners.size() - 1; i >= 0; --i) Listeners[i]->CursorPosCallback(XPosition, YPosition); }

private:

	inline void BindObject(IInputListener* Object) { Listeners.push_back(Object); }
	inline void UnbindObject(IInputListener* Object) { Listeners.erase(std::find(Listeners.begin(), Listeners.end(), Object)); }
	
	static void SetInstance(IInputManager* InInstance);

	std::vector<IInputListener*> Listeners;
	static IInputManager* Instance;
};

