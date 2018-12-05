#include "stdafx.h"
#include "CNetwork.h"
#include <fstream>

CNetwork* CNetwork::m_pInst = NULL;

CNetwork::CNetwork() : 
	m_iGameState(GAME_READY),
	m_bRecv(false)
{
	m_iTimeLimit = 0;
	m_iTime = 0;
}


CNetwork::~CNetwork()
{
	closesocket(m_Sock);
	WSACleanup();
}

SOCKET CNetwork::GetSock() const
{
	return m_Sock;
}

void CNetwork::SetClientNum(int Num)
{
	m_iClient = Num;
}

void CNetwork::SetKeyData(int key)
{
	m_tKeyData.num = m_iClient;
	m_tKeyData.key = key;
}

void CNetwork::SetGameState(int iState)
{
	m_iGameState = iState;
}

int CNetwork::GetClientNum() const
{
	return m_iClient;
}

DATA CNetwork::GetData() const
{
	return m_tData;
}

int CNetwork::GetGameState() const
{
	return m_iGameState;
}

int CNetwork::GetTimeLimit() const
{
	return m_iTimeLimit;
}

int CNetwork::GetTime() const
{
	return m_iTime;
}

bool CNetwork::GetWin() const
{
	return m_bWin[m_iClient - 1];
}

bool CNetwork::Login(int iNum) const
{
	if (m_tData.player[iNum - 1].num == iNum)
		return true;

	return false;
}

bool CNetwork::Init()
{
	LoadServerIPAddress();
	int retval;

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}


	// socket()
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET)
	{
		return false;
	}

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(m_ServerIP.c_str());
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(m_Sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	
	if (retval == SOCKET_ERROR)
	{
		return false;
	}

	recv(m_Sock, (char*)&m_tData, sizeof(DATA), 0);
	m_iClient = m_tData.num;

	_cprintf("%d\n", m_iClient);

	return true;
}

void CNetwork::Input()
{
	if (m_iGameState == GAME_PLAY)
	{
		send(m_Sock, (char*)&m_tKeyData, sizeof(Key_DATA), 0);
		_cprintf("send KeyData : %d\n", m_tKeyData.key);
	}
}

void CNetwork::Update()
{
	if (m_iGameState != GAME_PLAY)
	{
		if (m_iGameState == GAME_OK)
		{
			recv(m_Sock, (char*)&m_iTime, sizeof(int), 0);
			_cprintf("recv Time : %d\n", m_iTime);

			recv(m_Sock, (char*)&m_tData, sizeof(DATA), 0);
			_cprintf("recv Data \n");
		}

		recv(m_Sock, (char*)&m_iGameState, sizeof(int), 0);
		_cprintf("recv GameState %d\n", m_iGameState);

		if (m_iGameState == GAME_PLAY)
			return;							// game_play상태로 바뀌면 input->update로 순서 맞춰주기위해
	}

	if (m_iGameState == GAME_PLAY)
	{
		recv(m_Sock, (char*)&m_iTimeLimit, sizeof(int), 0);
		_cprintf("recv Time : %d\n", m_iTimeLimit);

		recv(m_Sock, (char*)&m_tData, sizeof(DATA), 0);
		_cprintf("recv Data \n");


		recv(m_Sock, (char*)&m_iGameState, sizeof(int), 0);
		_cprintf("recv GamePLAY\n");
	}

	if (m_iGameState == GAME_END)
	{
		recv(m_Sock, (char*)&m_iGameState, sizeof(int), 0);
		_cprintf("recv GameEnd\n");
		if (!m_bRecv)
		{
			recv(m_Sock, (char*)&m_bWin, sizeof(m_bWin), 0);

			if (m_bWin[m_iClient - 1])
				_cprintf("recv %d win\n", m_iClient);

			else
				_cprintf("recv %d lose\n", m_iClient);

			m_bRecv = true;
		}
	}
}

void CNetwork::Render(HDC hdc)
{
#ifdef _DEBUG
	TCHAR str[128] = {};
	TCHAR time[128] = {};
	if (m_iGameState == GAME_READY)
	{
		wsprintf(str, L"GameState : Game 대기중");
	}

	else if (m_iGameState == GAME_OK)
	{
		wsprintf(time, L"Time : %d", m_iTime);
		TextOut(hdc, 400, 200, time, lstrlen(time));
		wsprintf(str, L"GameState : Game 준비완료");
	}

	else if (m_iGameState == GAME_PLAY)
	{
		wsprintf(time, L"TIME : %d", m_iTimeLimit);
		wsprintf(str, L"GameState : Game 플레이상태");
	}

	else if (m_iGameState == GAME_END)
	{
		wsprintf(str, L"GameState : Game 종료상태");
	}
	TextOut(hdc, 400, 100, str, lstrlen(str));
	TextOut(hdc, 400, 200, time, lstrlen(time));

	if (Login(1))
	{
		wsprintf(str, L"1P Login");
		TextOut(hdc, 50, 160, str, lstrlen(str));
	}

	if (Login(2))
	{
		wsprintf(str, L"2P Login");
		TextOut(hdc, 1100, 160, str, lstrlen(str));
	}

#endif // _DEBUG
}

void CNetwork::LoadServerIPAddress()
{
	TCHAR	strPath[MAX_PATH] = {};
	GetModuleFileName(NULL, strPath, MAX_PATH);

	for (int i = lstrlen(strPath) - 1; i >= 0; --i)
	{
		if (strPath[i] == '\\' || strPath[i] == '/')
		{
			memset(&strPath[i + 1], 0, sizeof(TCHAR) * (MAX_PATH - (i + 1)));
			break;
		}
	}

	char Path[MAX_PATH] = {};

	WideCharToMultiByte(CP_ACP, 0, strPath, -1, Path, lstrlen(strPath), NULL, NULL);

	string FullPath = Path;
	FullPath += "ServerIPAddress.txt";


	FILE* pFile = NULL;

	fopen_s(&pFile, FullPath.c_str(), "rb");

	if (!pFile)
		return;

	wchar_t temp[128] = {};
	wchar_t ip[128] = {};
	char cip[128] = {};

	while (1)
	{
		if (fwscanf_s(pFile, L"%s", temp, 128) == -1)
			break;

		if (!wcscmp(temp, L"[SERVERIP]"))
		{
			fwscanf_s(pFile, L"%s", &ip, 128);
			WideCharToMultiByte(CP_ACP, 0, ip, -1, cip, lstrlen(ip), 0, 0);
			m_ServerIP = cip;
		}
	}

	fclose(pFile);
}
