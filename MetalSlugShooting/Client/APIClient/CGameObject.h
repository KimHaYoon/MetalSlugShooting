#pragma once

#include "Client.h"

class CGameObject
{
protected:
	friend class CScene;

public:
	CGameObject();
	CGameObject(const string& strTag);
	virtual ~CGameObject();


protected:
	string	m_strTag;
	bool	m_bEnable;
	bool	m_bAlive;

	float2	m_fPos;			// ÁÂÇ¥ x,y
	float2	m_fSize;		// Å©±â
	float2	m_fPivot;		// ÇÇº¿ 
	float2	m_fCollPos;		// Ãæµ¹Ã¼ ÁÂÇ¥ x,y

	class CTexture*		m_pTexture;
	class CAnimation*	m_pAnimation;
	class CInput*		m_pInput;
	class CScene*		m_pScene;

public:
	string GetTag() const;
	bool GetEnable() const;
	bool GetAlive()	const;
	float2 GetPos() const;
	float2 GetSize() const;
	float2 GetCollPos() const;
	class CAnimation* GetAnimation() const;


public:
	void SetTag(const string& strTag);
	void SetEnable(bool bEnable);
	void Die();
	void SetPos(float x, float y);
	void SetSize(float x, float y);
	void SetCollPos(float x, float y);
	void SetInput(class CInput* pInput);
	void SetScene(class CScene* pScene);


public:
	virtual bool Init();
	virtual void Input(float fTime) = 0;
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);

public:
	void SetTexture(const string& strKey, HINSTANCE hInst, HDC hDC, const TCHAR* pFileName, bool bColorKey = false,
		COLORREF dwColorKey = RGB(255, 0, 255));

	void CreateAnimation(const char * pName, ANIMATION_TYPE eType, ANIMATION_OPTION eOption, UINT iWidth, UINT iHeight, UINT iFrameMaxX, UINT iFrameMaxY, float fLimitTime);
};

