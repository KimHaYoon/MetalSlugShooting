#include "stdafx.h"
#include "CScene.h"
#include "CGameObject.h"

CScene::CScene() : 
	m_pSceneScript(NULL),
	m_pInput(NULL)
{
}


CScene::~CScene()
{
	list<class CGameObject*>::iterator iter = m_ObjList.begin();
	list<class CGameObject*>::iterator iterEnd = m_ObjList.end();

	while (iter != iterEnd)
	{
		if ((*iter))
		{
			delete (*iter);

			(*iter) = NULL;
		}
		++iter;
	}

	m_ObjList.clear();

	if (m_pSceneScript)
	{
		delete m_pSceneScript;
		m_pSceneScript = NULL;
	}
}

bool CScene::Init()
{
	return true;
}

void CScene::Input(float fTime)
{
	if (m_pSceneScript)
		m_pSceneScript->Input(fTime);

	list<class CGameObject*>::iterator iter;
	list<class CGameObject*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Input(fTime);
	}
}

void CScene::Update(float fTime)
{
	if (m_pSceneScript)
		m_pSceneScript->Update(fTime);

	list<class CGameObject*>::iterator iter;
	list<class CGameObject*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(fTime);

		if (!(*iter)->GetAlive())
		{
			if ((*iter))
			{
				delete (*iter);

				(*iter) = NULL;
			}
			iter = m_ObjList.erase(iter);
			continue;
		}

		++iter;
	}
}

void CScene::Render(HDC hDC, float fTime)
{
	if (m_pSceneScript)
		m_pSceneScript->Render(hDC, fTime);

	list<class CGameObject*>::iterator iter;
	list<class CGameObject*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}

		if (!(*iter)->GetAlive())
		{
			if ((*iter))
			{
				delete (*iter);

				(*iter) = NULL;
			}
			iter = m_ObjList.erase(iter);
			continue;
		}

		(*iter)->Render(hDC, fTime);
		++iter;
	}
}

void CScene::AddObject(CGameObject * pObj)
{
	pObj->SetScene(this);
	m_ObjList.push_back(pObj);
}

void CScene::SetWindHandle(HINSTANCE hInst, HDC hDC)
{
	m_hDC = hDC;
	m_hInst = hInst;
}

void CScene::SetInput(CInput * pInput)
{
	m_pInput = pInput;
}

CInput * CScene::GetInput() const
{
	return m_pInput;
}

HDC CScene::GetHdc() const
{
	return m_hDC;
}

HINSTANCE CScene::GetInst() const
{
	return m_hInst;
}

void CScene::SetTag(string strTag)
{
	m_strTag = strTag;
}
