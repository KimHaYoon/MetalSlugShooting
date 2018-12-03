#include "stdafx.h"
#include "CTimerUI.h"
#include "CAnimation.h"

CTimerUI::CTimerUI()
{
}

CTimerUI::CTimerUI(const string & strTag) : CGameObject(strTag)
{
}


CTimerUI::~CTimerUI()
{
}

void CTimerUI::SetNum(int iNum)
{
	m_iNum = iNum;
}

bool CTimerUI::Init()
{
	m_iNum = 9;
	CreateAnimation("Number", AT_LINE, AO_LOOP, 1000, 100, 10, 1, 1.f);
	m_pAnimation->AddLineFrameCount(10);
	m_pAnimation->ChangeAnimation(0);
	m_pAnimation->SetAnimationStart(false);
	return true;
}

void CTimerUI::Input(float fTime)
{

}

void CTimerUI::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_iNum != 0)
	{
		m_pAnimation->SetFrameX(m_iNum);
	}

	else
	{
		m_pAnimation->SetFrameX(0);
	}
}

void CTimerUI::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}
