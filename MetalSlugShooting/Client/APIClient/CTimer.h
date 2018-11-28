#pragma once
#include "Client.h"

class CTimer
{
public:
	CTimer();
	~CTimer();

private:
	LARGE_INTEGER	m_tPrev;
	LARGE_INTEGER	m_tSecond;
	bool			m_bStartUpdate;
	float			m_fDeltaTime;

	float			m_fFPS;
	float			m_fFPSTime;
	UINT			m_iFrame;
	UINT			m_iFrameLimit;

	float			m_fLimitTime;
	bool			m_bLimit;
	float			m_fAccTime;

	HWND			m_hWnd;

public:
	float GetDeltaTime()	const
	{
		return m_fDeltaTime;
	}

	bool GetLimit()	const
	{
		return m_bLimit;
	}


	void SetWindowHandle(HWND hWnd);
public:
	bool Init();
	void Update();
	void Destroy();
};

