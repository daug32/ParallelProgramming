#include <string>

#include "Images/Bitmap.h"
#include "Images/Operations/AsyncBitmapBlur.h"
#include "Images/Operations/SynchronousBitmapBlur.h"

int main()
{
	const std::string path = "D:\\Development\\Projects\\ParallelProgramming\\Lab2\\a.bmp";
	const auto blurService = AsyncBitmapBlur(true);

	std::cout << "Loading image" << std::endl;
	auto image = Bitmap::FromFile(path);
	
	std::cout << "Started to blur" << std::endl;
	blurService.Blur(image, 5, 10);
	
	std::cout << "Saving" << std::endl;
	image.Save("D:\\Development\\Projects\\ParallelProgramming\\Lab2\\ris2_udpated.bmp");

	std::cout << "Completed" << std::endl;

	return 0;
}
