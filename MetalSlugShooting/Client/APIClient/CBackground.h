#pragma once
#include "CGameObject.h"

class CBackground : public CGameObject
{
public:
	CBackground();
	CBackground(const string& strTag);
	virtual ~CBackground();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};

