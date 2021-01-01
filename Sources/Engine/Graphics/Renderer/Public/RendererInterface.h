#pragma once

class IRenderer {
public:

	friend class SRenderer;

protected:


	virtual void NextFrame() = 0;
private:

};
