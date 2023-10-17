#pragma once
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>

#pragma pack(push, 1)
struct BMPFileHeader
{
	uint16_t file_type{ 0x4D42 }; // File type always BM which is 0x4D42 (stored as hex uint16_t in little endian)
	uint32_t file_size{ 0 }; // Size of the file (in bytes)
	uint16_t reserved1{ 0 }; // Reserved, always 0
	uint16_t reserved2{ 0 }; // Reserved, always 0
	uint32_t offset_data{ 0 }; // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader
{
	uint32_t size{ 0 }; // Size of this header (in bytes)
	int32_t width{ 0 }; // width of bitmap in pixels
	int32_t height{ 0 }; // width of bitmap in pixels
	//       (if positive, bottom-up, with origin in lower left corner)
	//       (if negative, top-down, with origin in upper left corner)
	uint16_t planes{ 1 }; // No. of planes for the target device, this is always 1
	uint16_t bit_count{ 0 }; // No. of bits per pixel
	uint32_t compression{ 0 }; // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
	uint32_t size_image{ 0 }; // 0 - for uncompressed images
	int32_t x_pixels_per_meter{ 0 };
	int32_t y_pixels_per_meter{ 0 };
	uint32_t colors_used{ 0 };
	// No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
	uint32_t colors_important{ 0 }; // No. of colors used for displaying the bitmap. If 0 all colors are required
};

struct BMPColorHeader
{
	uint32_t red_mask{ 0x00ff0000 }; // Bit mask for the red channel
	uint32_t green_mask{ 0x0000ff00 }; // Bit mask for the green channel
	uint32_t blue_mask{ 0x000000ff }; // Bit mask for the blue channel
	uint32_t alpha_mask{ 0xff000000 }; // Bit mask for the alpha channel
	uint32_t color_space_type{ 0x73524742 }; // Default "sRGB" (0x73524742)
	uint32_t unused[16]{ 0 }; // Unused data for sRGB color space
};
#pragma pack(pop)

struct BMP
{
	BMPFileHeader FileHeader;
	BMPInfoHeader BmpInfoHeader;
	BMPColorHeader BmpColorHeader;
	std::vector<uint8_t> Data;

	explicit BMP(const char* filePath)
	{
		Read(filePath);
	}

	void Read(const char* filePath)
	{
		std::ifstream inp{ filePath, std::ios_base::binary };
		if (!inp)
		{
			throw std::runtime_error("Unable to open the input image file.");
		}
		
		inp.read(reinterpret_cast<char*>(&FileHeader), sizeof FileHeader);
		if (FileHeader.file_type != 0x4D42)
		{
			throw std::runtime_error("Error! Unrecognized file format.");
		}
		
		inp.read(reinterpret_cast<char*>(&BmpInfoHeader), sizeof BmpInfoHeader);

		// The BMPColorHeader is used only for transparent images
		if (BmpInfoHeader.bit_count == 32)
		{
			// Check if the file has bit mask color information
			if (BmpInfoHeader.size < sizeof(BMPInfoHeader) + sizeof(BMPColorHeader))
			{
				std::cerr
					<< "Error! The file \""
					<< filePath
					<< "\" does not seem to contain bit mask information\n";
				
				throw std::runtime_error("Error! Unrecognized file format.");
			}
			
			inp.read(reinterpret_cast<char*>(&BmpColorHeader), sizeof BmpColorHeader);
			
			// Check if the pixel data is stored as BGRA and if the color space type is sRGB
			CheckColorHeader(BmpColorHeader);
		}

		// Jump to the pixel data location
		inp.seekg(FileHeader.offset_data, inp.beg);

		// Adjust the header fields for output.
		// Some editors will put extra info in the image file, we only save the headers and the data.
		if (BmpInfoHeader.bit_count == 32)
		{
			BmpInfoHeader.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
			FileHeader.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
		}
		else
		{
			BmpInfoHeader.size = sizeof(BMPInfoHeader);
			FileHeader.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
		}
		
		FileHeader.file_size = FileHeader.offset_data;

		if (BmpInfoHeader.height < 0)
		{
			throw std::runtime_error(
				"The program can treat only BMP images with the origin in the bottom left corner!");
		}

		Data.resize(BmpInfoHeader.width * BmpInfoHeader.height * BmpInfoHeader.bit_count / 8);

		// Here we check if we need to take into account row padding
		if (BmpInfoHeader.width % 4 == 0)
		{
			inp.read(reinterpret_cast<char*>(Data.data()), Data.size());
			FileHeader.file_size += Data.size();
		}
		else
		{
			m_rowStride = BmpInfoHeader.width * BmpInfoHeader.bit_count / 8;
			const uint32_t newStride = MakeStrideAligned(4);
			std::vector<uint8_t> paddingRow(newStride - m_rowStride);

			for (int y = 0; y < BmpInfoHeader.height; ++y)
			{
				inp.read(reinterpret_cast<char*>(Data.data() + m_rowStride * y), m_rowStride);
				inp.read(reinterpret_cast<char*>(paddingRow.data()), paddingRow.size());
			}

			FileHeader.file_size += Data.size() + BmpInfoHeader.height * paddingRow.size();
		}
	}

