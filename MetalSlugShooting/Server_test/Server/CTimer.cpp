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

bool CTimer::GetLimit() const
{
	return m_bLimit;
}

bool CTimer::Init()
{
	QueryPerformanceFrequency(&m_tSecond);

	m_fDeltaTime = 0.f;
	m_fLimitTime = 1.f / 60.f;
	m_bStartUpdate = false;
	m_bLimit = true;
	m_fFPS = 0.f;
	m_fFPSTime = 0.f;
	m_iFrame = 0;
	m_iFrameMax = 60;
	m_tPrev.QuadPart = -1;

	return true;
}

void CTimer::Update()
{
	if (!m_bStartUpdate)
	{
		m_bStartUpdate = true;
		QueryPerformanceCounter(&m_tPrev);
		return;
	}

	LARGE_INTEGER	tTime;
	QueryPerformanceCounter(&tTime);

	m_fDeltaTime = (tTime.QuadPart - m_tPrev.QuadPart) / (float)m_tSecond.QuadPart;

	m_tPrev = tTime;

	m_fFPSTime += m_fDeltaTime;
	++m_iFrame;

	if (m_iFrame == m_iFrameMax)
	{
		m_fFPS = m_iFrame / m_fFPSTime;
		m_iFrame = 0;
		m_fFPSTime = 0.f;
	}
}
