#include "stdafx.h"
#include "CHeli.h"
#include "CAnimation.h"
#include "CNetwork.h"

CHeli::CHeli()
{
}

CHeli::CHeli(const string & strTag) : CGameObject(strTag)
{
}


CHeli::~CHeli()
{
}

bool CHeli::Init()
{
	CreateAnimation("Heli", AT_LINE, AO_LOOP, 2400, 1200, 6, 3, 1.f);
	m_pAnimation->AddLineFrameCount(6);
	m_pAnimation->AddLineFrameCount(6);
	m_pAnimation->AddLineFrameCount(6);
	m_pAnimation->ChangeAnimation(2);

	m_fPos.y = 10.f;

	return true;
}

void CHeli::Input(float fTime)
{
}

void CHeli::Update(float fTime)
{
	CGameObject::Update(fTime);

	m_tInfo = GET_NETWORKINST->GetHeliInfo();
	m_fPos.x = m_tInfo.x;

	if (m_fPos.x == 440)
	{
		m_pAnimation->ChangeAnimation(1);
	}

	else
		m_pAnimation->ChangeAnimation(2);

}

void CHeli::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}
