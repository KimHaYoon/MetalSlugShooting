#include "stdafx.h"
#include "CNetwork.h"

CNetwork* CNetwork::m_pInst = NULL;

CNetwork::CNetwork()
{
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

bool CNetwork::Init()
{
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
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(m_Sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	
	if (retval == SOCKET_ERROR)
	{
		return false;
	}

	recvn(m_Sock, (char*)&m_tData, sizeof(DATA), 0);

	m_iClient = m_tData.num;

	return true;
}

void CNetwork::Input()
{
	if (m_iGameState == GAME_PLAY)
	{
		send(m_Sock, (char*)&m_tKeyData, sizeof(Key_DATA), 0);
	}
}

void CNetwork::Update()
{
	recv(m_Sock, (char*)&m_iGameState, sizeof(int), 0);
	
	if(m_iGameState == GAME_PLAY)
		recv(m_Sock, (char*)&m_tData, sizeof(DATA), 0);

	//if (m_iGameState == GAME_PLAY)
	//{
	//	recv(m_Sock, (char*)&m_tData, sizeof(DATA), 0);
	//}
}

void CNetwork::Render(HDC hdc)
{
	TCHAR str[128] = {};
	if (m_iGameState == GAME_READY)
	{
		wsprintf(str, L"GameState : Game 대기중");
	}

	else if (m_iGameState == GAME_OK)
	{
		wsprintf(str, L"GameState : Game 준비완료");
	}

	else if (m_iGameState == GAME_PLAY)
	{
		wsprintf(str, L"GameState : Game 플레이상태");
	}

	else if (m_iGameState == GAME_END)
	{
		wsprintf(str, L"GameState : Game 종료상태");
	}
	TextOut(hdc, 400, 100, str, lstrlen(str));
}
