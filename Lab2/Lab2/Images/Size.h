#pragma once

struct Size
{
	Size() : Size(0, 0)
	{
	}
	
	Size(const int x, const int y)
		: m_width(x), m_height(y)
	{
	}

	int GetWidth() const { return m_width; }
	void SetWidth(const int value) { m_width = value; }

	int GetHeight() const { return m_height; }
	void SetHeight(const int value) { m_height = value; }
	
private:
	int m_width;
	int m_height;
};
