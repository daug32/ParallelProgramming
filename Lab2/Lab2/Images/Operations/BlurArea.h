#pragma once

#include "../Bitmap.h"

struct BlurArea
{
	int StartHeight;
	int EndHeight;
	int Height;
	
	int StartWidth;
	int EndWidth;
	int Width;

	BlurArea(const int startWidth, const int endWidth, const int startHeight, const int endHeight)
	{
		if ( startHeight < 0 || startWidth < 0)
		{
			throw std::exception("Start point can't be less than 0");
		}

		if ( endHeight < startHeight || endWidth < startWidth )
		{
			throw std::exception("End point can't be less than start point");
		}
		
		StartHeight = startHeight;
		EndHeight = endHeight;
		Height = endHeight - startHeight;
		
		StartWidth = startWidth;
		EndWidth = endWidth;
		Width = endWidth - startWidth;
	}

	static BlurArea AllImage(const Bitmap& bitmap)
	{
		const auto size = bitmap.GetSize();
		return BlurArea(0, size.GetWidth(), 0, size.GetHeight());
	}
};