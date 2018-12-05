#pragma once
#include "CSceneScript.h"
class CLobby :
	public CSceneScript
{
private:
	friend class CScene;

public:
	CLobby();
	~CLobby();

private:
	class CBackground*		m_pBackGround;
	class CLobbyTimerUI*	m_pTimerUI;

	int time;
	
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};

