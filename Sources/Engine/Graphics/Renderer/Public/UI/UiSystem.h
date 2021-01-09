#pragma once
#include <vector>
#include "Types/Color.h"
#include "UiTypes.h"


class SUISurface {
public:
	const SUIOffsets& GetSurfaceTransform() { return SurfaceTransform; }
private:
	SUIOffsets SurfaceTransform;
};


class SUIElement {
	friend class SRenderer;
public:

	SUIOffsets GetDisplayTransform() const;
	SLinearColor GetDisplayColor() const;
	SLinearColor GetContentColor() const { return ContentColor; }
	SUIOffsets TransformTo(const SUIOffsets& Parent) const;

private:
	SUISurface* RenderContext = nullptr;
	SUIElement* Parent = nullptr;
	std::vector<SUIElement*> Children;

	SLinearColor ItemColor = SLinearColor::WHITE;
	SLinearColor ContentColor = SLinearColor::WHITE;

	SUIOffsets Anchors;
	SUIOffsets Offsets;
};
