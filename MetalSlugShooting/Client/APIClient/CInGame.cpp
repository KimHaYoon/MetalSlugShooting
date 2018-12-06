#include "stdafx.h"
#include "CInGame.h"
#include "CScene.h"
#include "CGameObject.h"
#include "CBackground.h"
#include "CPlayer.h"
#include "CBoom.h"
#include "CAnimation.h"
#include "CHPBar.h"
#include "CHPGauge.h"
#include "CNetwork.h"
#include "CTimerUI.h"
#include "CWinOrLose.h"
#include "CHeli.h"
#include "CItem.h"

CInGame::CInGame() : 
	m_pBackGround(NULL),
	m_pWinOrLoseUI(NULL),
	m_pHeli(NULL)
{
	for (int i = 0; i < 2; ++i)
	{
		m_pPlayer[i] = NULL;
		m_pHPBar[i] = NULL;
		m_pHPGauge[i] = NULL;
		m_pTimerUI[i] = NULL;
	}

	for (int i = 0; i < 4; ++i)
	{
		m_pItem[i] = NULL;
	}

	ten = 9;
	one = 0;
}


CInGame::~CInGame()
{
}

bool CInGame::Init()
{
	m_pScene->SetTag("Ingame");

	// 배경
	m_pBackGround = new CBackground("Back1");
	
	if (!m_pBackGround->Init())
	{
		return false;
	}
	m_pBackGround->SetTexture("Background", m_hInst, m_hDC, L"Texture/BackGround.bmp");
	m_pScene->AddObject(m_pBackGround);
	//=============================================================================================================
	
	DATA tData = GET_NETWORKINST->GetData();
	
	// Player1
	m_pPlayer[0] = new CPlayer("Player1");

	if (!m_pPlayer[0]->Init())
	{
		return false;
	}

	m_pPlayer[0]->SetPlayerInfo(tData.player[0]);
	m_pPlayer[0]->SetTexture("Player1", m_hInst, m_hDC, L"Texture/1P_ALL.bmp", true, RGB(86.f, 177.f, 222.f));
	m_pPlayer[0]->SetInput(m_pScene->GetInput());
	//m_pPlayer[0]->SetScene(m_pScene);
	m_pScene->AddObject(m_pPlayer[0]);
	//=============================================================================================================
	// Player2
	m_pPlayer[1] = new CPlayer("Player1");

	if (!m_pPlayer[1]->Init())
	{
		return false;
	}

	m_pPlayer[1]->SetPlayerInfo(tData.player[1]);
	m_pPlayer[1]->SetTexture("Player2", m_hInst, m_hDC, L"Texture/2P_ALL.bmp", true, RGB(255.f, 255.f, 255.f));
	m_pPlayer[1]->SetInput(m_pScene->GetInput());
	m_pPlayer[1]->SetScene(m_pScene);
	m_pScene->AddObject(m_pPlayer[1]);
	//=============================================================================================================
	// 1PHPBar
	m_pHPBar[0] = new CHPBar("1PHPBar");
	if (!m_pHPBar[0]->Init())
	{
		return false;
	}
	m_pHPBar[0]->SetTexture("1PHPBar", m_hInst, m_hDC, L"Texture/1PHPBar.bmp", true);
	m_pHPBar[0]->SetPos(15.f, 10.f);
	m_pScene->AddObject(m_pHPBar[0]);
	//=============================================================================================================
	// 2PHPBar
	m_pHPBar[1] = new CHPBar("2PHPBar");
	if (!m_pHPBar[1]->Init())
	{
		return false;
	}
	m_pHPBar[1]->SetTexture("2PHPBar", m_hInst, m_hDC, L"Texture/2PHPBar.bmp", true);
	m_pHPBar[1]->SetPos(865.f, 10.f);
	m_pScene->AddObject(m_pHPBar[1]);
	//=============================================================================================================
	// 1PHP게이지
	m_pHPGauge[0] = new CHPGauge("1PHP");
	if (!m_pHPGauge[0]->Init(1))
	{
		return false;
	}
	m_pHPGauge[0]->SetHP(m_pPlayer[0]->GetHP());
	m_pScene->AddObject(m_pHPGauge[0]);
	//=============================================================================================================
	//2PHP게이지
	m_pHPGauge[1] = new CHPGauge("2PHP");
	if (!m_pHPGauge[1]->Init(2))
	{
		return false;
	}
	m_pHPGauge[1]->SetHP(m_pPlayer[1]->GetHP());
	m_pScene->AddObject(m_pHPGauge[1]);
	//=============================================================================================================
	// TimerUI
	// 십의 자리
	m_pTimerUI[0] = new CTimerUI("Timer_Ten");
	if (!m_pTimerUI[0]->Init())
	{
		return false;
	}
	m_pTimerUI[0]->SetTexture("Num", m_hInst, m_hDC, L"Texture/TimerUI.bmp", true);
	m_pTimerUI[0]->SetPos(547.5f, 3.f);
	m_pTimerUI[0]->SetNum(9);
	m_pScene->AddObject(m_pTimerUI[0]);
	// 일의자리
	m_pTimerUI[1] = new CTimerUI("Timer_one");
	if (!m_pTimerUI[1]->Init())
	{
		return false;
	}
	m_pTimerUI[1]->SetTexture("Num", m_hInst, m_hDC, L"Texture/TimerUI.bmp", true);
	m_pTimerUI[1]->SetPos(627.5f, 3.f);
	m_pTimerUI[1]->SetNum(0);
	m_pScene->AddObject(m_pTimerUI[1]);
	//=============================================================================================================
	// 승패여부
	m_pWinOrLoseUI = new CWinOrLose("WinOrLose");
	if (!m_pWinOrLoseUI->Init())
	{
		return false;
	}
	m_pWinOrLoseUI->SetTexture("WinOrLose", m_hInst, m_hDC, L"Texture/winlose.bmp", true);
	m_pWinOrLoseUI->SetPos(440.f, 350.f);
	m_pWinOrLoseUI->SetEnable(false);
	m_pScene->AddObject(m_pWinOrLoseUI);

	//=============================================================================================================
	// 아이템
	for (int i = 0; i < 4; ++i)
	{
		m_pItem[i] = new CItem("Item");
		if (!m_pItem[i]->Init())
		{
			return false;
		}
		m_pItem[i]->SetTexture("Item", m_hInst, m_hDC, L"Texture/Item.bmp", true, RGB(255.f, 0.f, 255.f));
		m_pItem[i]->SetEnable(false);
		m_pScene->AddObject(m_pItem[i]);
	}
	//=============================================================================================================
	// 헬기
	m_pHeli = new CHeli("Heli");
	if (!m_pHeli->Init())
	{
		return false;
	}
	m_pHeli->SetTexture("Heli", m_hInst, m_hDC, L"Texture/heli.bmp", true, RGB(255.f, 0.f, 255.f));
	m_pHeli->SetEnable(false);
	m_pScene->AddObject(m_pHeli);
	
	return true;
}

