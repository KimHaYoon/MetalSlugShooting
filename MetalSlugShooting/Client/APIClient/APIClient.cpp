// APIClient.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "APIClient.h"
#include "CGameFramework.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	CGameFramework* pGame = NULL;
	
	pGame = new CGameFramework;

	if (!(pGame->Init(hInstance, L"MetalSlugShooting", L"MetalSlugShooting", IDI_ICON1, WIN_WIDTH, WIN_HEIGHT, true)))
	{
		delete pGame;

		pGame = NULL;

		return 0;
	}


	int iRet = pGame->Run();

	delete pGame;

	pGame = NULL;

	return iRet;
}
