#pragma once

#include "Client.h"
#include "CSceneScript.h"

class CScene
{
private:
	friend class CSceneManager;

public:
	CScene();
	~CScene();

private:
	list<class CGameObject*>	m_ObjList;
	CSceneScript*			m_pSceneScript;
	class CInput*			m_pInput;

	HDC			m_hDC;
	HINSTANCE	m_hInst;

	string		m_strTag;

public:
	bool Init();
	void Input(float fTime);
	void Update(float fTime);
	void Render(HDC hDC, float fTime);

public:
	template <typename T>
	bool CreateSceneScript()
	{
		if (m_pSceneScript)
		{
			delete m_pSceneScript;

			m_pSceneScript = NULL;
		}

		m_pSceneScript = new T;

		m_pSceneScript->SetScene(this);
		m_pSceneScript->SetWindHandle(m_hInst, m_hDC);

		if (!m_pSceneScript->Init())
		{
			if (m_pSceneScript)
			{
				delete m_pSceneScript;

				m_pSceneScript = NULL;
			}
			return false;
		}

		return true;
	}

public:
	void AddObject(class CGameObject* pObj);
	void SetWindHandle(HINSTANCE hInst, HDC hDC);
	void SetInput(class CInput* pInput);

	class CInput* GetInput() const;
	HDC	GetHdc() const;
	HINSTANCE	GetInst() const;


	void SetTag(string strTag);
};

