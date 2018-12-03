#include "stdafx.h"
#include "CItem.h"
#include "CAnimation.h"

CItem::CItem()
{
}

CItem::CItem(const string & strTag) : CGameObject(strTag)
{
	m_fPivot = float2(0.5f, 0.5f);
	m_fPos.x = 640.f;
	m_fSpeed = 200.f;
	m_bDrop = false;
}


CItem::~CItem()
{
}

void CItem::SetInfo(ItemInfo info)
{
	m_tInfo = info;
}

void CItem::NotDrop()
{
	m_bDrop = false;
}

bool CItem::Init()
{
	m_fPivot = float2(0.5f, 0.5f);
	m_fPos.x = 640.f;
	m_fSpeed = 200.f;
	m_bDrop = false;
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
		if (m_fPos.y < 515.f)
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


void CItem::SetDrop(bool drop)
{
	m_bDrop = drop;
	m_fPos.y = 100.f;
}