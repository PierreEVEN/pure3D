#pragma once
#include "Renderer.h"


class OpenGLRenderer : public IRenderer {

public:
	OpenGLRenderer();

protected:

	virtual void NextFrame() override;



};