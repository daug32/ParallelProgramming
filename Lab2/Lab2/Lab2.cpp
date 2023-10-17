#include <string>

#include "Images/Bitmap.h"
#include "Images/Operations/SynchronousBitmapBlur.h"

int main()
{
	const std::string path = "D:\\Development\\Projects\\ParallelProgramming\\Lab2\\a.bmp";

	std::cout << "Loading image" << std::endl;
	auto image = Bitmap::FromFile(path);
	
	std::cout << "Started to blur" << std::endl;
	auto blurParams = BlurArea::AllImage(image);
	SynchronousBitmapBlur::Blur(image, 5, blurParams);
	
	std::cout << "Saving" << std::endl;
	image.Save("D:\\Development\\Projects\\ParallelProgramming\\Lab2\\ris2_udpated.bmp");

	std::cout << "Completed" << std::endl;

	return 0;
}
