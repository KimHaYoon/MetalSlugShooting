#pragma once

#include "Client.h"

class CSceneManager
{
public:
	CSceneManager();
	~CSceneManager();

private:
	class CScene*		m_pCurScene;
	class CScene*		m_pNextScene;

	class CScene*		m_pLobbyScene;
	class CScene*		m_pIngameScene;
	class CInput*		m_pInput;

	HDC			m_hDC;
	HINSTANCE	m_hInst;

	bool				m_bChange;

public:
	class CScene*	GetNextScene() const;
	class CScene*	GetCurScene()	const;

	void SetInput(class CInput* pInput);

public:
	bool Init(HINSTANCE hInst, HDC hDC, class CInput* pInput);
	int Input(float fTime);
	int Update(float fTime);
	void Render(HDC hDC, float fTime);
	int ChangeScene();
	int ChangeScene(class CScene* pScene);
	void SetNextScene(class CScene* pScene);
	class CScene* CreateScene();
};

