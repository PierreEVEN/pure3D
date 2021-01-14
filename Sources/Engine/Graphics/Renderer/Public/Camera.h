#pragma once
#include "Types/Transform.h"

class SCamera {

public:

	SCamera() : CameraTransform(STransform()), FieldOfView(40), NearClipPlane(0.01f), FarClipPlane(1000) {}

	SMatrix4 GetPerspective(float AspectRatio) {
		return Matrix::MakePerspectiveMatrix(FieldOfView, AspectRatio, NearClipPlane, FarClipPlane);
	}

	SMatrix4 GetView() {
		SVectorDouble ForwardVector(CameraTransform.GetRotation().GetForwardVector());
		SVector Position((float)CameraTransform.GetLocation().x, (float)CameraTransform.GetLocation().y, (float)CameraTransform.GetLocation().z);
		return Matrix::MakeLookAtMatrix(
			Position,
			Position + SVector((float)ForwardVector.x, (float)ForwardVector.y, (float)ForwardVector.z),
			SVector(0,0,1));
	}

	inline void SetFieldOfView(float FOV) { FieldOfView = FOV; }
	inline void SetNearClipPlane(float Near) { NearClipPlane = Near; }
	inline void SetFarClipPlane(float Far) { FarClipPlane = Far; }

	inline float GetFieldOfView() const { return FieldOfView; }
	inline float GetNearClipPlane() const { return FarClipPlane; }
	inline float GetFarClipPlane() const { return NearClipPlane; }

	inline STransform& GetCameraTransform() { return CameraTransform; }

private:

	float FieldOfView;
	float NearClipPlane;
	float FarClipPlane;
	STransform CameraTransform;
};