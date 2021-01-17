#pragma once

#include "Types/Transform.h"
#include "Scene/SceneComponent.refl.h"

enum class EAttachmentType {
	EAT_KEEP_ABSOLUTE,
	EAT_KEEP_RELATIVE
};


REFLECT()
class SSceneComponent {
	REFLECT_BODY()

public:

	STransform GetAbsoluteTransform();
	[[nodiscard]] STransform GetRelativeTransform() const { return ComponentTransform; }
	virtual void SetRelativeTransform(const STransform& inTransform) { ComponentTransform = inTransform; }

	[[nodiscard]] SSceneComponent* GetParent() const { return Parent; }
	[[nodiscard]] std::vector<SSceneComponent*> GetChildren() const { return Children; }
	
	void AttachTo(SSceneComponent* NewParent, EAttachmentType AttachmentType = EAttachmentType::EAT_KEEP_ABSOLUTE);

	virtual void OnTransformationChanged() {}

private:

	RPROPERTY()
	STransform ComponentTransform;

	RPROPERTY()
	SSceneComponent* Parent = nullptr;

	RPROPERTY()
	std::vector<SSceneComponent*> Children = {};
};