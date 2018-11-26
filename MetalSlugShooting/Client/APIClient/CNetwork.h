#pragma once
#include "Client.h"

//===============��������==================
#define SERVERIP	"192.168.26.147"
#define SERVERPORT	9000
#define BUFSIZE		5000

class CNetwork
{
	// �̱��� 
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
	// ������ ��ſ� �ʿ��� ������
	SOCKET				m_Sock;
	int					m_iClient;
	DATA				m_tData;
	Key_DATA			m_tKeyData;
	int					m_iGameState;

	string				m_ServerIP;

public:
	SOCKET	GetSock()	const;
	int		GetClientNum() const;
	DATA	GetData()	const;

	int		GetGameState()	const;

public:
	void	SetClientNum(int Num);
	void	SetKeyData(int key);

public:
	bool Init();
	void Input();		// SendKeyData
	void Update();
	void Render(HDC hdc);

private:
	void LoadServerIPAddress();
};

