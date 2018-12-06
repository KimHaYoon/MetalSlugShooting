#pragma once
#include "CGameObject.h"

class CItem : public CGameObject
{
public:
	CItem();
	CItem(const string& strTag);
	virtual ~CItem();

private:
	ItemInfo	m_tInfo;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);


	void SetItemNum(int num);
	int GetNum()	const;
};