#pragma once
#include "CGameObject.h"
class CLobbyTimerUI :
	public CGameObject
{
public:
	CLobbyTimerUI();
	CLobbyTimerUI(const string& strTag);
	virtual ~CLobbyTimerUI();

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

