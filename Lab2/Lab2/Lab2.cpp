#include "Images/Bitmap.h"
#include "Images/Operations/AsyncBitmapBlur.h"
#include "Images/Operations/SynchronousBitmapBlur.h"

int main()
{
	std::cout << "Loading image" << std::endl;
	auto image = Bitmap::FromFile("D:\\Development\\Projects\\ParallelProgramming\\Lab2\\a.bmp");
	
	std::cout << "Started to blur" << std::endl;
	const auto blurService = AsyncBitmapBlur();
	blurService.Blur(image, 5, 64);
	
	std::cout << "Saving" << std::endl;
	image.Save("D:\\Development\\Projects\\ParallelProgramming\\Lab2\\ris2_udpated.bmp");

	std::cout << "Completed" << std::endl;

	return 0;
}
