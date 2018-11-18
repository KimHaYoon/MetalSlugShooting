#include "stdafx.h"
#include "Client.h"

class CGameFramework
{
public:
	CGameFramework();
	virtual ~CGameFramework();

private:
	static bool m_bLoop;

private:
	HINSTANCE		m_hInst;
	HWND			m_hWnd;
	UINT			m_iWidth;
	UINT			m_iHeight;
	HDC				m_hDC;

	class CTexture*			m_pBackBuffer;
	class CTimer*			m_pTimer;
	class CSceneManager*	m_pSceneManager;
	class CInput*			m_pInput;

public:
	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	HDC GetDC() const
	{
		return m_hDC;
	}

	UINT GetWidth()	const
	{
		return m_iWidth;
	}

	UINT GetHeight()	const
	{
		return m_iHeight;
	}

public:
	bool Init(HINSTANCE hInst, const TCHAR* pTitle, const TCHAR* pClass, int iIconID,
		UINT iWidth, UINT iHeight, bool bWindowMode, bool bOnMouseRenderer = true);
	int Run();


private:
	void Logic();
	void Input(float fTime);
	int Update(float fTime);
	void Render(float fTime);

private:
	ATOM WindowRegisterClass(const TCHAR* pClass, int iIconID);
	BOOL InitWindow(const TCHAR* pClass, const TCHAR* pTitle, UINT iWidth, UINT iHeight);

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};
