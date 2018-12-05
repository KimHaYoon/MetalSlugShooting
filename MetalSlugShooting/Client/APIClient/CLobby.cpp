#include "stdafx.h"
#include "CLobby.h"
#include "CNetwork.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CBackground.h"
#include "CLobbyTimerUI.h"


CLobby::CLobby() : 
	m_pBackGround(NULL)
{
	time = 0;
}


CLobby::~CLobby()
{
}

bool CLobby::Init()
{
	m_pScene->SetTag("Lobby");

	// ¹è°æ
	m_pBackGround = new CBackground("Back1");

	if (!m_pBackGround->Init())
	{
		return false;
	}
	m_pBackGround->SetTexture("Background", m_hInst, m_hDC, L"Texture/BackGround.bmp");
	m_pScene->AddObject(m_pBackGround);

	// TimerUI
	m_pTimerUI = new CLobbyTimerUI("LobbyTimer");
	if (!m_pTimerUI->Init())
	{
		return false;
	}
	m_pTimerUI->SetTexture("Num", m_hInst, m_hDC, L"Texture/LobbyTimerUI.bmp", true);
	m_pTimerUI->SetPos(385.f, 100.f);
	m_pTimerUI->SetEnable(false);
	m_pScene->AddObject(m_pTimerUI);
	return true;
}

void CLobby::Input(float fTime)
{
}

void CLobby::Update(float fTime)
{
	time = GET_NETWORKINST->GetTime();
	if (GET_NETWORKINST->Login(1) && GET_NETWORKINST->Login(2))
	{
		m_pTimerUI->SetEnable(true);
		m_pTimerUI->SetNum(time % 10);
	}
}

void CLobby::Render(HDC hDC, float fTime)
{
	//DATA tData = GET_NETWORKINST->GetData();

}