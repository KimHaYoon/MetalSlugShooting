#include "stdafx.h"
#include "CHealing.h"


CHealing::CHealing()
{
}

CHealing::CHealing(const string & strTag) : CGameObject(strTag)
{
}


CHealing::~CHealing()
{
}

bool CHealing::Init()
{
	return true;
}

void CHealing::Input(float fTime)
{
}

void CHealing::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CHealing::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}
