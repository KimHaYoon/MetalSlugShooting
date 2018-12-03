#pragma once
#include "CSceneScript.h"
class CInGame : 
	public CSceneScript
{
private:
	friend class CScene;

private:
	CInGame();
	~CInGame();

private:
	class CBackground*	m_pBackGround;
	class CPlayer*		m_pPlayer[2];
	class CHPBar*		m_pHPBar[2];
	class CHPGauge*		m_pHPGauge[2];
	class CTimerUI*		m_pTimerUI[2];
	class CWinOrLose*	m_pWinOrLoseUI;

	int ten, one;
	 
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);

public:
	// 서버랑 통신하는 함수
	void RecvInitData();
};

