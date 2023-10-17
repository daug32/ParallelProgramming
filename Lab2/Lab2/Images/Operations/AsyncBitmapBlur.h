#pragma once
#include <windows.h>
#include "BlurArea.h"
#include "SynchronousBitmapBlur.h"

class AsyncBitmapBlur
{
public:
	Bitmap Blur(const Bitmap& sourceBitmap, const float radius, const int threadsCount) const
	{
		Bitmap result = Bitmap::CreateEmpty(sourceBitmap.GetSize());

		// Create threads params
		const auto threadsParamsList = new BlurBitmapThreadParams[threadsCount];
		for (int i = 0; i < threadsCount; i++)
		{
			const auto area = BuildBlurAreaForThread(sourceBitmap.GetSize(), i, threadsCount);

			auto threadParams = BlurBitmapThreadParams();

			threadParams.SourceBitmap = &sourceBitmap;
			threadParams.DestinationBitmap = &result;
			threadParams.BlurArea = new BlurArea(area);
			threadParams.Radius = radius;

			threadsParamsList[i] = threadParams;
		}

		// Create thread objects
		auto threadObjects = new HANDLE[threadsCount];
		for (int i = 0; i < threadsCount; i++)
		{
			threadObjects[i] = CreateThread(NULL, i, &StartThreads, &threadsParamsList[i], CREATE_SUSPENDED, NULL);
		}

		// Start threads
		for (int i = 0; i < threadsCount; i++)
		{
			ResumeThread(threadObjects[i]);
		}

		// Wait for all threads
		WaitForMultipleObjects(threadsCount, threadObjects, true, INFINITE);

		delete[] threadsParamsList;
		delete[] threadObjects;

		return result;
	}

private:
	struct BlurBitmapThreadParams
	{
		const Bitmap* SourceBitmap;
		Bitmap* DestinationBitmap;

		float Radius;
		BlurArea* BlurArea;
	};

	static BlurArea BuildBlurAreaForThread(
		const Size& bitmapSize,
		const int currentThreadIndex,
		const int threadsCount)
	{
		const int heightPerThread = bitmapSize.GetHeight() / threadsCount;

		const int startHeight = heightPerThread * currentThreadIndex;
		const int endHeight = currentThreadIndex == threadsCount - 1
			                      ? bitmapSize.GetHeight()
			                      : startHeight + heightPerThread;

		return BlurArea(0, bitmapSize.GetWidth(), startHeight, endHeight);
	}

	static DWORD WINAPI StartThreads(CONST LPVOID lpParam)
	{
		const auto blurService = SynchronousBitmapBlur(false);

		const auto params = static_cast<struct BlurBitmapThreadParams*>(lpParam);

		blurService.Blur(
			*params->SourceBitmap,
			*params->DestinationBitmap,
			params->Radius,
			*params->BlurArea);

		ExitThread(0);
	}
};
