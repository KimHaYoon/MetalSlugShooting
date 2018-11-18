#include "stdafx.h"
#include "CAnimation.h"


CAnimation::CAnimation() :
	m_iCurrentAnimation(0),
	m_bFrameEnd(false),
	m_bAnimationStart(true)
{
}


CAnimation::~CAnimation()
{
}

CAnimation * CAnimation::Create(const char * pName, ANIMATION_TYPE eType, ANIMATION_OPTION eOption, UINT iWidth, UINT iHeight, UINT iFrameMaxX, UINT iFrameMaxY, float fLimitTime)
{
	CAnimation*	pAnimation = new CAnimation;

	if (!pAnimation->Init(pName, eType, eOption, iWidth, iHeight, iFrameMaxX,
		iFrameMaxY, fLimitTime))
	{
		delete	pAnimation;
		return NULL;
	}

	return pAnimation;
}

bool CAnimation::Init(const char * pName, ANIMATION_TYPE eType, ANIMATION_OPTION eOption, UINT iWidth, UINT iHeight, UINT iFrameMaxX, UINT iFrameMaxY, float fLimitTime)
{
	m_tInfo.strName = pName;
	m_tInfo.eType = eType;
	m_tInfo.eOption = eOption;
	m_tInfo.iImageWidth = iWidth;
	m_tInfo.iImageHeight = iHeight;
	m_tInfo.iFrameMaxX = iFrameMaxX;
	m_tInfo.iFrameMaxY = iFrameMaxY;
	m_tInfo.iFrameX = 0;
	m_tInfo.iFrameY = 0;

	m_tInfo.fLimitTime = fLimitTime;
	m_tInfo.fTime = 0.f;

	return true;
}

void CAnimation::Update(float fTime)
{
	if (m_tInfo.eOption == AO_LOOP)
	{
		if (m_bFrameEnd)
			m_bFrameEnd = false;
	}

	if (m_bFrameEnd)
		return;

	if (!m_bAnimationStart)
		return;

	if (m_tInfo.fLimitTime == -1.f)
		return;

	m_tInfo.fTime += fTime;

	UINT	iFrameMax = m_vecLineFrameCount[m_iCurrentAnimation];

	if (m_tInfo.eType == AT_ALL)
	{
		iFrameMax = 0;
		for (size_t i = 0; i < m_vecLineFrameCount.size(); ++i)
		{
			iFrameMax += m_vecLineFrameCount[i];
		}
	}

	float	fChangeTime = m_tInfo.fLimitTime / iFrameMax;

	while (m_tInfo.fTime >= fChangeTime)
	{
		++m_tInfo.iFrameX;
		m_tInfo.fTime -= fChangeTime;

		if (m_tInfo.iFrameX == m_vecLineFrameCount[m_tInfo.iFrameY])
		{
			m_tInfo.iFrameX = 0;

			if (m_tInfo.eType == AT_ALL)
			{
				++m_tInfo.iFrameY;

				if (m_tInfo.iFrameY == m_tInfo.iFrameMaxY)
				{
					m_tInfo.iFrameY = 0;
					m_bFrameEnd = true;
				}
			}

			else
				m_bFrameEnd = true;
		}
	}
}

void CAnimation::ChangeAnimation(UINT iAnimation)
{
	if (m_iCurrentAnimation == iAnimation)
		return;

	else if (iAnimation >= m_vecLineFrameCount.size())
		return;

	m_tInfo.iFrameX = 0;

	m_tInfo.iFrameY = iAnimation;
	m_iCurrentAnimation = iAnimation;
}
