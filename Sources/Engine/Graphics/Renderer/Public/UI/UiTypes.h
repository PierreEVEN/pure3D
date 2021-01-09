#pragma once

struct SUIOffsets {

	inline SUIOffsets(float inMinX, float inMinY, float inMaxX, float inMaxY) : MinX(inMinX), MinY(inMinY), MaxX(inMaxX), MaxY(inMaxY) {}


	union {
		struct {
			float OffsetLeft;
			float OffsetTop;
			float OffsetRight;
			float OffsetBottom;
		};
		struct {
			float PercentLeft;
			float PercentTop;
			float PercentRight;
			float PercentBottom;
		};
		struct {
			float MinX;
			float MinY;
			float MaxX;
			float MaxY;
		};
	};
};

