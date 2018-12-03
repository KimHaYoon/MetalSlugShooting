#include "stdafx.h"
#include "CWinOrLose.h"
#include "CAnimation.h"


CWinOrLose::CWinOrLose()
{
}

CWinOrLose::CWinOrLose(const string & strTag) : CGameObject(strTag)
{
}

CWinOrLose::~CWinOrLose()
{
}

void CWinOrLose::SetWinOrLose(bool bWin)
{
	m_bWin = bWin;
}

bool CWinOrLose::Init()
{
	CreateAnimation("Number", AT_LINE, AO_LOOP, 1600, 400, 2, 1, 1.f);
	m_pAnimation->AddLineFrameCount(2);
	m_pAnimation->ChangeAnimation(0);
	m_pAnimation->SetAnimationStart(false);

	m_bWin = true;

	return true;
}

void CWinOrLose::Input(float fTime)
{
}

void CWinOrLose::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_bWin)
	{
		m_pAnimation->SetFrameX(0);
	}

	else
	{
		m_pAnimation->SetFrameX(1);
	}
}

void CWinOrLose::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}
