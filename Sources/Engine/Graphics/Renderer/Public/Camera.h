#pragma once
#include "Types/Transform.h"

class SCamera {

public:

	SCamera() : CameraTransform(STransform()) {}



private:

	STransform CameraTransform;
};