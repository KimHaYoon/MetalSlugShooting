#pragma once
#include "CGameObject.h"
class CHeli :
	public CGameObject
{
public:
	CHeli();
	CHeli(const string& strTag);
	virtual ~CHeli();

private:
	HeliInfo	m_tInfo;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};

