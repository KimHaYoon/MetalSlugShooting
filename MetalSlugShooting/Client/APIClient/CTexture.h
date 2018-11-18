#pragma once

#include "Client.h"

class CTexture
{
public:
	CTexture();
	~CTexture();

private:
	string		m_strKey;
	wstring		m_strFileName;

	HDC			m_hMemDC;
	HBITMAP		m_hBmp;
	HBITMAP		m_hOldBmp;
	BITMAP		m_tInfo;
	bool		m_bColorKey;
	COLORREF	m_dwColorKey;

public:
	UINT GetWidth()	const
	{
		return m_tInfo.bmWidth;
	}

	UINT GetHeight()	const
	{
		return m_tInfo.bmHeight;
	}

	HDC GetMemDC()	const
	{
		return m_hMemDC;
	}

	bool GetColorKeyEnable()	const
	{
		return m_bColorKey;
	}

	COLORREF GetColorKey()	const
	{
		return m_dwColorKey;
	}

	void* GetPixelData()	const
	{
		return m_tInfo.bmBits;
	}

	string GetKey() const
	{
		return m_strKey;
	}

public:
	bool LoadTexture(const string& strKey, HINSTANCE hInst, HDC hDC, const TCHAR* pFileName, bool bColorKey = false,
		COLORREF dwColorKey = RGB(255, 0, 255));
	bool LoadTextureFromFullPath(const string& strKey, HINSTANCE hInst, HDC hDC,
		const wchar_t* pFullPath, bool bColorKey = false, COLORREF dwColorKey = RGB(255, 0, 255));
};

