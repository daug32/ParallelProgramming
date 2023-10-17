#pragma once

#include "../Bitmap.h"

struct BlurArea
{
	int StartHeight;
	int EndHeight;
	int StartWidth;
	int EndWidth;

	BlurArea(const int startWidth, const int endWidth, const int startHeight, const int endHeight)
		: StartHeight(startHeight), EndHeight(endHeight), StartWidth(startWidth), EndWidth(endWidth)
	{
	}

	static BlurArea AllImage(const Bitmap& bitmap)
	{
		const auto size = bitmap.GetSize();
		return BlurArea(0, size.GetWidth(), 0, size.GetHeight());
	}
};