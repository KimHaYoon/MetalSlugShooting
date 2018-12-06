#include "stdafx.h"
#include "CLobbyCharactor.h"
#include "CAnimation.h"


CLobbyCharactor::CLobbyCharactor()
{
}

CLobbyCharactor::CLobbyCharactor(const string & strTag)
{
}


CLobbyCharactor::~CLobbyCharactor()
{
}

bool CLobbyCharactor::Init()
{
	CreateAnimation("Ch", AT_LINE, AO_LOOP, 480, 816, 2, 2, 1.f);
	m_pAnimation->AddLineFrameCount(2);								// 가로 프레임 개수
	m_pAnimation->AddLineFrameCount(2);								// 가로 프레임 개수
	m_pAnimation->ChangeAnimation(0);
	m_pAnimation->SetAnimationStart(false);
	m_iNum = 0;
	m_bLogin = false;

	return true;
}

void CLobbyCharactor::Input(float fTime)
{
}

void CLobbyCharactor::Update(float fTime)
{
	CGameObject::Update(fTime);

	m_pAnimation->ChangeAnimation(m_iNum);

	if (m_bLogin)
	{
		m_pAnimation->SetFrameX(1);
	}

	else
	{
		m_pAnimation->SetFrameX(0);
	}
}

void CLobbyCharactor::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}

void CLobbyCharactor::SetNum(int Num)
{
	m_iNum = Num;
}

void CLobbyCharactor::SetLogin(bool Login)
{
	m_bLogin = Login;
}
