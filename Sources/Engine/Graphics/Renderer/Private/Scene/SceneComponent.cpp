
#include "Scene/SceneComponent.h"

void SSceneComponent::AttachTo(SSceneComponent* NewParent, EAttachmentType AttachmentType) {
	if (Parent) {
		Parent->Children.erase(std::find(Parent->Children.begin(), Parent->Children.end(), this));
		if (AttachmentType == EAttachmentType::EAT_KEEP_ABSOLUTE && Parent) {
			SetRelativeTransform(ComponentTransform.GetTransformation() * Parent->GetAbsoluteTransform().GetTransformation());
		}
	}
	
	Parent = NewParent;
	
	if (Parent)	{
		Parent->Children.push_back(this);
		if (AttachmentType == EAttachmentType::EAT_KEEP_ABSOLUTE && Parent) {
			SetRelativeTransform(ComponentTransform.GetTransformation() * Parent->GetAbsoluteTransform().GetTransformation().Inverse());
		}
	}
}

STransform SSceneComponent::GetAbsoluteTransform() {
	if (!Parent) return ComponentTransform;

	return STransform(ComponentTransform.GetTransformation() * Parent->GetAbsoluteTransform().GetTransformation());
}