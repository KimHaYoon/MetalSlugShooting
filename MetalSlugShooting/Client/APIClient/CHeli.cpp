#include "stdafx.h"
#include "CHeli.h"
#include "CAnimation.h"

CHeli::CHeli()
{
}

CHeli::CHeli(const string & strTag) : CGameObject(strTag)
{
	m_fPivot = float2(0.5f, 0.5f);
	m_fPos.x = 0;
	m_fPos.y = 100.f;
	m_fSpeed = 150.f;
	m_bGo = false;
}


CHeli::~CHeli()
{
}

void CHeli::SetInfo(ItemInfo info)
{
	m_tInfo = info;
}

void CHeli::NotGo()
{
	m_bGo = false;
}

bool CHeli::Init()
{
	m_fPivot = float2(0.5f, 0.5f);
	m_fPos.x = 0;
	m_fPos.y = 100.f;
	m_fSpeed = 150.f;
	m_bGo = false;
	return true;
}

void CHeli::Input(float fTime)
{
}

void CHeli::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_bGo)
	{
		if (m_fPos.x < 640.f)
			m_fPos.x += m_fSpeed * fTime;

		else
		{
			m_bGo = false;
			m_bDrop = true;
			if (m_pAnimation)
			{
				int iCurFrame = m_pAnimation->GetAnimationInfo().iFrameX;
				m_pAnimation->SetFrameX(iCurFrame);
			}
		}
	}

}

void CHeli::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}


bool CHeli::GetbDrop()
{
	return m_bDrop;
}

void CHeli::SetbDrop(bool drop)
{
	m_bDrop = drop;
}

void CHeli::SetbGo(bool go)
{
	m_bGo = go;
}