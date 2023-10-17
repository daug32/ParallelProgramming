#include <string>

#include "Images/Bitmap.h"
#include "Images/SynchronousBitmapBlurMaker.h"
#include "Libs/BmpImageHandler/Bmp.h"

int main()
{
	const std::string path = "D:\\Development\\Projects\\ParallelProgramming\\Lab2\\a.bmp";

	std::cout << "Loading image" << std::endl;
	auto image = Bitmap::FromFile(path);
	
	auto blurParams = BlurParams();
	blurParams.endHeight = image.GetSize().GetHeight();
	blurParams.endWidth = image.GetSize().GetWidth();
	blurParams.startHeight = 0;
	blurParams.startWidth = 0;
	
	std::cout << "Started to blur" << std::endl;
	SynchronousBitmapBlurMaker::Blur(image, 5, blurParams);
	
	std::cout << "Saving" << std::endl;
	image.Save("D:\\Development\\Projects\\ParallelProgramming\\Lab2\\ris2_udpated.bmp");

	std::cout << "Completed" << std::endl;

	return 0;
}
