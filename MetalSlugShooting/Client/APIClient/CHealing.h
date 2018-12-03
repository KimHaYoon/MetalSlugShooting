#pragma once
#include "CItem.h"

class CHealing : public CItem
{
private:
	int m_iHealAmount;

public:
	CHealing();
	CHealing(const string& strTag);
	virtual ~CHealing();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};