#include "stdafx.h"
#include "CTimer.h"


CTimer::CTimer()
{
}


CTimer::~CTimer()
{
}

void CTimer::SetWindowHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

bool CTimer::Init()
{
	// 1초에 처리 가능한 tick을 얻어온다.
	QueryPerformanceFrequency(&m_tSecond);

	m_fDeltaTime = 0.f;
	m_fFPS = 0.f;
	m_fFPSTime = 0.f;
	m_iFrame = 0;
	m_iFrameLimit = 60;
	m_bStartUpdate = false;

	m_fLimitTime = 1.f / 120.f;
	m_fAccTime = 0.f;
	m_bLimit = true;

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
	m_fAccTime += m_fDeltaTime;

	//if (m_fAccTime >= m_fLimitTime)
	//{
	//	m_fAccTime -= m_fLimitTime;

	//	++m_iFrame;
	//	//m_bLimit = true;

	//	_cprintf("1/30\n");
	//}

	//else
	//	m_bLimit = false;


	if (m_iFrame == m_iFrameLimit)
	{
		m_fFPS = m_iFrame / m_fFPSTime;
		m_iFrame = 0;
		m_fFPSTime = 0.f;

		char	str[128] = {};

		sprintf_s(str, "DeltaTime : %f FPS : %f", m_fDeltaTime, m_fFPS);
		SetWindowTextA(m_hWnd, str);
	}
}

void CTimer::Destroy()
{
	delete	this;
}