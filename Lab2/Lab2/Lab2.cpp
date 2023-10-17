#include "Images/Bitmap.h"
#include "Images/Operations/AsyncBitmapBlur.h"

int main()
{
	std::cout << "Started" << std::endl;
	constexpr auto blurService = AsyncBitmapBlur();
	
	std::cout << "Loading image" << std::endl;
	const auto sourceImage = Bitmap::FromFile("D:\\Development\\Projects\\ParallelProgramming\\Lab2\\image.bmp");

	std::cout << "Started to blur" << std::endl;
	const Bitmap result = blurService.Blur(sourceImage, 10, 24);

	std::cout << "Saving" << std::endl;
	result.Save("D:\\Development\\Projects\\ParallelProgramming\\Lab2\\image_blured.bmp");

	std::cout << "Completed" << std::endl;

	return 0;
}
