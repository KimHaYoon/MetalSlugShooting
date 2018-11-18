#include "stdafx.h"
#include "CBackground.h"
#include "CTexture.h"
#include "CInput.h"


CBackground::CBackground()
{
}

CBackground::CBackground(const string & strTag) : CGameObject(strTag)
{
}


CBackground::~CBackground()
{
}

bool CBackground::Init()
{
	return true;
}

void CBackground::Input(float fTime)
{	
}

void CBackground::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CBackground::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}
