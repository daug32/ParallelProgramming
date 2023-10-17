#pragma once
#include <climits>

class ProgressNotificator
{
public:	
	ProgressNotificator(float maxValue, bool isEnabled = true)
	{
		m_maxValue = maxValue;
		m_previousValue = INT_MIN;
		m_progress = 0.f;
		m_isEnabled = isEnabled;
	}

	bool GetIsEnabled() const { return m_isEnabled; }
	void SetIsEnabled( bool isEnabled ) { m_isEnabled = isEnabled; }

	void Update(float currentValue)
	{
		if ( !m_isEnabled )
		{
			return;
		} 
		
		m_progress = currentValue / m_maxValue * 100.f;
		int prevProgress = m_previousValue / m_maxValue * 100.f;
		
		if ( m_progress != prevProgress )
		{
			printf("Progress: %d%%\n", m_progress );
		}
		
		m_previousValue = currentValue;
	}

private:
	bool m_isEnabled;

	float m_maxValue;
	float m_previousValue;
	int m_progress;
};
