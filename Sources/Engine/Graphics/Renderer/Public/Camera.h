#pragma once
#include "Types/Transform.h"

class SCamera {

public:

	SCamera() : FieldOfView(40), NearClipPlane(0.01f), FarClipPlane(1000), CameraTransform(STransform()) {}

	[[nodiscard]] SMatrix4 GetPerspective(const float AspectRatio) const {
		return Matrix::MakePerspectiveMatrix(FieldOfView, AspectRatio, NearClipPlane, FarClipPlane);
	}

	SMatrix4 GetView() {
		const SVectorDouble ForwardVector(CameraTransform.GetRotation().GetForwardVector());
		const SVector Position(static_cast<float>(CameraTransform.GetLocation().x), static_cast<float>(CameraTransform.GetLocation().y), static_cast<float>(CameraTransform.GetLocation().z));
		return Matrix::MakeLookAtMatrix(
			Position,
			Position + SVector(static_cast<float>(ForwardVector.x), static_cast<float>(ForwardVector.y), static_cast<float>(ForwardVector.z)),
			SVector(0,0,1));
	}

	void SetFieldOfView(const float Fov) { FieldOfView = Fov; }
	void SetNearClipPlane(const float Near) { NearClipPlane = Near; }
	void SetFarClipPlane(const float Far) { FarClipPlane = Far; }

	[[nodiscard]] float GetFieldOfView() const { return FieldOfView; }
	[[nodiscard]] float GetNearClipPlane() const { return FarClipPlane; }
	[[nodiscard]] float GetFarClipPlane() const { return NearClipPlane; }

	STransform& GetCameraTransform() { return CameraTransform; }

private:

	float FieldOfView;
	float NearClipPlane;
	float FarClipPlane;
	STransform CameraTransform;
};