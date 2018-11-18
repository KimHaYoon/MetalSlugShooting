#pragma once
#include "CGameObject.h"

class CBullet : public CGameObject
{
public:
	CBullet();
	CBullet(const string& strTag);
	virtual ~CBullet();

private:
	BulletInfo	m_tInfo;

public:
	void SetInfo(BulletInfo info);
	BulletInfo GetInfo() const;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};
