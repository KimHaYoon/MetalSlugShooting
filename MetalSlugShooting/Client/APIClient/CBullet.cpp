#include "stdafx.h"
#include "CBullet.h"
#include "CScene.h"


CBullet::CBullet()
{
}

CBullet::CBullet(const string & strTag) : CGameObject(strTag)
{
}


CBullet::~CBullet()
{
}

void CBullet::SetInfo(BulletInfo info)
{
	m_tInfo = info;
}

BulletInfo CBullet::GetInfo() const
{
	return m_tInfo;
}

bool CBullet::Init()
{
	return true;
}

void CBullet::Input(float fTime)
{
}

void CBullet::Update(float fTime)
{
	CGameObject::Update(fTime);

	/*m_fPos.x += m_fSpeed * m_iDir * fTime;

	if (m_fPos.x + (1.f - m_fPivot.x) * m_fSize.x < 0 ||
	m_fPos.x - m_fPivot.x * m_fSize.x > WIN_WIDTH ||
	m_fPos.y + (1.f - m_fPivot.y) * m_fSize.y < 0 ||
	m_fPos.y - m_fPivot.y * m_fSize.y > WIN_HEIGHT)
	{
	Die();
	}*/

}

void CBullet::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}
