#pragma once
#include <climits>
#include <iostream>

class ProgressNotificator
{
public:	
	ProgressNotificator(float maxValue)
	{
		m_maxValue = maxValue;
		m_previousValue = INT_MIN;
		m_progress = 0.f;
	}

	void Update(float currentValue)
	{
		m_progress = currentValue / m_maxValue * 100.f;

		int prevProgress = m_previousValue / m_maxValue * 100.f;
		
		if ( m_progress != prevProgress )
		{
			std::cout << "Porgress: " << m_progress << "%" << std::endl;
		}
		
		m_previousValue = currentValue;
	}

private:
	float m_maxValue;
	float m_previousValue;
	int m_progress;
};
