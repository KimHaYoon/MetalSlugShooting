#pragma once
#include "Server.h"

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
	float			m_fLimitTime;
	bool			m_bLimit;

	float			m_fFPS;
	float			m_fFPSTime;
	int				m_iFrame;
	int				m_iFrameMax;

public:
	float GetDeltaTime()	const;
	float GetFPS()			const;
	bool GetLimit()			const;

public:
	bool Init();
	void Update();
};