	BMP(const int32_t width, const int32_t height, const bool hasAlpha = true)
	{
		if (width <= 0 || height <= 0)
		{
			throw std::runtime_error("The image width and height must be positive numbers.");
		}

		BmpInfoHeader.width = width;
		BmpInfoHeader.height = height;

		if (hasAlpha)
		{
			BmpInfoHeader.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
			FileHeader.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);

			BmpInfoHeader.bit_count = 32;
			BmpInfoHeader.compression = 3;
			m_rowStride = width * 4;
			Data.resize(m_rowStride * height);
			FileHeader.file_size = FileHeader.offset_data + Data.size();

			return;
		}

		BmpInfoHeader.size = sizeof(BMPInfoHeader);
		FileHeader.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

		BmpInfoHeader.bit_count = 24;
		BmpInfoHeader.compression = 0;
		m_rowStride = width * 3;
		Data.resize(m_rowStride * height, 0);

		const uint32_t newStride = MakeStrideAligned(4);
		FileHeader.file_size =
			FileHeader.offset_data +
			Data.size() +
			BmpInfoHeader.height * (newStride - m_rowStride);
	}

	void Write(const char* filePath) const
	{
		std::fstream of(filePath, std::ios_base::binary | std::ios_base::out);
		if (!of.is_open())
		{
			throw std::runtime_error("Unable to open the output image file.");
		}

		if (BmpInfoHeader.bit_count == 32)
		{
			WriteHeadersAndData(of);
			return;
		}

		if (BmpInfoHeader.bit_count == 24)
		{
			if (BmpInfoHeader.width % 4 == 0)
			{
				WriteHeadersAndData(of);
				return;
			}

			const uint32_t newStride = MakeStrideAligned(4);
			const std::vector<uint8_t> paddingRow(newStride - m_rowStride);

			WriteHeaders(of);

			for (int y = 0; y < BmpInfoHeader.height; ++y)
			{
				of.write(reinterpret_cast<const char*>(Data.data() + m_rowStride * y), m_rowStride);
				of.write(reinterpret_cast<const char*>(paddingRow.data()), paddingRow.size());
			}

			return;
		}

		throw std::runtime_error("The program can treat only 24 or 32 bits per pixel BMP files");
	}

