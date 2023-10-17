#pragma once
#include "../Bitmap.h"
#include "../../Libs/Math/Math.h"
#include "BlurArea.h"
#include "../../Libs/ProgressNotificators/ProgressNotificator.h"

class SynchronousBitmapBlur
{
public:	
	static void Blur(Bitmap& bitmap, float radius, BlurArea& blurArea)
	{
		auto progressNotificator = ProgressNotificator(blurArea.Height * blurArea.Width);
		
		const float sigma = radius / 2.0f;
		const float twoSigmaSquare = 2.0f * sigma * sigma;
		const float twoPiSigmaSquare = Math::PI * twoSigmaSquare;
		
		for (int centerY = blurArea.StartHeight; centerY < blurArea.EndHeight; centerY++)
		{
			for (int centerX = blurArea.StartWidth; centerX < blurArea.EndWidth; centerX++)
			{
				float gaussianCoefficient = 0;
				float r = 0, g = 0, b = 0;

				for (int y = centerY - radius; y < centerY + radius + 1; ++y)
				{
					for (int x = centerX - radius; x < centerX + radius + 1; ++x)
					{
						const float weight = exp( -GetSquareDistance(centerX, centerY, x, y) / twoSigmaSquare ) / twoPiSigmaSquare;

						Color pixelToAddToFinalSum = bitmap.UnsafeGetPixel(
							std::min(blurArea.EndWidth - 1, std::max(0, x)),
							std::min(blurArea.EndHeight - 1, std::max(0, y)) );

						r += pixelToAddToFinalSum.GetR() * weight;
						g += pixelToAddToFinalSum.GetG() * weight;
						b += pixelToAddToFinalSum.GetB() * weight;
						
						gaussianCoefficient += weight;
					}
				}

				auto newColor = Color(
					r / gaussianCoefficient,
					g / gaussianCoefficient,
					b / gaussianCoefficient);
				
				bitmap.SetPixel(centerX, centerY, newColor);
				
				progressNotificator.Update(centerY * blurArea.Width + centerX);
			}
		}
	}

private:
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
