#include "Server.h"
#include "Protocol.h"
#include "CTimer.h"

#define SERVERPORT 9000
#define BUFSIZE    5000


//=====================================================================
int g_iGameState = GAME_READY;

CTimer* g_Timer = NULL;				// Timer class

float g_fTime[PLAYERMAX];				// ���� �ð�
float g_fTimeLimit[PLAYERMAX];			// �Ѱ� �ð�

float g_fInputTime = 0.f;


CRITICAL_SECTION cs;
DATA g_tData;
Key_DATA g_tKeyData;
int	g_iConnectNum = 0;
char buf[BUFSIZE] = {};

int g_iBulletCount[PLAYERMAX] = {};
int g_iBoomCount[3] = {};
bool g_bPlayerTimer[PLAYERMAX] = {};
//=====================================================================

// Ŭ���̾�Ʈ�� ������ ��� �������Լ�
DWORD WINAPI ProcessClient(LPVOID arg);

void Logic(int client_id, SOCKET sock);
void Init();
void Delete();
void Input(int id, SOCKET sock, float fTime);			// RecvInput
void Update(int id, SOCKET sock, float fTime);


void DataInit();
void BulletUpdate(int id);
//void BoonUpdate(int id);

int main(int argc, char *argv[])
{
	Init();

	int retval;

	// ���� �ʱ�ȭ
	WSADATA	wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "���� �ʱ�ȭ �ȵ�" << endl;
		return 1;
	}

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		cout << "socket ����" << endl;
	}

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		cout << "bind ����" << endl;
	}

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		cout << "listen ����" << endl;
	}

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;

	while (true)
	{
		if (g_iConnectNum >= PLAYERMAX - 1)
			continue;

		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) 
		{
			cout << "accept ����" << endl;
			break;
		}


		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
		g_tData.num = g_iConnectNum + 1;
		send(client_sock, (char*)&g_tData, sizeof(DATA), 0);			// �ʱ� ������ ����

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);

		// closesocket() - client socket
		if (hThread == NULL)
		{
			closesocket(client_sock);
		}

		else
		{
			// ������ Ŭ���̾�Ʈ ���
			CloseHandle(hThread);
		}

	}
	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();

	return 0;
}

DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen;
	int client_id;

	EnterCriticalSection(&cs);
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	client_id = g_iConnectNum;
	++g_iConnectNum;
	LeaveCriticalSection(&cs);

	while (g_iGameState != GAME_END)
	{
		Logic(client_id, client_sock);
	}

	Delete();

	return 0;
}

void Logic(int client_id, SOCKET sock)
{
	g_Timer->Update();

	//cout << g_Timer->GetLimit() << endl;


	if (g_Timer->GetLimit())
	{
		float fTime = g_Timer->GetDeltaTime();
		cout << fTime << endl;

		Input(client_id, sock, fTime);
		Update(client_id, sock, fTime);
	}
}

void Init()
{
	InitializeCriticalSection(&cs);
	g_Timer = new CTimer;
	g_Timer->Init();
	
	DataInit();
}

void Delete()
{
	delete g_Timer;
	g_Timer = NULL;

	g_bPlayerTimer[0] = false;
	g_bPlayerTimer[1] = false;

	DeleteCriticalSection(&cs);
	cout << "Delete() ȣ��" << endl;
}

