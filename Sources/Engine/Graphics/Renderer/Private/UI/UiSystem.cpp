#include "UI/UiSystem.h"


SUIOffsets SUIElement::GetDisplayTransform() const {
	if (Parent)	return TransformTo(Parent->GetDisplayTransform());
	else return TransformTo(RenderContext->GetSurfaceTransform());
}

SLinearColor SUIElement::GetDisplayColor() const {
	if (Parent) return Parent->ContentColor * ItemColor;
	return ItemColor;
}

SUIOffsets SUIElement::TransformTo(const SUIOffsets& Parent) const {
	SUIOffsets LocalOffset(
		Anchors.PercentLeft * (Parent.MaxX - Parent.MinX) + Parent.MinX,
		Anchors.PercentTop * (Parent.MaxY - Parent.MinY) + Parent.MinY,
		Anchors.PercentRight * (Parent.MaxX - Parent.MinX) + Parent.MinX,
		Anchors.PercentBottom * (Parent.MaxY - Parent.MinY) + Parent.MinY
	);

	LocalOffset.MinX += Offsets.OffsetLeft;
	LocalOffset.MinY += Offsets.OffsetTop;
	LocalOffset.MaxX += Offsets.OffsetRight;
	LocalOffset.MaxY += Offsets.OffsetBottom;
	return LocalOffset;
}
