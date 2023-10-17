#pragma once
#include <windows.h>

#include "Bitmap.h"
#include "../Libs/Math/Math.h"

struct BlurParams
{
	uint32_t startHeight;
	uint32_t endHeight;
	uint32_t startWidth;
	uint32_t endWidth;
};

class SynchronousBitmapBlurMaker
{
public:
	static void Blur(Bitmap& bitmap, int radius, BlurParams& params)
	{
		float rs = ceil(radius * 3);
		float sigma = radius / 2.0;
		float sigmaSquare = sigma * sigma;
		float twoPiSigmaSquare = 2.0 * Math::PI * sigmaSquare;
		
		for (auto i = params.startHeight; i < params.endHeight; ++i)
		{
			for (auto j = params.startWidth; j < params.endWidth; ++j)
			{
				auto width = params.endWidth - params.startWidth;
				auto height = params.endHeight - params.startHeight;

				std::cout << i * width + j << " / " << width * height << std::endl;
				
				double r = 0, g = 0, b = 0;
				double weightSum = 0;

				for (int iy = i - rs; iy < i + rs + 1; ++iy)
				{
					for (int ix = j - rs; ix < j + rs + 1; ++ix)
					{
						auto x = min(static_cast<int>(params.endWidth) - 1, max(0, ix));
						auto y = min(static_cast<int>(params.endHeight) - 1, max(0, iy));

						float distanceSquare = (ix - j) * (ix - j) + (iy - i) * (iy - i);
						float weight = exp(-distanceSquare / (2.0 * sigmaSquare)) / twoPiSigmaSquare;

						Color pixel = bitmap.GetPixel(x, y);

						r += pixel.GetR() * weight;
						g += pixel.GetG() * weight;
						b += pixel.GetB() * weight;
						weightSum += weight;
					}
				}
				
				Color pixel = bitmap.GetPixel(j, i);
				
				pixel.SetR(std::round(r / weightSum));
				pixel.SetG(std::round(g / weightSum));
				pixel.SetB(std::round(b / weightSum));

				bitmap.SetPixel( j, i, pixel );
			}
		}
	}

};
