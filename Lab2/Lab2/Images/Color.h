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

	int GetR() const { return m_r; }
	void SetR(const int value ) { m_r = value; }
	
	int GetG() const { return m_g; }
	void SetG(const int value ) { m_g = value; }
	
	int GetB() const { return m_b; }
	void SetB(const int value ) { m_b = value; }

private:
	int m_r;
	int m_g;
	int m_b;
};
