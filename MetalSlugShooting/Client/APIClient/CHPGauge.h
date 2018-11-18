#pragma once
#include "CGameObject.h"

class CHPGauge :
	public CGameObject
{
public:
	CHPGauge();
	CHPGauge(const string& strTag);
	virtual ~CHPGauge();
	
private:
	int		m_iPlayerNum;
	int		m_iHP;
	float	m_fHPStartPosX;

public:
	void SetHP(int iHP);

public:
	virtual bool Init(int iPlayerNum);
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};

