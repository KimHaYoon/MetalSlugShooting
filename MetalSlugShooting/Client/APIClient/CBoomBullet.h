#pragma once
#include "CGameObject.h"
class CBoomBullet :
	public CGameObject
{
public:
	CBoomBullet();
	CBoomBullet(const string& strTag);
	virtual ~CBoomBullet();

private:
	BoomInfo	m_tInfo;

public:
	void SetInfo(BoomInfo info);
	BoomInfo GetInfo() const;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};