void CInGame::Input(float fTime)
{
}

void CInGame::Update(float fTime)
{
	DATA tData = GET_NETWORKINST->GetData();

	for (int i = 0; i < PLAYERMAX; ++i)
	{
		m_pPlayer[i]->SetPlayerInfo(tData.player[i]);
		m_pHPGauge[i]->SetHP(m_pPlayer[i]->GetHP());
		m_pPlayer[i]->SetBulletInfo(tData.bullet[i]);
		m_pPlayer[i]->SetBoomInfo(tData.boom[i]);
	}

	if (GET_NETWORKINST->GetGameState() == GAME_PLAY)
	{
		int TimeLimit = GET_NETWORKINST->GetTimeLimit();
		ten = TimeLimit / 10;
		m_pTimerUI[0]->SetNum(ten);
		one = TimeLimit % 10;
		m_pTimerUI[1]->SetNum(one);

		bool bHeli = GET_NETWORKINST->GetHeli();
		m_pHeli->SetEnable(bHeli);

		//if (bHeli)
		{
			for (int i = 0; i < 4; ++i)
			{
				m_pItem[i]->SetItemNum(GET_NETWORKINST->GetItemInfo(i).num);
				bool bItem = GET_NETWORKINST->GetItemInfo(i).enable;
				m_pItem[i]->SetEnable(bItem);
			}
		}
	}

	if (GET_NETWORKINST->GetGameState() == GAME_END)
	{
		m_pWinOrLoseUI->SetWinOrLose(GET_NETWORKINST->GetWin());
		m_pWinOrLoseUI->SetEnable(true);
	}
}

void CInGame::Render(HDC hDC, float fTime)
{
}