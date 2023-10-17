#pragma once

struct Color
{
	int R;
	int G;
	int B;
	
	Color() : Color(0, 0, 0)
	{
	}
	
	Color(const int r, const int g, const int b)
	{
		R = r;
		G = g;
		B = b;
	}
};