	void FillRegion(
		const uint32_t x0,
		const uint32_t y0,
		const uint32_t w,
		const uint32_t h,
		const uint8_t b,
		const uint8_t g,
		const uint8_t r,
		const uint8_t a)
	{
		if (x0 + w > BmpInfoHeader.width || y0 + h > BmpInfoHeader.height)
		{
			throw std::runtime_error("The region does not fit in the image!");
		}

		const uint32_t channels = BmpInfoHeader.bit_count / 8;
		for (uint32_t y = y0; y < y0 + h; ++y)
		{
			for (uint32_t x = x0; x < x0 + w; ++x)
			{
				Data[channels * (y * BmpInfoHeader.width + x) + 0] = b;
				Data[channels * (y * BmpInfoHeader.width + x) + 1] = g;
				Data[channels * (y * BmpInfoHeader.width + x) + 2] = r;
				if (channels == 4)
				{
					Data[channels * (y * BmpInfoHeader.width + x) + 3] = a;
				}
			}
		}
	}

	void SetPixel(
		const uint32_t x0,
		const uint32_t y0,
		const uint8_t b,
		const uint8_t g,
		const uint8_t r,
		const uint8_t a)
	{
		if (x0 >= BmpInfoHeader.width || y0 >= BmpInfoHeader.height || x0 < 0 || y0 < 0)
		{
			throw std::runtime_error("The point is outside of the image boundaries!");
		}

		const uint32_t channels = BmpInfoHeader.bit_count / 8;
		const int offset = channels * (y0 * BmpInfoHeader.width + x0);
		
		Data[offset] = b;
		Data[offset + 1] = g;
		Data[offset + 2] = r;
		if (channels == 4)
		{
			Data[offset + 3] = a;
		}
	}

	void DrawRectangle(
		const uint32_t x0,
		const uint32_t y0,
		const uint32_t w,
		const uint32_t h,
		const uint8_t b,
		const uint8_t g,
		const uint8_t r,
		const uint8_t a,
		const uint8_t lineW)
	{
		if (x0 + w > BmpInfoHeader.width || y0 + h > BmpInfoHeader.height)
		{
			throw std::runtime_error("The rectangle does not fit in the image!");
		}

		FillRegion(x0, y0, w, lineW, b, g, r, a); // top line
		FillRegion(x0, y0 + h - lineW, w, lineW, b, g, r, a); // bottom line
		FillRegion(x0 + w - lineW, y0 + lineW, lineW, h - 2 * lineW, b, g, r, a); // right line
		FillRegion(x0, y0 + lineW, lineW, h - 2 * lineW, b, g, r, a); // left line
	}

private:
	uint32_t m_rowStride{ 0 };

	void WriteHeaders(std::fstream& of) const
	{
		of.write(reinterpret_cast<const char*>(&FileHeader), sizeof(FileHeader));
		of.write(reinterpret_cast<const char*>(&BmpInfoHeader), sizeof(BmpInfoHeader));
		if (BmpInfoHeader.bit_count == 32)
		{
			of.write(reinterpret_cast<const char*>(&BmpColorHeader), sizeof(BmpColorHeader));
		}
	}

	void WriteHeadersAndData(std::fstream& of) const
	{
		WriteHeaders(of);
		of.write(reinterpret_cast<const char*>(Data.data()), Data.size());
	}

	// Add 1 to the row_stride until it is divisible with align_stride
	uint32_t MakeStrideAligned(const uint32_t alignStride) const
	{
		uint32_t newStride = m_rowStride;
		while (newStride % alignStride != 0)
		{
			newStride++;
		}
		return newStride;
	}

	// Check if the pixel data is stored as BGRA and if the color space type is sRGB
	void CheckColorHeader(const BMPColorHeader& bmpColorHeader)
	{
		const BMPColorHeader expectedColorHeader;
		if (expectedColorHeader.red_mask != bmpColorHeader.red_mask ||
			expectedColorHeader.blue_mask != bmpColorHeader.blue_mask ||
			expectedColorHeader.green_mask != bmpColorHeader.green_mask ||
			expectedColorHeader.alpha_mask != bmpColorHeader.alpha_mask)
		{
			throw std::runtime_error(
				"Unexpected color mask format! The program expects the pixel data to be in the BGRA format");
		}
		if (expectedColorHeader.color_space_type != bmpColorHeader.color_space_type)
		{
			throw std::runtime_error("Unexpected color space type! The program expects sRGB values");
		}
	}
};
