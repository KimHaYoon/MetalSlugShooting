#include "stdafx.h"
#include "CLobbyTimerUI.h"
#include "CAnimation.h"


CLobbyTimerUI::CLobbyTimerUI()
{
}

CLobbyTimerUI::CLobbyTimerUI(const string & strTag)
{
}


CLobbyTimerUI::~CLobbyTimerUI()
{
}

void CLobbyTimerUI::SetNum(int iNum)
{
	m_iNum = iNum;
}

bool CLobbyTimerUI::Init()
{
	m_iNum = 0;
	CreateAnimation("LobbyNumber", AT_LINE, AO_LOOP, 5000, 500, 10, 1, 1.f);
	m_pAnimation->AddLineFrameCount(10);
	m_pAnimation->ChangeAnimation(0);
	m_pAnimation->SetAnimationStart(false);

	return true;
}

void CLobbyTimerUI::Input(float fTime)
{
}

void CLobbyTimerUI::Update(float fTime)
{
	CGameObject::Update(fTime);
	m_pAnimation->SetFrameX(m_iNum);
}

void CLobbyTimerUI::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}
