#include "stdafx.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CInGame.h"

CSceneManager::CSceneManager() :
	m_pCurScene(NULL),
	m_pNextScene(NULL)
{
}


CSceneManager::~CSceneManager()
{
	if (m_pCurScene)
	{
		delete m_pCurScene;

		m_pCurScene = NULL;
	}
}

CScene * CSceneManager::GetNextScene() const
{
	return m_pNextScene;
}

CScene * CSceneManager::GetCurScene() const
{
	return m_pCurScene;
}

void CSceneManager::SetInput(CInput * pInput)
{
	m_pInput = pInput;
}

bool CSceneManager::Init(HINSTANCE hInst, HDC hDC, class CInput* pInput)
{
	m_hInst = hInst;
	m_hDC = hDC;
	m_pInput = pInput;

	m_pCurScene = CreateScene();

	m_pCurScene->CreateSceneScript<CInGame>();

	return true;
}

int CSceneManager::Input(float fTime)
{
	m_pCurScene->Input(fTime);
	return 0;
}

int CSceneManager::Update(float fTime)
{
	m_pCurScene->Update(fTime);

	return ChangeScene();
}

void CSceneManager::Render(HDC hDC, float fTime)
{
	m_pCurScene->Render(hDC, fTime);
}

int CSceneManager::ChangeScene()
{
	if (m_pNextScene)
	{
		if (m_pCurScene)
		{
			delete m_pCurScene;

			m_pCurScene = NULL;
		}

		m_pCurScene = m_pNextScene;
		m_pNextScene = NULL;

		return SC_CHANGE;
	}

	return SC_NONE;
}

void CSceneManager::SetNextScene(CScene * pScene)
{
	m_pNextScene = pScene;
}

CScene * CSceneManager::CreateScene()
{
	CScene* pScene = new CScene;

	if (!pScene->Init())
	{
		if (pScene)
		{
			delete pScene;

			pScene = NULL;

			return NULL;
		}
	}

	pScene->SetWindHandle(m_hInst, m_hDC);
	pScene->SetInput(m_pInput);

	return pScene;
}
