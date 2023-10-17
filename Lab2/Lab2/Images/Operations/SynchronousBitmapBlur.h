#pragma once
#include <windows.h>

#include "../Bitmap.h"
#include "../../Libs/Math/Math.h"
#include "BlurArea.h"

class SynchronousBitmapBlur
{
public:	
	static void Blur(Bitmap& bitmap, float radius, BlurArea& params)
	{
		const float sigma = radius / 2.0f;
		const float twoSigmaSquare = 2.0f * sigma * sigma;
		const float twoPiSigmaSquare = Math::PI * twoSigmaSquare;

		for (auto y = params.StartHeight; y < params.EndHeight; ++y)
		{
			for (auto x = params.StartWidth; x < params.EndWidth; ++x)
			{
				Color currentPixel = bitmap.GetPixel(x, y);
				
				float gaussianCoefficient = 0;
				float r = 0, g = 0, b = 0;

				for (int iy = y - radius; iy < y + radius + 1; ++iy)
				{
					for (int ix = x - radius; ix < x + radius + 1; ++ix)
					{
						float weight = exp( -GetSquareDistance(x, y, ix, iy) / twoSigmaSquare ) / twoPiSigmaSquare;
						
						Color pixel = bitmap.GetPixel(
							min(params.EndWidth - 1, max(0, ix)),
							min(params.EndHeight - 1, max(0, iy)));

						r += pixel.GetR() * weight;
						g += pixel.GetG() * weight;
						b += pixel.GetB() * weight;
						
						gaussianCoefficient += weight;
					}
				}

				currentPixel.SetR(r / gaussianCoefficient);
				currentPixel.SetG(g / gaussianCoefficient);
				currentPixel.SetB(b / gaussianCoefficient);

				bitmap.SetPixel(x, y, currentPixel);
				
				LogProcessingInfo(x, y, params);
			}
		}
	}

private:
	inline static void LogProcessingInfo(uint32_t x, uint32_t y, BlurArea& params)
	{
		if ( y % 10 == 0 )
		{
			auto width = params.EndWidth - params.StartWidth;
			auto height = params.EndHeight - params.StartHeight;
			std::cout << y * width + x << " / " << width * height << std::endl;
		}
	}
	
	inline static float GetSquareDistance(
		const float x0,
		const float y0,
		const float x1,
		const float y1) 
	{
		auto dX = x1 - x0;
		auto dY = y1 - y0;
		return dX * dX + dY * dY;
	}
};
