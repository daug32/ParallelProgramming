#include <string>

#include "Images/BmpImage.h"
#include "Libs/BmpImageHandler/Bmp.h"

int main()
{
	const std::string path = "D:\\Development\\Projects\\ParallelProgramming\\Lab2\\ris2.bmp";

	auto image = BmpImage::FromFile(path);
	
	const auto color = Color(255, 0, 0);
	for ( int i = 0; i < image.GetSize().GetWidth() / 2; i++ )
	{
		image.SetPixel(i, 0, color);
		image.SetPixel(i, 1, color);
		image.SetPixel(i, 2, color);
	}

	image.Save("D:\\Development\\Projects\\ParallelProgramming\\Lab2\\ris2_udpated.bmp");

	return 0;
}
