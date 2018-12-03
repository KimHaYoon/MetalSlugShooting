#include "stdafx.h"
#include "CGameObject.h"
#include "CTexture.h"
#include "CAnimation.h"
#include "CScene.h"


CGameObject::CGameObject() :
	m_bEnable(true),
	m_bAlive(true),
	m_pTexture(NULL),
	m_pAnimation(NULL),
	m_pInput(NULL)
{
	m_fPivot = float2(0.f, 0.f);
}

CGameObject::CGameObject(const string & strTag)
{
	m_strTag = strTag;
	m_bAlive = true;
	m_bEnable = true;
	m_pTexture = NULL;
	m_pAnimation = NULL;
	m_pInput = NULL;
	m_fPivot = float2(0.f, 0.f);
}


CGameObject::~CGameObject()
{
	if (m_pTexture)
	{
		delete m_pTexture;

		m_pTexture = NULL;
	}

	if (m_pAnimation)
	{
		delete m_pAnimation;

		m_pAnimation = NULL;
	}
}

string CGameObject::GetTag() const
{
	return m_strTag;
}

bool CGameObject::GetEnable() const
{
	return m_bEnable;
}

bool CGameObject::GetAlive() const
{
	return m_bAlive;
}

float2 CGameObject::GetPos() const
{
	return m_fPos;
}

float2 CGameObject::GetSize() const
{
	return m_fSize;
}

float2 CGameObject::GetCollPos() const
{
	return m_fCollPos;
}

CAnimation * CGameObject::GetAnimation() const
{
	return m_pAnimation;
}

void CGameObject::SetTag(const string & strTag)
{
	m_strTag = strTag;
}

void CGameObject::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

void CGameObject::Die()
{
	m_bAlive = false;
}

void CGameObject::SetPos(float x, float y)
{
	m_fPos.x = x;
	m_fPos.y = y;
}

void CGameObject::SetSize(float x, float y)
{
	m_fSize.x = x;
	m_fSize.y = y;
}

void CGameObject::SetCollPos(float x, float y)
{
	m_fCollPos.x = x;
	m_fCollPos.y = y;
}

void CGameObject::SetInput(CInput * pInput)
{
	m_pInput = pInput;
}

void CGameObject::SetScene(CScene * pScene)
{
	m_pScene = pScene;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float fTime)
{
	if (m_pAnimation)
		m_pAnimation->Update(fTime);
}

void CGameObject::Render(HDC hDC, float fTime)
{
	float2 fPos = m_fPos;

	if (fPos.x + (1.f - m_fPivot.x) * m_fSize.x < 0 ||
		fPos.x - m_fPivot.x * m_fSize.x > WIN_WIDTH ||
		fPos.y + (1.f - m_fPivot.y) * m_fSize.y < 0 ||
		fPos.y - m_fPivot.y * m_fSize.y > WIN_HEIGHT)
		return;

	if (m_pTexture)
	{
		if (!m_pTexture->GetColorKeyEnable())
		{
			int	x, y;
			x = fPos.x - m_fPivot.x * m_fSize.x;
			y = fPos.y - m_fPivot.y * m_fSize.y;

			UINT	iWidth, iHeight;
			iWidth = m_pTexture->GetWidth();
			iHeight = m_pTexture->GetHeight();

			UINT	iImageX = 0, iImageY = 0;

			if (m_pAnimation)
			{
				iWidth = m_pAnimation->GetFrameWidth();
				iHeight = m_pAnimation->GetFrameHeight();

				iImageX = m_pAnimation->GetAnimationInfo().iFrameX * iWidth;
				iImageY = m_pAnimation->GetAnimationInfo().iFrameY * iHeight;
			}

			BitBlt(hDC, x, y, iWidth, iHeight, m_pTexture->GetMemDC(), iImageX, iImageY, SRCCOPY);
		}

		else
		{
			int	x, y;
			x = fPos.x - m_fPivot.x * m_fSize.x;
			y = fPos.y - m_fPivot.y * m_fSize.y;

			UINT	iWidth, iHeight;
			iWidth = m_pTexture->GetWidth();
			iHeight = m_pTexture->GetHeight();

			UINT	iImageX = 0, iImageY = 0;

			if (m_pAnimation)
			{
				iWidth = m_pAnimation->GetFrameWidth();
				iHeight = m_pAnimation->GetFrameHeight();

				iImageX = m_pAnimation->GetAnimationInfo().iFrameX * iWidth;
				iImageY = m_pAnimation->GetAnimationInfo().iFrameY * iHeight;
			}

			TransparentBlt(hDC, x, y, iWidth, iHeight, m_pTexture->GetMemDC(), iImageX, iImageY, iWidth, iHeight, m_pTexture->GetColorKey());
		}
	}
}

void CGameObject::SetTexture(const string & strKey, HINSTANCE hInst, HDC hDC, const TCHAR * pFileName, bool bColorKey, COLORREF dwColorKey)
{
	if (m_pTexture)
	{
		delete m_pTexture;

		m_pTexture = NULL;
	}

	m_pTexture = new CTexture;

	if (pFileName)
	{
		if (!m_pTexture->LoadTexture(strKey, hInst, hDC, pFileName, bColorKey, dwColorKey))
		{
			if (m_pTexture)
			{
				delete m_pTexture;

				m_pTexture = NULL;
			}
		}
	}

	if (m_pAnimation)
	{
		m_pAnimation->SetTextureSize(m_pTexture->GetWidth(), m_pTexture->GetHeight());
		SetSize(m_pAnimation->GetFrameWidth(), m_pAnimation->GetFrameHeight());
	}

	else
		SetSize(m_pTexture->GetWidth(), m_pTexture->GetHeight());
}

void CGameObject::CreateAnimation(const char * pName, ANIMATION_TYPE eType, ANIMATION_OPTION eOption, UINT iWidth, UINT iHeight, UINT iFrameMaxX, UINT iFrameMaxY, float fLimitTime)
{
	if (m_pAnimation)
	{
		delete m_pAnimation;

		m_pAnimation = NULL;
	}

	m_pAnimation = new CAnimation;

	if (pName)
	{
		if (!m_pAnimation->Init(pName, eType, eOption, iWidth, iHeight, iFrameMaxX, iFrameMaxY, fLimitTime))
		{
			delete m_pAnimation;

			m_pAnimation = NULL;
		}
	}
}
