#pragma once
#include "Client.h"

//===============서버관련==================
#define SERVERIP	"192.168.26.147"
#define SERVERPORT	9000
#define BUFSIZE		5000

class CNetwork
{
	// 싱글톤 
private:
	CNetwork();
	~CNetwork();

private:
	static CNetwork*	m_pInst;

public:
	static CNetwork*	GetInstance()
	{
		if (!m_pInst)
		{
			m_pInst = new CNetwork;
		}
		return m_pInst;
	}

	static void DestroyInstance()
	{
		if (m_pInst)
		{
			delete m_pInst;

			m_pInst = NULL;
		}
	}

private:
	// 서버와 통신에 필요한 변수들
	SOCKET				m_Sock;
	int					m_iClient;
	DATA				m_tData;
	Key_DATA			m_tKeyData;
	int					m_iGameState;
	bool				m_bRecv;
	int					m_iTimeLimit;
	int					m_iTime;
	bool				m_bWin[PLAYERMAX];

	string				m_ServerIP;


	// 아이템
	HeliInfo			m_tHeilInfo;
	ITEM_DATA			m_tItem;
	bool				m_bHeli;

public:
	SOCKET	GetSock()		const;
	int		GetClientNum()	const;
	DATA	GetData()		const;
	HeliInfo GetHeliInfo()	const;
	ItemInfo GetItemInfo(int n)	const;
	bool	GetHeli()		const;

	int		GetGameState()	const;
	int		GetTimeLimit()	const;
	int		GetTime()		const;
	bool	GetWin()		const;

	bool	Login(int iNum)	const;

public:
	void	SetClientNum(int Num);
	void	SetKeyData(int key);
	void	SetGameState(int iState);

public:
	bool Init();
	void Input();		// SendKeyData
	void Update();
	void Render(HDC hdc);

private:
	void LoadServerIPAddress();
};

