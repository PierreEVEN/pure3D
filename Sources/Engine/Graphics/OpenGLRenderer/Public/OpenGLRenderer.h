#pragma once
#include "RendererInterface.h"


class OpenGLRenderer : public IRenderer {

public:
	OpenGLRenderer(int MajorVersion, int MinorVersion);

protected:

	virtual void NextFrame() override;



};