void Input(int id, SOCKET sock, float fTime)
{
	if (g_iGameState != GAME_PLAY)
		return;

	//EnterCriticalSection(&cs);
	recvn(sock, (char*)&g_tKeyData, sizeof(Key_DATA), 0);		// Ű�� �ޱ�

	//cout << id + 1 << "P : recvKeyData " << g_tKeyData.key << endl;
	//LeaveCriticalSection(&cs);

	if (id + 1 != g_tKeyData.num)			// id�� �� keydata�� num�� ���������� �Է¹��� Ű ������ ó���� ���Ұſ���!
	{
		return;
	}

	if (g_tKeyData.key == NONE_KEY)
	{
		if (g_tData.player[id].dir == -1)
		{
			g_tData.player[id].state = 0;
		}

		else
		{
			g_tData.player[id].state = 1;
		}
	}

	if (g_tKeyData.key == LEFT_KEY)
	{
		g_tData.player[id].dir = -1;
		g_tData.player[id].x += PLAYER_SPEED * g_tData.player[id].dir;
		g_tData.player[id].state = 4;
	}

	if (g_tKeyData.key == RIGHT_KEY)
	{
		g_tData.player[id].dir = 1;
		g_tData.player[id].x += PLAYER_SPEED * g_tData.player[id].dir;
		g_tData.player[id].state = 5;
	}

	if (g_tKeyData.key == DOWN_KEY)
	{
		if (g_tData.player[id].dir == -1)
		{
			g_tData.player[id].state = 8;
		}

		else
		{
			g_tData.player[id].state = 9;
		}
	}

	if (g_tKeyData.key == SHOOT_KEY)
	{
		if (g_tData.player[id].bulletcnt < 1)
			return;

		g_iBulletCount[id] += 1;												// id�� �Ѿ� ī��Ʈ ���� 
		g_tData.player[id].bulletcnt -= 1;	// id�� ������ �Ѿ� ����
	
		if (g_tData.player[id].dir == -1)
		{
			g_tData.player[id].state = 2;
		}

		else
		{
			g_tData.player[id].state = 3;
		}
		
		g_tData.bullet[id][g_iBulletCount[id]].num = id;						// �� �Ѿ� ������ id
		g_tData.bullet[id][g_iBulletCount[id]].x = g_tData.player[id].x;		// id�� x
		g_tData.bullet[id][g_iBulletCount[id]].y = g_tData.player[id].y;		// id�� y
		g_tData.bullet[id][g_iBulletCount[id]].dir = g_tData.player[id].dir;	// id�� ����
		g_tData.bullet[id][g_iBulletCount[id]].shoot = true;					// �Ѿ��� �߻��!
		
		//cout << "CreateBullet" << endl;
	}

	/*if (g_tKeyData.key == Q_KEY)
	{
		if (g_tData.player[id].boomcnt< 1)
			return;
	}*/

	/*if (g_tKeyData.key == SHOOT_LEFT_KEY)
	{
		if (g_tData.player[id].bulletcnt < 1)
			return;

		g_tData.player[id].dir = -1;
		g_tData.player[id].x += PLAYER_SPEED * g_tData.player[id].dir;

		g_iBulletCount[id] += 1;												// id�� �Ѿ� ī��Ʈ ���� 
		g_tData.player[id].bulletcnt -= 1;										// id�� ������ �Ѿ� ����
		g_tData.bullet[id][g_iBulletCount[id]].num = id;						// �� �Ѿ� ������ id
		g_tData.bullet[id][g_iBulletCount[id]].x = g_tData.player[id].x;		// id�� x
		g_tData.bullet[id][g_iBulletCount[id]].y = g_tData.player[id].y;		// id�� y
		g_tData.bullet[id][g_iBulletCount[id]].dir = g_tData.player[id].dir;	// id�� ����
		g_tData.bullet[id][g_iBulletCount[id]].shoot = true;					// �Ѿ��� �߻��!

		cout << "CreateBullet" << endl; 		
	}

	if (g_tKeyData.key == SHOOT_RIGHT_KEY)
	{
		if (g_tData.player[id].bulletcnt < 1)
			return;
		g_tData.player[id].dir = 1;
		g_tData.player[id].x += PLAYER_SPEED * g_tData.player[id].dir;

		g_iBulletCount[id] += 1;												// id�� �Ѿ� ī��Ʈ ���� 
		g_tData.player[id].bulletcnt -= 1;										// id�� ������ �Ѿ� ����
		g_tData.bullet[id][g_iBulletCount[id]].num = id;						// �� �Ѿ� ������ id
		g_tData.bullet[id][g_iBulletCount[id]].x = g_tData.player[id].x;		// id�� x
		g_tData.bullet[id][g_iBulletCount[id]].y = g_tData.player[id].y;		// id�� y
		g_tData.bullet[id][g_iBulletCount[id]].dir = g_tData.player[id].dir;	// id�� ����
		g_tData.bullet[id][g_iBulletCount[id]].shoot = true;					// �Ѿ��� �߻��!

		cout << "CreateBullet" << endl;
	}
	*/
}

