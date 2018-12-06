#include "stdafx.h"
#include "CLobby.h"
#include "CNetwork.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CBackground.h"
#include "CLobbyTimerUI.h"
#include "CLobbyCharactor.h"


CLobby::CLobby() : 
	m_pBackGround(NULL),
	m_pRect(NULL)
{
	for (int i = 0; i < PLAYERMAX; ++i)
	{
		m_pPlayerNum[i] = NULL;
		m_pCharactor[i] = NULL;
	}
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
	m_pBackGround->SetTexture("Background", m_hInst, m_hDC, L"Texture/Lobby_background.bmp");
	m_pScene->AddObject(m_pBackGround);

	// m_pPlayerNum
	m_pPlayerNum[0] = new CBackground("PlayerNum1");

	if (!m_pPlayerNum[0]->Init())
	{
		return false;
	}
	m_pPlayerNum[0]->SetPos(420.f, 137.f);
	m_pPlayerNum[0]->SetTexture("Background", m_hInst, m_hDC, L"Texture/p1_resize.bmp", true, RGB(0, 0, 0));
	m_pScene->AddObject(m_pPlayerNum[0]);

	m_pPlayerNum[1] = new CBackground("PlayerNum2");

	if (!m_pPlayerNum[1]->Init())
	{
		return false;
	}
	m_pPlayerNum[1]->SetPos(736.f, 137.f);
	m_pPlayerNum[1]->SetTexture("Background", m_hInst, m_hDC, L"Texture/p2_resize.bmp", true, RGB(0, 0, 0));
	m_pScene->AddObject(m_pPlayerNum[1]);


	m_pRect = new CBackground("Rect");
	if (!m_pRect->Init())
		return false;
	int iNum = GET_NETWORKINST->GetClientNum();
	if (iNum == 1)
		m_pRect->SetPos(350, 210.f);
	else if(iNum == 2)
		m_pRect->SetPos(673, 210.f);
	m_pRect->SetTexture("RECT", m_hInst, m_hDC, L"Texture/red_rect.bmp", true, RGB(255, 255, 255));
	m_pScene->AddObject(m_pRect);


	m_pCharactor[0] = new CLobbyCharactor("Player1");
	if (!m_pCharactor[0]->Init())
	{
		return false;
	}
	m_pCharactor[0]->SetPos(354, 221.f);
	m_pCharactor[0]->SetNum(0);
	m_pCharactor[0]->SetTexture("Background", m_hInst, m_hDC, L"Texture/p1p2_charactor_resize.bmp");
	m_pScene->AddObject(m_pCharactor[0]);

	m_pCharactor[1] = new CLobbyCharactor("Player2");
	if (!m_pCharactor[1]->Init())
	{
		return false;
	}
	m_pCharactor[1]->SetPos(677, 221.f);
	m_pCharactor[1]->SetNum(1);
	m_pCharactor[1]->SetTexture("Background", m_hInst, m_hDC, L"Texture/p1p2_charactor_resize.bmp");
	m_pScene->AddObject(m_pCharactor[1]);

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
	if (GET_NETWORKINST->Login(1))
	{
		m_pCharactor[0]->SetLogin(true);
	}

	if (GET_NETWORKINST->Login(2))
	{
		m_pCharactor[1]->SetLogin(true);
	}

	if (GET_NETWORKINST->Login(1) && GET_NETWORKINST->Login(2))
	{
		if (time % 10 != 0)
		{
			m_pTimerUI->SetEnable(true);
			m_pTimerUI->SetNum(time % 10);
		}
	}
}

void CLobby::Render(HDC hDC, float fTime)
{
}