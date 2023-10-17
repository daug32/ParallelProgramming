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
		float twoSigmaSquare = 2.0 * sigma * sigma;
		float twoPiSigmaSquare = Math::PI * twoSigmaSquare;

		for (auto i = params.startHeight; i < params.endHeight; ++i)
		{
			for (auto j = params.startWidth; j < params.endWidth; ++j)
			{
				if ( i % 10 == 0 )
				{
					auto width = params.endWidth - params.startWidth;
					auto height = params.endHeight - params.startHeight;
					std::cout << i * width + j << " / " << width * height << std::endl;
				}

				double r = 0, g = 0, b = 0;
				double weightSum = 0;

				for (int iy = i - rs; iy < i + rs + 1; ++iy)
				{
					for (int ix = j - rs; ix < j + rs + 1; ++ix)
					{
						float weight = exp( -GetSquareDistance(j, i, ix, iy) / twoSigmaSquare ) / twoPiSigmaSquare;
						
						Color pixel = bitmap.GetPixel(
							min(params.endWidth - 1, max(0, ix)),
							min(params.endHeight - 1, max(0, iy)));

						r += pixel.GetR() * weight;
						g += pixel.GetG() * weight;
						b += pixel.GetB() * weight;
						
						weightSum += weight;
					}
				}

				Color newPixel = bitmap.GetPixel(j, i);

				newPixel.SetR(std::round(r / weightSum));
				newPixel.SetG(std::round(g / weightSum));
				newPixel.SetB(std::round(b / weightSum));

				bitmap.SetPixel(j, i, newPixel);
			}
		}
	}

private:
	inline static double GetSquareDistance(double x0, double y0, double x1, double y1) 
	{
		auto dX = x1 - x0;
		auto dY = y1 - y0;
		return dX * dX + dY * dY;
	}
};
