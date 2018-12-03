#pragma once

#include "Client.h"

class CAnimation
{
public:
	CAnimation();
	~CAnimation();

private:
	ANIMATION		m_tInfo;
	vector<UINT>	m_vecLineFrameCount;
	UINT			m_iCurrentAnimation;
	bool			m_bFrameEnd;
	bool			m_bAnimationStart;

public:
	bool GetAnimationFrameEnd()	const
	{
		return m_bFrameEnd;
	}

	bool GetAnimationStart() const
	{
		return m_bAnimationStart;
	}

	ANIMATION GetAnimationInfo()	const
	{
		return m_tInfo;
	}

	UINT GetFrameWidth()	const
	{
		return m_tInfo.iImageWidth / m_tInfo.iFrameMaxX;
	}

	UINT GetFrameHeight()	const
	{
		return m_tInfo.iImageHeight / m_tInfo.iFrameMaxY;
	}

	float GetLimitTime()	const
	{
		return m_tInfo.fLimitTime;
	}

	UINT GetFrameX()	const
	{
		return m_tInfo.iFrameX;
	}

	UINT GetCurrentAnimation()	const
	{
		return m_iCurrentAnimation;
	}

public:
	void SetAnimationStart(bool bAnimationStart)
	{
		m_bAnimationStart = bAnimationStart;
	}

	void SetLimitTime(float fTime)
	{
		m_tInfo.fLimitTime = fTime;
	}

	// 필요에 따라 Set함수를 만들어 사용합시다.
	void AddLineFrameCount(UINT iFrameCount)
	{
		m_vecLineFrameCount.push_back(iFrameCount);
	}

	void SetTextureSize(UINT iWidth, UINT iHeight)
	{
		m_tInfo.iImageHeight = iHeight;
		m_tInfo.iImageWidth = iWidth;
	}

	void SetFrameX(UINT x)
	{
		m_tInfo.iFrameX = x;
		m_bAnimationStart = false;
	}

public:
	CAnimation * Create(const char* pName, ANIMATION_TYPE eType,
		ANIMATION_OPTION eOption, UINT iWidth, UINT iHeight,
		UINT iFrameMaxX, UINT iFrameMaxY, float fLimitTime);

public:
	bool Init(const char* pName, ANIMATION_TYPE eType,
		ANIMATION_OPTION eOption, UINT iWidth, UINT iHeight,
		UINT iFrameMaxX, UINT iFrameMaxY, float fLimitTime);
	void Update(float fTime);

public:
	void ChangeAnimation(UINT iAnimation);
};

