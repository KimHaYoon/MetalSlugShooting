#include "stdafx.h"
#include "CSceneScript.h"
#include "CScene.h"


CSceneScript::CSceneScript() :
	m_pScene(NULL)
{
}


CSceneScript::~CSceneScript()
{
}

void CSceneScript::SetScene(CScene * pScene)
{
	m_pScene = pScene;
}

void CSceneScript::SetWindHandle(HINSTANCE hInst, HDC hDC)
{
	m_hInst = hInst;
	m_hDC = hDC;
}
