#include "Server.h"
#include "Protocol.h"

#define SERVERPORT 9000
#define BUFSIZE    5000

int g_iGameState = GAME_READY;


float	g_fTime = 0.f;						// 현재 시간
float	g_fTimeLimit = 90.f;			// 한계 시간
int		g_iConnectNum = 0;		

CRITICAL_SECTION	cs;

unordered_map<int, SOCKET*> g_mapClient;
SOCKET		g_socket[PLAYERMAX] = {};
DATA		g_tData;
Key_DATA	g_tKeyData;

int g_iBulletCount[PLAYERMAX] = {};
int g_iBoomCount[3] = {};

RECT PlayerColl[PLAYERMAX] = {};
RECT BulletColl[PLAYERMAX][MAXCOUNT];

DWORD WINAPI ProcessGame(LPVOID arg);
void Send(const char* buf, int len, string str = NULL);
void Recv(char* buf, int len, string str = NULL);
void RecvKeyAndDataUpdate();
void Init();
void DataInit();
void Update();
void DataUpdate(int id, Key_DATA keydata);
void BulletUpdate(int id);
bool Collisition(RECT rc1, RECT rc2);

int main(int argc, char *argv[])
{
	Init();

	int retval;

	// 윈속 초기화
	WSADATA	wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "윈속 초기화 안됨" << endl;
		return 1;
	}

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		cout << "socket 에러" << endl;
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
		cout << "bind 에러" << endl;
	}

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		cout << "listen 에러" << endl;
	}


	while (true)
	{
		// 데이터 통신에 사용할 변수
		SOCKET client_sock;
		SOCKADDR_IN clientaddr;
		int addrlen;
		HANDLE hThread;

		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			cout << "accept 에러" << endl;
			break;
		}


		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		int iConnectNum = g_iConnectNum;

		g_tData.num = iConnectNum + 1;
		send(client_sock, (char*)&g_tData, sizeof(DATA), 0);			// 초기 데이터 전송


		EnterCriticalSection(&cs);
		//g_mapClient.insert(make_pair(iConnectNum, &client_sock));
		g_socket[iConnectNum] = client_sock;
		LeaveCriticalSection(&cs);

		if (g_iConnectNum == 0)
		{
			CreateThread(NULL, 0, ProcessGame, NULL, 0, NULL);
		}

		++g_iConnectNum;

	}
	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();

	DeleteCriticalSection(&cs);

	return 0;
}

DWORD WINAPI ProcessGame(LPVOID arg)
{
	DWORD dwTime = GetTickCount();

	while (true)
	{
		DWORD dwNow = GetTickCount();

		if (dwTime + 33 > dwNow)
		{
			continue;
		}

		dwTime = dwNow;

		/*EnterCriticalSection(&cs);
		int iClientCount = g_mapClient.size();
		LeaveCriticalSection(&cs);

		auto iter = g_mapClient.begin();
		auto iterEnd = g_mapClient.end();*/

		if (g_iConnectNum == 2)
		{
			if (g_iGameState == GAME_READY)
			{
				g_iGameState = GAME_OK;
				Send((char*)&g_iGameState, sizeof(g_iGameState), "GameState : GAME_OK");
			}

			if (g_iGameState == GAME_OK)
			{
				g_fTime += 0.033f;
				if (g_fTime > 3.f)
				{
					g_iGameState = GAME_PLAY;
					Send((char*)&g_iGameState, sizeof(g_iGameState), "GameState : GAME_PLAY");
				}
			}

			if (g_iGameState == GAME_PLAY)
			{
				// key값 받기
				//Recv((char*)&g_tKeyData, sizeof(g_tKeyData), "KeyData");
				RecvKeyAndDataUpdate();
				Update();
				Send((char*)&g_tData, sizeof(DATA), "Data");
			}
		}

		else
			Send((char*)&g_iGameState, sizeof(g_iGameState), "GameState : GAME_OK 1p만접속");
	}

	return 0;
}

void Send(const char * buf, int len, string str)
{
	/*auto iter = g_mapClient.begin();
	auto iterEnd = g_mapClient.end();

	for (iter; iter != iterEnd; ++iter)
	{
		send(*(iter->second), buf, len, 0);
		cout << iter->first + 1 << "send " << str << endl;
	}*/

	for (int i = 0; i < PLAYERMAX; ++i)
	{
		if (g_socket[i] != NULL)
		{
			send(g_socket[i], buf, len, 0);
			cout << i + 1 << "send " << str << endl;
		}
	}
}

void Recv(char * buf, int len, string str)
{
	/*auto iter = g_mapClient.begin();
	auto iterEnd = g_mapClient.end();

	for (iter; iter != iterEnd; ++iter)
	{
		recv(*(iter->second), buf, len, 0);
		cout << iter->first + 1 << "recv " << str << endl;
	}*/

	for (int i = 0; i < PLAYERMAX; ++i)
	{
		if (g_socket[i] != NULL)
		{
			recv(g_socket[i], buf, len, 0);
			cout << i + 1 << "recv " << str << endl;
		}
	}
}