void Update(int id, SOCKET sock, float fTime)
{
	send(sock, (char*)&g_iGameState, sizeof(int), 0);
	cout << id + 1<< "P : SendGameState -> " << g_iGameState << endl;

	if (g_iGameState == GAME_READY)
	{
		//cout << "ConnectCount : " << g_iConnectNum << endl;
		if (g_iConnectNum == 2)
		{
			cout << "Client �Ѵ� ����" << endl;
			g_iGameState = GAME_OK;
		}
	}

	if (g_iGameState == GAME_OK)
	{
		g_fTime[id] += fTime;
		//cout << id + 1 << "P Time" << g_fTime[id] << endl;
		if (g_fTime[id] > 1.f)
		{
			g_iGameState = GAME_PLAY;
			g_fTime[id] = 0.f;
		}
	}

	if (g_iGameState == GAME_PLAY)
	{
		g_fTimeLimit[id] -= fTime;

		BulletUpdate(id);

		//EnterCriticalSection(&cs);
		send(sock, (char*)&g_tData, sizeof(DATA), 0);
		//cout << id + 1<< "P���� ������ ����" << endl;
		//cout << id + 1<< "���� : " << g_tData.player[id].state << endl;
		//LeaveCriticalSection(&cs);
	}
}

void DataInit()
{
	cout << "Data �ʱ�ȭ" << endl;

	g_iConnectNum = 0;
	g_iGameState = GAME_READY;

	g_tData.num = 0;

	g_tData.player[0].num = 1;
	g_tData.player[0].x = 300;
	g_tData.player[0].y = 500;
	g_tData.player[0].magazinecnt = 2;
	g_tData.player[0].boomcnt = 0;
	g_tData.player[0].bulletcnt = 100;
	g_tData.player[0].hp = 30;
	g_tData.player[0].dir = 1;

	g_tData.player[1].num = 2;
	g_tData.player[1].x = 500;
	g_tData.player[1].y = 500;
	g_tData.player[1].magazinecnt = 2;
	g_tData.player[1].boomcnt = 0;
	g_tData.player[1].bulletcnt = 100;
	g_tData.player[1].hp = 100;
	g_tData.player[1].dir = -1;


	for (int i = 0; i < PLAYERMAX; ++i)
	{
		for (int j = 0; j < MAXCOUNT; ++j)
		{
			g_tData.bullet[i][j].x = -200;
			g_tData.bullet[i][j].y = -200;
			g_tData.bullet[i][j].dir = 0;
			g_tData.bullet[i][j].num = i;
			g_tData.bullet[i][j].shoot = false;
		}

		/*for (int j = 0; j < 3; ++j)
		{
			g_tData.boom[i][j].x = -300;
			g_tData.boom[i][j].y = -300;
			g_tData.boom[i][j].dir = 0;
			g_tData.boom[i][j].num = i;
			g_tData.boom[i][j].shoot = false;
		}*/

		g_iBulletCount[i] = 0;
		g_iBoomCount[i] = 0;

		g_fTimeLimit[i] = 90.f;
		g_fTime[i] = 0.f;
	}
}

void BulletUpdate(int id)
{
	for (int i = 0; i < MAXCOUNT; ++i)
	{
		if (!g_tData.bullet[id][i].shoot)
		{
			g_tData.bullet[id][i].x = -200;
			continue;
		}

		g_tData.bullet[id][i].x += BULLET_SPEED * g_tData.bullet[id][i].dir;
	}
}
