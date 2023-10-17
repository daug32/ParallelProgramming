#pragma once
#include <windows.h>
#include "BlurArea.h"
#include "SynchronousBitmapBlur.h"

class AsyncBitmapBlur
{
public:
	AsyncBitmapBlur(bool useProgressNotification = false)
	{
		m_useProgressNotification = useProgressNotification;
	}
	
	void Blur(Bitmap& bitmap, const float radius, const int threadsCount) const
	{
		ProgressNotificator* progressNotificator = nullptr;
		if ( m_useProgressNotification )
		{
			progressNotificator = new ProgressNotificator(threadsCount);
		}
		
		// Create threads params
		const auto threadsParamsList = new BlurBitmapThreadParams[threadsCount];
		for (int i = 0; i < threadsCount; i++)
		{
			const auto area = BuildBlurAreaForThread(bitmap.GetSize(), i, threadsCount);
			
			auto threadParams = BlurBitmapThreadParams();
			
			threadParams.Bitmap = &bitmap;
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
		for ( int threadIndex = 0; threadIndex < threadsCount; threadIndex++)
		{
			WaitForMultipleObjects(1, handles + threadIndex, true, INFINITE);
			
			if ( m_useProgressNotification )
			{
				progressNotificator->Update(threadIndex + 1);
			}
		}
        
		delete[] threadsParamsList;
		delete[] handles;
	}

private:
	struct BlurBitmapThreadParams
	{
		Bitmap* Bitmap;
		float Radius;
		BlurArea* BlurArea;
	};

	bool m_useProgressNotification;

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
		Bitmap bitmap = *params->Bitmap;

		blurService.Blur(bitmap, radius, blurArea);
		
		ExitThread(0);  
	}
};
