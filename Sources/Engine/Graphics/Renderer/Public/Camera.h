#pragma once
#include "Types/Transform.h"

class SCamera {

public:

	SCamera() : CameraTransform(STransform()) {}

	SMatrix4 GetPerspective() {
		return Matrix::MakePerspectiveMatrix(40, 600 / 800.f, 0.01f, 100);
	}

	SMatrix4 GetView() {
		return Matrix::MakeLookAtMatrix(SVector(-2, 0, 0), SVector(0,0,0), SVector(0,0,1));
	}

private:

	STransform CameraTransform;
};