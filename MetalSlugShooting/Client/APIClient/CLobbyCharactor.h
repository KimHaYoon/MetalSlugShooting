#pragma once
#include "CGameObject.h"
class CLobbyCharactor :
	public CGameObject
{
public:
	CLobbyCharactor();
	CLobbyCharactor(const string& strTag);
	virtual ~CLobbyCharactor();

private:
	int		m_iNum;
	bool	m_bLogin;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);

public:
	void SetNum(int Num);
	void SetLogin(bool Login);
};

