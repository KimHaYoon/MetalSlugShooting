#include "stdafx.h"
#include "CTexture.h"


CTexture::CTexture()
{
	m_dwColorKey = RGB(255, 0, 255);
}


CTexture::~CTexture()
{
	// 그리기 도구를 기존에 가지고 있던 도구로 변경해준다.
	SelectObject(m_hMemDC, m_hOldBmp);

	// 로드한 비트맵 핸들을 지워준다.
	DeleteObject(m_hBmp);

	// 메모리 DC를 지워준다.
	DeleteDC(m_hMemDC);
}

bool CTexture::LoadTexture(const string & strKey, HINSTANCE hInst, HDC hDC, const TCHAR * pFileName, bool bColorKey, COLORREF dwColorKey)
{
	m_strFileName = pFileName;

	return LoadTextureFromFullPath(strKey, hInst, hDC, pFileName, bColorKey, dwColorKey);
}

bool CTexture::LoadTextureFromFullPath(const string & strKey, HINSTANCE hInst, HDC hDC, const wchar_t * pFullPath, bool bColorKey, COLORREF dwColorKey)
{
	m_strKey = strKey;
	m_dwColorKey = dwColorKey;
	m_bColorKey = bColorKey;

	m_hBmp = (HBITMAP)LoadImage(hInst, pFullPath, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE);

	m_hMemDC = CreateCompatibleDC(hDC);

	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBmp);

	GetObject(m_hBmp, sizeof(m_tInfo), &m_tInfo);

	return true;
}
