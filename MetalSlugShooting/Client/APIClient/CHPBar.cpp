#include "stdafx.h"
#include "CHPBar.h"
#include "CScene.h"


CHPBar::CHPBar()
{
}

CHPBar::CHPBar(const string & strTag) : CGameObject(strTag)
{
}

CHPBar::~CHPBar()
{
}

bool CHPBar::Init()
{
	/*m_pScene = pScene;

	if (iPlayerNum == 1)
	{
		SetTexture("1PHPBar", m_pScene->GetInst(), m_pScene->GetHdc(), L"Texture/1PHPBar.bmp", true);
		m_fPos = float2(15.f, 10.f);
	}

	else if (iPlayerNum == 2)
	{
		SetTexture("2PHPBar", m_pScene->GetInst(), m_pScene->GetHdc(), L"Texture/2PHPBar.bmp", true);
		m_fPos = float2(390.f, 10.f);
	}*/
	return true;
}

void CHPBar::Input(float fTime)
{
}

void CHPBar::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void CHPBar::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}