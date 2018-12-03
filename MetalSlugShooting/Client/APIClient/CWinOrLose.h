#pragma once
#include "CGameObject.h"

class CWinOrLose : public CGameObject
{
public:
	CWinOrLose();
	CWinOrLose(const string& strTag);
	virtual ~CWinOrLose();

private:
	bool	m_bWin;

public:
	void SetWinOrLose(bool	bWin);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};

