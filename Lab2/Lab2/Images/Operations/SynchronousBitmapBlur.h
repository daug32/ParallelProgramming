#pragma once
#include "../Bitmap.h"
#include "../../Libs/Math/Math.h"
#include "BlurArea.h"
#include "../../Libs/ProgressNotificators/ProgressNotificator.h"

class SynchronousBitmapBlur
{
public:
	SynchronousBitmapBlur(bool needToShowProgress = false)
	{
		m_needToShowProgress = needToShowProgress;
	}

	Bitmap Blur(const Bitmap& sourceBitmap, const float radius, BlurArea& blurArea) const
	{
		Bitmap result = Bitmap::CreateEmpty(sourceBitmap.GetSize());
		Blur(sourceBitmap, result, radius, blurArea);
		return result;
	}

	void Blur(const Bitmap& bitmapToRead, Bitmap& result, const float radius, BlurArea& blurArea) const
	{
		if (bitmapToRead.GetSize() != result.GetSize())
		{
			throw std::exception("Result bitmap must be same size as the bitmap to read");
		}

		auto progressNotificator = ProgressNotificator(
			blurArea.Height * blurArea.Width,
			m_needToShowProgress);

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
						const float weight = exp(-GetSquareDistance(centerX, centerY, x, y) / twoSigmaSquare) /
							twoPiSigmaSquare;

						Color pixelToAddToFinalSum = bitmapToRead.GetPixel(
							min(bitmapToRead.GetSize().GetWidth() - 1, max(0, x)),
							min(bitmapToRead.GetSize().GetHeight() - 1, max(0, y)));

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

				result.SetPixel(centerX, centerY, newColor);

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

	bool m_needToShowProgress;
};
