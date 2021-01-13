#pragma once

#include "Types/Transform.h"
#include "Scene/SceneComponent.refl.h"

REFLECT()
class SSceneComponent {
	REFLECT_BODY()

public:

	STransform GetAbsoluteTransform() { return STransform(); }

	inline STransform GetRelativeTransform() { return ComponentTransform; }
	inline virtual void SetRelativeTransform(const STransform& inTransform) { ComponentTransform = inTransform; }

private:

	RPROPERTY()
	STransform ComponentTransform;

	SSceneComponent* Parent;
	std::vector<SSceneComponent*> Childrens;
};