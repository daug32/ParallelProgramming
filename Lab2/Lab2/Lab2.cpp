#include <string>

#include "Images/Bitmap.h"
#include "Images/Operations/AsyncBitmapBlur.h"
#include "Images/Operations/SynchronousBitmapBlur.h"

int main()
{
	std::cout << "Loading image" << std::endl;
	auto sourceImage = Bitmap::FromFile("D:\\Development\\Projects\\ParallelProgramming\\Lab2\\a.bmp");
	
	std::cout << "Started to blur" << std::endl;
	const auto blurService = AsyncBitmapBlur();
	auto result = blurService.Blur(sourceImage, 5, 10);
	
	std::cout << "Saving" << std::endl;
	result.Save("D:\\Development\\Projects\\ParallelProgramming\\Lab2\\ris2_udpated.bmp");

	std::cout << "Completed" << std::endl;

	return 0;
}
