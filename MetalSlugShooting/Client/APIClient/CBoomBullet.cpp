#include "stdafx.h"
#include "CBoomBullet.h"
#include "CAnimation.h"

CBoomBullet::CBoomBullet()
{
}

CBoomBullet::CBoomBullet(const string & strTag) : CGameObject(strTag)
{
}


CBoomBullet::~CBoomBullet()
{
}

void CBoomBullet::SetInfo(BoomInfo info)
{
	m_tInfo = info;
}

BoomInfo CBoomBullet::GetInfo() const
{
	return m_tInfo;
}

bool CBoomBullet::Init()
{
	CreateAnimation("Boom", AT_LINE, AO_LOOP, 1216, 38, 32, 1, 1.f);
	m_pAnimation->AddLineFrameCount(32);
	m_pAnimation->ChangeAnimation(0);

	return true;
}

void CBoomBullet::Input(float fTime)
{
}

void CBoomBullet::Update(float fTime)
{
	CGameObject::Update(fTime);

	m_fPos.x = m_tInfo.x;
	m_fPos.y = m_tInfo.y;
}

void CBoomBullet::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}
