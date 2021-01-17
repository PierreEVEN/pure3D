#pragma once
#include "Vector.h"
#include "Rotator.h"
#include "Matrix.h"

struct STransform
{
public:

	STransform() : Scale3D(1) {}
	STransform(const SVectorDouble& inLocation) : Location(inLocation), Scale3D(1), bIsTransformDirty(true), bAreAxesDirty(false) {}
	STransform(const SMatrix4Double& inTransform) : TransformationMatrix(inTransform), bIsTransformDirty(false), bAreAxesDirty(true) {}
	STransform(const SQuatd& inRotation) : Rotation(inRotation), Scale3D(1), bIsTransformDirty(true), bAreAxesDirty(false) {}
	STransform(const SVectorDouble& inLocation, const SVectorDouble& inScale3d) : Location(inLocation), Scale3D(inScale3d), bIsTransformDirty(true), bAreAxesDirty(false) {}
	STransform(const SVectorDouble& inLocation, const SQuatd& inRotation) : Location(inLocation), Rotation(inRotation), Scale3D(1), bIsTransformDirty(true), bAreAxesDirty(false) {}
	STransform(const SVectorDouble& inLocation, const SQuatd& inRotation, const SVectorDouble& inScale3d) : Location(inLocation), Rotation(inRotation), Scale3D(inScale3d), bIsTransformDirty(true), bAreAxesDirty(false) {}
		
	inline const SMatrix4Double& GetTransformation() {
		TryRebuildTransform();
		return TransformationMatrix;
	}
	inline const SVectorDouble& GetLocation() {
		TryRebuildAxes();
		return Location;
	}
	inline const SQuatd& GetRotation() {
		TryRebuildAxes();
		return Rotation;
	}
	inline const SVectorDouble& GetScale() {
		TryRebuildAxes();
		return Scale3D;
	}

	inline void SetTransformation(const SMatrix4Double& inTransform) {
		TryRebuildAxes();
		TransformationMatrix = inTransform;
		bAreAxesDirty = true;
	}
	inline void SetLocation(const SVectorDouble& inLocation) {
		TryRebuildTransform();
		Location = inLocation;
		bIsTransformDirty = true;
	}
	inline void SetRotation(const SQuatd& inRotation) {
		TryRebuildTransform();
		Rotation = inRotation;
		bIsTransformDirty = true;
	}
	inline void SetScale(const SVectorDouble& inScale3D) {
		TryRebuildTransform();
		Scale3D = inScale3D;
		bIsTransformDirty = true;
	}

private:

	inline void TryRebuildTransform() {
		if (bIsTransformDirty) {
			TransformationMatrix = SMatrix4Double(Location, Rotation, Scale3D);
			bIsTransformDirty = false;
		}
	}

	inline void TryRebuildAxes() {
		if (bAreAxesDirty) {
			SVectorDouble Skew;
			SVector4Double Perspective;
			TransformationMatrix.Decompose(Scale3D, Rotation, Location, Skew, Perspective);
			bAreAxesDirty = false;
		}
	}

	bool bIsTransformDirty;
	bool bAreAxesDirty;

	SMatrix4Double TransformationMatrix;

	SVectorDouble Location;
	SQuatd Rotation;
	SVectorDouble Scale3D;
};