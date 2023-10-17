﻿#pragma once

#include "Color.h"
#include "Size.h"
#include "../Libs/BmpImageHandler/Bmp.h"

class Bitmap
{
public:
	static Bitmap FromFile(const std::string& path)
	{
		const auto rawImage = new BMP(path.c_str());
		return Bitmap(rawImage);
	}

	static Bitmap CreateEmpty(const Size& size)
	{
		return CreateEmpty(size.GetWidth(), size.GetHeight());
	}

	static Bitmap CreateEmpty(const int width, const int height)
	{
		return Bitmap(new BMP(width, height, false));
	}

	~Bitmap()
	{
		delete m_rawImage;
	}

	void Save(const std::string& path) const
	{
		if (m_rawImage == nullptr)
		{
			return;
		}

		m_rawImage->Write(path.c_str());
	}

	Size GetSize() const { return m_size; }

	void SetPixel(const int x, const int y, const Color& color)
	{
		ValidatePointOrThrow(x, y);
		m_rawImage->SetPixel(x, y, color.GetB(), color.GetG(), color.GetR(), 0);
	}

	Color GetPixel(const int x, const int y) const
	{
		ValidatePointOrThrow(x, y);

		const int offset = m_channelsNumber * (y * m_size.GetWidth() + x);

		return Color(
			m_rawImage->Data[offset + 2],
			m_rawImage->Data[offset + 1],
			m_rawImage->Data[offset]);
	}

private:
	explicit Bitmap(BMP* bmp) : m_rawImage(bmp)
	{
		m_size = Size(m_rawImage->BmpInfoHeader.width, m_rawImage->BmpInfoHeader.height);
		m_channelsNumber = m_rawImage->BmpInfoHeader.bit_count / 8;
	}

	BMP* m_rawImage;
	Size m_size;
	uint32_t m_channelsNumber;

	inline void ValidatePointOrThrow(const int x, const int y) const
	{
		if (x >= m_rawImage->BmpInfoHeader.width || y >= m_rawImage->BmpInfoHeader.height || x < 0 || y < 0)
		{
			throw std::runtime_error("The point is outside of the image boundaries!");
		}
	}
};
