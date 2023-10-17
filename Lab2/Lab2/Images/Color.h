#pragma once

struct Color
{
	Color() : Color(0, 0, 0)
	{
	}
	
	Color(const int r, const int g, const int b)
	{
		m_r = r;
		m_b = b;
		m_g = g;
	}

	inline int GetR() const { return m_r; }
	inline void SetR(const int value ) { m_r = value; }
	
	inline int GetG() const { return m_g; }
	inline void SetG(const int value ) { m_g = value; }
	
	inline int GetB() const { return m_b; }
	inline void SetB(const int value ) { m_b = value; }

private:
	int m_r;
	int m_g;
	int m_b;
};
