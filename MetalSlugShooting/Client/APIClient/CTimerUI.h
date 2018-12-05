#pragma once
#include "CGameObject.h"
class CTimerUI :
	public CGameObject
{
public:
	CTimerUI();
	CTimerUI(const string& strTag);
	virtual ~CTimerUI();

private:
	int m_iNum;

public:
	void SetNum(int iNum);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};

