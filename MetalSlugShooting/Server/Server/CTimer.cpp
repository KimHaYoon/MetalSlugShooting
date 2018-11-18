#include "CTimer.h"

CTimer::CTimer()
{
}


CTimer::~CTimer()
{
}

float CTimer::GetDeltaTime() const
{
	return m_fDeltaTime;
}

float CTimer::GetFPS() const
{
	return m_fFPS;
}

bool CTimer::Init()
{
	QueryPerformanceFrequency(&m_tSecond);

	m_fDeltaTime = 0.f;
	m_fFPS = 0.f;
	m_fFPSTime = 0.f;
	m_iFrame = 0;
	m_iFrameMax = 60;
	m_tTime.QuadPart = -1;

	return true;
}

void CTimer::Update()
{
	if (m_tTime.QuadPart == -1)
	{
		QueryPerformanceCounter(&m_tTime);
		return;
	}

	LARGE_INTEGER	tTime;
	QueryPerformanceCounter(&tTime);

	m_fDeltaTime = (tTime.QuadPart - m_tTime.QuadPart) /
		(float)(m_tSecond.QuadPart);

	m_tTime = tTime;

	m_fFPSTime += m_fDeltaTime;
	++m_iFrame;

	if (m_iFrame == m_iFrameMax)
	{
		m_fFPS = m_iFrame / m_fFPSTime;

		m_fFPSTime = 0.f;
		m_iFrame = 0;
	}

}
