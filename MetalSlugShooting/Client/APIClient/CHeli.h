#pragma once
#include "CGameObject.h"

class CHeli : public CGameObject
{
public:
	CHeli();
	CHeli(const string& strTag);
	virtual ~CHeli();

protected:
	ItemInfo	m_tInfo;
	float		m_fSpeed;
	bool		m_bGo;
	bool		m_bDrop;

protected:
	virtual void SetInfo(ItemInfo info);
	virtual void NotGo();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);

	// test
	virtual bool GetbDrop();
	virtual void SetbDrop(bool drop);
	virtual void SetbGo(bool go);
};