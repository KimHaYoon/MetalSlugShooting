#pragma once

#include "CGameObject.h"
class CHPBar : public CGameObject
{
public:
	CHPBar();
	CHPBar(const string& strTag);
	virtual ~CHPBar();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};

