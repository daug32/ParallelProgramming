#include <string>

#include "Images/BmpImage.h"
#include "Libs/BmpImageHandler/Bmp.h"

int main()
{
	const std::string path = "D:\\Development\\Projects\\ParallelProgramming\\Lab2\\ris2.bmp";

	auto image = BmpImage::FromFile(path);
	
	const auto color = Color(255, 255, 255);
	for ( int x = 0; x < image.GetSize().GetWidth(); x++ )
	{
		for ( int y = 0; y < image.GetSize().GetHeight(); y++ )
		{
			image.SetPixel(x, y, color);		
		}
	}

	image.Save("D:\\Development\\Projects\\ParallelProgramming\\Lab2\\ris2_udpated.bmp");

	return 0;
}
