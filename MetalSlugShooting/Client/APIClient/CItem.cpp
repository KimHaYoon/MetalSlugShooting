#include "stdafx.h"
#include "CItem.h"
#include "CAnimation.h"
#include "CNetwork.h"

CItem::CItem()
{
}

CItem::CItem(const string & strTag) : CGameObject(strTag)
{
}


CItem::~CItem()
{
}

bool CItem::Init()
{
	CreateAnimation("Item", AT_LINE, AO_LOOP, 200, 50, 4, 1, 1.f);
	m_pAnimation->AddLineFrameCount(4);								// 가로 프레임 개수
	m_pAnimation->ChangeAnimation(0);

	return true;
}

void CItem::Input(float fTime)
{
	
}

void CItem::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_tInfo.num != -1)
	{
		m_pAnimation->SetFrameX(m_tInfo.num);
	}

	m_tInfo = GET_NETWORKINST->GetItemInfo(m_tInfo.num);

	m_fPos.x = m_tInfo.x;
	m_fPos.y = m_tInfo.y;
}

void CItem::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);
}

void CItem::SetItemNum(int num)
{
	m_tInfo.num = num;
}

int CItem::GetNum() const
{
	return m_tInfo.num;
}
