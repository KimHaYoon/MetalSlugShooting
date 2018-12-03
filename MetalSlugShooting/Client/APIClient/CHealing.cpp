#include "stdafx.h"
#include "CHealing.h"


CHealing::CHealing()
{
}

CHealing::CHealing(const string & strTag) : CItem(strTag)
{
}


CHealing::~CHealing()
{
}

bool CHealing::Init()
{
	if (m_tInfo.num == 0)	// HP30
		m_iHealAmount = 30;
	else if (m_tInfo.num == 1)	// HP70
		m_iHealAmount = 70;

	return true;
}

void CHealing::Input(float fTime)
{
}

void CHealing::Update(float fTime)
{
	CItem::Update(fTime);
}

void CHealing::Render(HDC hDC, float fTime)
{
	CItem::Render(hDC, fTime);
}
