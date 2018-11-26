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
	m_fLimitTime = 0.f;
	m_bLimit = false;
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

	if (m_fLimitTime < (1 / 60.f))
		m_bLimit = true;

	else
	{
		m_bLimit = false;
		m_fLimitTime -= (1 / 60.f);
	}

	m_fLimitTime += m_fDeltaTime;

	m_tTime = tTime;

	m_fFPSTime += m_fDeltaTime;
	++m_iFrame;

	if (m_iFrame == m_iFrameMax)
	{
		m_fFPS = m_iFrame / m_fFPSTime;

		cout << m_fFPS << endl;

		m_fFPSTime = 0.f;
		m_fLimitTime = 0.f;
		m_iFrame = 0;
	}
}
