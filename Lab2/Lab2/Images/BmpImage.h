#pragma once

#include "Color.h"
#include "Size.h"
#include "../Libs/BmpImageHandler/Bmp.h"

class BmpImage
{
public:
	static BmpImage FromFile(const std::string& path)
	{
		const auto rawImage = new BMP(path.c_str());
		return BmpImage(path, rawImage);
	}

	~BmpImage()
	{
		delete m_rawImage;
	}

	void Save() const
	{
		Save(m_path);
	}

	void Save(const std::string& path) const
	{
		if ( m_rawImage == nullptr )
		{
			return;
		}

		m_rawImage->Write(path.c_str());
	}

	Size GetSize() const
	{
		return m_size;
	}

	void SetPixel(const int x, const int y, const Color& color)
	{
		m_rawImage->SetPixel(x, y, color.GetB(), color.GetG(), color.GetR(), 0);
	}

	Color GetPixel(const int x, const int y) const
	{
		if (x >= m_rawImage->BmpInfoHeader.width || y >= m_rawImage->BmpInfoHeader.height || x < 0 || y < 0)
		{
			throw std::runtime_error("The point is outside of the image boundaries!");
		}
		
		const uint32_t channels = m_rawImage->BmpInfoHeader.bit_count / 8;
		const int offset = channels * (y * m_rawImage->BmpInfoHeader.width + x);

		return Color(
			m_rawImage->Data[offset + 2],
			m_rawImage->Data[offset + 1],
			m_rawImage->Data[offset] );
	}

private:
	explicit BmpImage(const std::string& path, BMP* bmp)
		: m_path(path), m_rawImage(bmp)
	{
		m_size = Size(m_rawImage->BmpInfoHeader.width, m_rawImage->BmpInfoHeader.height);
	}

	std::string m_path;
	BMP* m_rawImage;
	Size m_size;
};
