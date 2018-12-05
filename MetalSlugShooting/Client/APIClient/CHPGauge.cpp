#include "stdafx.h"
#include "CHPGauge.h"


CHPGauge::CHPGauge()
{
}

CHPGauge::CHPGauge(const string & strTag)
{
}


CHPGauge::~CHPGauge()
{
}

void CHPGauge::SetHP(int iHP)
{
	m_iHP = iHP;
}

bool CHPGauge::Init(int iPlayerNum)
{
	m_iPlayerNum = iPlayerNum;
	m_fHPStartPosX = 0.f;

	m_fSize = float2(310.f, 32.f);
	if (m_iPlayerNum == 1)
		m_fPos = float2(95.f, 25.f);

	else if (m_iPlayerNum == 2)
		m_fPos = float2(875.f, 25.f);

	return true;
}

void CHPGauge::Input(float fTime)
{
}

void CHPGauge::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_iPlayerNum == 1)
	{
		if (m_iHP <= 0)
			m_iHP = 0;
		m_fHPStartPosX = (100 - m_iHP) * 3.1f;
	}

	else if (m_iPlayerNum == 2)
	{
		if (m_iHP <= 0)
			m_iHP = 0;
		m_fSize = float2(m_iHP * 3.1f, 32.f);
	}
}

void CHPGauge::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);

	HBRUSH Brush, oldBrush;
	HPEN	Pen, oldPen;
	Brush = CreateSolidBrush(RGB(209, 57, 34));
	Pen = CreatePen(PS_SOLID, 1, RGB(60, 30, 5));
	oldBrush = (HBRUSH)SelectObject(hDC, Brush);
	oldPen = (HPEN)SelectObject(hDC, Pen);
	Rectangle(hDC, m_fPos.x + m_fHPStartPosX, m_fPos.y, m_fPos.x + m_fSize.x, m_fPos.y + m_fSize.y);
	SelectObject(hDC, oldBrush);
	SelectObject(hDC, oldPen);
	DeleteObject(Brush);
	DeleteObject(Pen);
}