void RecvKeyAndDataUpdate()
{
	for (int i = 0; i < PLAYERMAX; ++i)
	{
		if (g_socket[i] != NULL)
		{
			recv(g_socket[i], (char*)&g_tKeyData, sizeof(g_tKeyData), 0);
			DataUpdate(i, g_tKeyData);
			BulletUpdate(i);
		}
	}
}

void Init()
{
	InitializeCriticalSection(&cs);

	DataInit();
}

void DataUpdate(int id, Key_DATA keydata)
{
	if (keydata.key == NONE_KEY)
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

	if (keydata.key == LEFT_KEY)
	{
		g_tData.player[id].dir = -1;
		g_tData.player[id].x += PLAYER_SPEED * g_tData.player[id].dir;
		g_tData.player[id].state = 4;
	}

	if (keydata.key == RIGHT_KEY)
	{
		g_tData.player[id].dir = 1;
		g_tData.player[id].x += PLAYER_SPEED * g_tData.player[id].dir;
		g_tData.player[id].state = 5;
	}

	if (keydata.key == DOWN_KEY)
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

	if (keydata.key == SHOOT_KEY)
	{
		if (g_tData.player[id].bulletcnt < 1)
			return;

		if (g_iBulletCount[id] >= MAXCOUNT)
			g_iBulletCount[id] = 0;
		g_iBulletCount[id] += 1;							// id의 총알 카운트 증가 

		g_tData.player[id].bulletcnt -= 1;	// id의 보유한 총알 감소

		if (g_tData.player[id].dir == -1)
		{
			g_tData.player[id].state = 2;
			g_tData.bullet[id][g_iBulletCount[id]].x = g_tData.player[id].x;		// id의 x
		}

		else
		{
			g_tData.player[id].state = 3;
			g_tData.bullet[id][g_iBulletCount[id]].x = g_tData.player[id].x + 130;		// id의 x
		}

		g_tData.bullet[id][g_iBulletCount[id]].num = id;						// 이 총알 주인은 id
		g_tData.bullet[id][g_iBulletCount[id]].y = g_tData.player[id].y;		// id의 y
		g_tData.bullet[id][g_iBulletCount[id]].dir = g_tData.player[id].dir;	// id의 방향
		g_tData.bullet[id][g_iBulletCount[id]].shoot = true;					// 총알이 발사됨!

		//cout << "CreateBullet" << endl;
	}
}

bool Collisition(RECT rc1, RECT rc2)
{
	if (rc1.left > rc2.right)
		return false;

	if (rc1.right < rc2.left)
		return false;

	if (rc1.top > rc2.bottom)
		return false;

	if (rc1.bottom < rc2.top)
		return false;

	return true;
}

void Update()
{
	for (int i = 0; i < PLAYERMAX; ++i)
	{
		for (int j = 0; j < MAXCOUNT; ++j)
		{
			if (g_tData.bullet[i][j].shoot)
			{
				BulletColl[i][j] = { g_tData.bullet[i][j].x, g_tData.bullet[i][j].y,g_tData.bullet[i][j].x + 20, g_tData.bullet[i][j].y + 20 };
			}
		}

		PlayerColl[i] = { g_tData.player[i].x + 20, g_tData.player[i].y, g_tData.player[i].x + 80, g_tData.player[i].y + 100 };
		cout << PlayerColl[i].left << ", " << PlayerColl[i].top << ", " << PlayerColl[i].right << ", " << PlayerColl[i].bottom << endl;
	}
	
	for (int j = 0; j < MAXCOUNT; ++j)
	{
		if (Collisition(PlayerColl[0], BulletColl[1][j]) && g_tData.bullet[1][j].shoot)
		{
			g_tData.player[0].hp -= BULLET_DAMAGE;
			g_tData.bullet[1][j].shoot = false;
		}

		if (Collisition(PlayerColl[1], BulletColl[0][j]) && g_tData.bullet[0][j].shoot)
		{
			g_tData.player[1].hp -= BULLET_DAMAGE;
			g_tData.bullet[0][j].shoot = false;
		}

		RECT Win = { 0, 0, WIN_WIDTH, WIN_HEIGHT };

		for (int i = 0; i < PLAYERMAX; ++i)
		{
			if (!Collisition(Win, BulletColl[i][j]))
			{
				g_tData.bullet[i][j].x = -200;
				g_tData.bullet[i][j].y = -200;
				g_tData.bullet[i][j].dir = 0;
				g_tData.bullet[i][j].num = i;
				g_tData.bullet[i][j].shoot = false;
			}
		}
	}
}

void DataInit()
{
	cout << "Data 초기화" << endl;

	g_iConnectNum = 0;

	g_tData.num = 0;

	g_tData.player[0].num = 1;
	g_tData.player[0].x = 300;
	g_tData.player[0].y = 500;
	g_tData.player[0].magazinecnt = 2;
	g_tData.player[0].boomcnt = 0;
	g_tData.player[0].bulletcnt = 100;
	g_tData.player[0].hp = 100;
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
		g_iGameState = GAME_READY;
		//PlayerColl[i] = { g_tData.player[i].x + 20, g_tData.player[i].y, g_tData.player[i].x + 80, g_tData.player[i].y + 100 };
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
