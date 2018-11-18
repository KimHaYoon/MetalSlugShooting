#pragma once
#include "CGameObject.h"

class CItem : public CGameObject
{
public:
	CItem();
	CItem(const string& strTag);
	virtual ~CItem();

protected:
	ItemInfo	m_tInfo;
	float		m_fSpeed;
	bool		m_bDrop;

protected:
	virtual void SetInfo(int num, int x, int y);
	virtual void NotDrop();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};