#pragma once
#include "Vector.h"
#include "Rotator.h"

struct STransform
{
	STransform() : scale3d(1) {}
	STransform(const SVectorDouble& inLocation) : location(inLocation), scale3d(1) {}
	STransform(const SQuatd& inRotation) : rotation(inRotation), scale3d(1) {}
	STransform(const SVectorDouble& inLocation, const SVectorDouble& inScale3d) : location(inLocation), scale3d(inScale3d) {}
	STransform(const SVectorDouble& inLocation, const SQuatd& inRotation) : location(inLocation), rotation(inRotation), scale3d(1) {}
	STransform(const SVectorDouble& inLocation, const SQuatd& inRotation, const SVectorDouble& inScale3d) : location(inLocation), rotation(inRotation), scale3d(inScale3d) {}

	SVectorDouble location;
	SVectorDouble scale3d;
	SQuatd rotation;
};