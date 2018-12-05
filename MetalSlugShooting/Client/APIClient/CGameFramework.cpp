#include "stdafx.h"
#include "CGameFramework.h"
#include "CTexture.h"
#include "CTimer.h"
#include "CInput.h"
#include "CSceneManager.h"
#include "CNetwork.h"

bool CGameFramework::m_bLoop = true;

#define SERVERON

CGameFramework::CGameFramework() : 
	m_pBackBuffer(NULL),
	m_pTimer(NULL),
	m_pSceneManager(NULL),
	m_pInput(NULL)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(215);

//#ifdef _DEBUG
	AllocConsole();
//#endif
}

CGameFramework::~CGameFramework()
{
	if (m_pBackBuffer)
	{
		delete m_pBackBuffer;

		m_pBackBuffer = NULL;
	}

	if (m_pTimer)
	{
		delete m_pTimer;

		m_pTimer = NULL;
	}

	if (m_pSceneManager)
	{
		delete m_pSceneManager;

		m_pSceneManager = NULL;
	}

	if (m_pInput)
	{
		delete m_pInput;

		m_pInput = NULL;
	}

	ReleaseDC(m_hWnd, m_hDC);

	DESTROY_NETWORK

//#ifdef _DEBUG
		FreeConsole();
//#endif // _DEBUG
}

bool CGameFramework::Init(HINSTANCE hInst, const TCHAR * pTitle, const TCHAR * pClass, int iIconID, UINT iWidth, UINT iHeight, bool bWindowMode, bool bOnMouseRenderer)
{
	srand((unsigned)time(NULL));
	m_hInst = hInst;
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	WindowRegisterClass(pClass, iIconID);

	InitWindow(pClass, pTitle, iWidth, iHeight);

	m_hDC = ::GetDC(m_hWnd);

	// Timer 초기화
	m_pTimer = new CTimer;
	if (!m_pTimer->Init())
	{
		return false;
	}
	m_pTimer->SetWindowHandle(m_hWnd);

	// Input 초기화
	m_pInput = new CInput;
	if (!m_pInput->Init(m_hWnd))
	{
		return false;
	}
	
	// SceneManager 초기화
	m_pSceneManager = new CSceneManager;
	if (!m_pSceneManager->Init(m_hInst, m_hDC, m_pInput))
	{
		return false;
	}	

#ifdef SERVERON
	if (!GET_NETWORKINST->Init())
	{
		return false;
	}
#endif

	// 백버퍼 생성
	m_pBackBuffer = new CTexture;
	m_pBackBuffer->LoadTexture("BackBuffer", m_hInst, m_hDC, L"Texture/BackBuffer.bmp");

	return true;
}

int CGameFramework::Run()
{
	MSG msg;

	while (m_bLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도우 데드타임일 때는 else  구문으로 들어온다.
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CGameFramework::Logic()
{
	m_pTimer->Update();

	float fTime = m_pTimer->GetDeltaTime();
		
	Input(fTime);
	Update(fTime);
	Render(fTime);
}

void CGameFramework::Input(float fTime)
{
	m_pInput->Update(fTime);
	m_pSceneManager->Input(fTime);

	GET_NETWORKINST->Input();
}

int CGameFramework::Update(float fTime)
{
	GET_NETWORKINST->Update();
#ifdef SERVERON

#else
	GET_NETWORKINST->SetGameState(GAME_OK);
#endif // SERVERON


	return m_pSceneManager->Update(fTime);
}

void CGameFramework::Render(float fTime)
{	
	// 장면 안에있는 모든 오브젝트를 백버퍼에 그린다
	m_pSceneManager->Render(m_pBackBuffer->GetMemDC(), fTime);

	GET_NETWORKINST->Render(m_pBackBuffer->GetMemDC());

	// 마지막으로 백버퍼를 화면에 그린다.
	BitBlt(m_hDC, 0, 0, m_iWidth, m_iHeight, m_pBackBuffer->GetMemDC(),	0, 0, SRCCOPY);
}

ATOM CGameFramework::WindowRegisterClass(const TCHAR * pClass, int iIconID)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(iIconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_MY170825);
	wcex.lpszClassName = pClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iIconID));

	return RegisterClassExW(&wcex);
}

BOOL CGameFramework::InitWindow(const TCHAR * pClass, const TCHAR * pTitle, UINT iWidth, UINT iHeight)
{
	HWND hWnd = CreateWindowW(pClass, pTitle, WS_OVERLAPPEDWINDOW,
		300, 0, iWidth, iHeight, nullptr, nullptr, m_hInst, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	// 윈도우의 클라이언트 영역을 원하는 크기로 설정하는 방법
	RECT	rc = { 0, 0, iWidth, iHeight };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(hWnd, HWND_TOPMOST, 300, 0, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER);

	m_hWnd = hWnd;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CGameFramework::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:	// 키를 누를때 들어오는 메세지이다.
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
