#pragma once
#include <windows.h>
#include "BlurArea.h"
#include "SynchronousBitmapBlur.h"

class AsyncBitmapBlur
{
public:
	Bitmap Blur(Bitmap& sourceBitmap, const float radius, const int threadsCount) const
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
		HANDLE* handles = new HANDLE[threadsCount];
		for (int i = 0; i < threadsCount; i++)
		{
			handles[i] = CreateThread(NULL, i, &StartThreads, &threadsParamsList[i], CREATE_SUSPENDED, NULL);
		}

		// Start threads
		for (int i = 0; i < threadsCount; i++)
		{
			ResumeThread(handles[i]);
		}

		// Wait for all threads
		WaitForMultipleObjects(threadsCount, handles, true, INFINITE);
        
		delete[] threadsParamsList;
		delete[] handles;

		return result;
	}

private:
	struct BlurBitmapThreadParams
	{
		Bitmap* SourceBitmap;
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

		BlurArea blurArea = *params->BlurArea;
		const float radius = params->Radius;
		const Bitmap sourceBitmap = *params->SourceBitmap;
		Bitmap destinationBitmap = *params->DestinationBitmap;

		blurService.Blur(sourceBitmap, destinationBitmap, radius, blurArea);
		
		ExitThread(0);  
	}
};
