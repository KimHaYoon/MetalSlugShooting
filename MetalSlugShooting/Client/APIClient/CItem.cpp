#include "stdafx.h"
#include "CItem.h"
#include "CAnimation.h"

CItem::CItem()
{
}

CItem::CItem(const string & strTag) : CGameObject(strTag)
{
	m_fSpeed = 200.f;
	m_bDrop = true;
}


CItem::~CItem()
{
}

void CItem::SetInfo(int num, int x, int y)
{
	m_tInfo.num = num;
	m_tInfo.x = x;
	m_tInfo.y = y;
}

void CItem::NotDrop()
{
	m_bDrop = false;
}

bool CItem::Init()
{
	m_fSpeed = 200.f;
	m_bDrop = true;
	return true;
}

void CItem::Input(float fTime)
{
}

void CItem::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_bDrop)
	{
		if (m_fPos.y < 600.f)
			m_fPos.y += m_fSpeed * fTime;

		else
		{
			m_bDrop = false;
			if (m_pAnimation)
			{
				int iCurFrame = m_pAnimation->GetAnimationInfo().iFrameX;
				m_pAnimation->SetFrameX(iCurFrame);
			}
		}
	}

}

void CItem::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}
