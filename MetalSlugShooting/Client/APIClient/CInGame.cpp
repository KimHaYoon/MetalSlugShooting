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

CInGame::CInGame() : 
	m_pBackGround(NULL)
{
	for (int i = 0; i < 2; ++i)
	{
		m_pPlayer[i] = NULL;
		m_pHPBar[i] = NULL;
		m_pHPGauge[i] = NULL;
	}
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
	}
}

void CInGame::Render(HDC hDC, float fTime)
{
}

void CInGame::RecvInitData()
{
}
