#include "Server.h"
#include "Protocol.h"

#define SERVERPORT 9000
#define BUFSIZE    5000

int g_iGameState = GAME_READY;


float	g_fTime = 0.f;						// 현재 시간
float	g_fTimeLimit = 90.f;				// 한계 시간
int		g_iConnectNum = 0;		

CRITICAL_SECTION	cs;

unordered_map<int, SOCKET*> g_mapClient;
SOCKET		g_socket[PLAYERMAX] = {};
DATA		g_tData;
Key_DATA	g_tKeyData;
HeliInfo	g_tHeli;
ITEM_DATA	g_tItem;

int g_iBulletCount[PLAYERMAX] = {};
int g_iBoomCount[PLAYERMAX] = {};
bool g_bWin[PLAYERMAX] = {false, false};
bool g_bSend = false;
bool g_bHeli = false;
int g_iHeliDir = 1;
bool g_bItem[4] = { false, false, false, false };
bool g_bItemColl[4] = { false, false, false, false };
int	g_iItemCount = -1;

float g_fTargetPosX[PLAYERMAX][BOOMCOUNT], g_fTargetPosY[PLAYERMAX][BOOMCOUNT];
float g_fOrgPosX[PLAYERMAX][BOOMCOUNT], g_fOrgPosY[PLAYERMAX][BOOMCOUNT];
float g_fMidPosX[PLAYERMAX][BOOMCOUNT], g_fMidPosY[PLAYERMAX][BOOMCOUNT];
float g_fMu[PLAYERMAX][BOOMCOUNT];
float num1[PLAYERMAX][BOOMCOUNT], num2[PLAYERMAX][BOOMCOUNT], mu2[PLAYERMAX][BOOMCOUNT];


RECT PlayerColl[PLAYERMAX] = {};
RECT BulletColl[PLAYERMAX][MAXCOUNT];
RECT BoomColl[PLAYERMAX][BOOMCOUNT];
RECT Win = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
RECT ItemColl[4] = {};

DWORD WINAPI ProcessGame(LPVOID arg);
void Send(const char* buf, int len, string str = NULL);
void Recv(char* buf, int len, string str = NULL);
void RecvKeyAndDataUpdate();
void Init();
void DataInit();
void Update();
void DataUpdate(int id, Key_DATA keydata);
void BulletUpdate(int id);
void BoomUpdate(int id);
void IsWin();
bool Collision(RECT rc1, RECT rc2);

void HeliUpdate();
void ItemUpdate();

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
		g_tData.player[iConnectNum].num = iConnectNum + 1;
		send(client_sock, (char*)&g_tData, sizeof(DATA), 0);			// 초기 데이터 전송


		EnterCriticalSection(&cs);
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

		if (g_iConnectNum == 2)
		{
			if (g_iGameState == GAME_READY)
			{
				g_iGameState = GAME_OK;
				Send((char*)&g_iGameState, sizeof(g_iGameState), "GameState : GAME_READY");
			}

			if (g_iGameState == GAME_OK)
			{
				int iTime = (int)g_fTime;
				if (g_fTime > 4.f)
				{
					g_iGameState = GAME_PLAY;
				}
				Send((char*)&iTime, sizeof(int), "시작 타이머");
				Send((char*)&g_tData, sizeof(DATA), "Data");
				Send((char*)&g_iGameState, sizeof(g_iGameState), "GameState : GAME_OK");
				g_fTime += 0.0333f;
			}

			if (g_iGameState == GAME_PLAY)
			{
				int iTimeLimit = (int)g_fTimeLimit;
				Send((char*)&iTimeLimit, sizeof(int), "제한시간");
				cout << iTimeLimit << endl;

				if (iTimeLimit % 20 == 0 && !g_bHeli)
				{
					g_bHeli = true;
				}

				Send((char*)&g_bHeli, sizeof(g_bHeli), "g_bHeli");

				if (g_bHeli)
				{
					HeliUpdate();
					Send((char*)&g_tHeli, sizeof(g_tHeli), "헬기");
				}

				ItemUpdate();
				Send((char*)&g_tItem, sizeof(g_tItem), "아이템");

				RecvKeyAndDataUpdate();
				Update();
				Send((char*)&g_tData, sizeof(DATA), "Data");

				if (g_fTimeLimit <= 0.f)
				{
					IsWin();
					g_iGameState = GAME_END;
				}

				Send((char*)&g_iGameState, sizeof(g_iGameState), "GameState : GAME_PLAY");
				g_fTimeLimit -= 0.0333f;
			}

			if (g_iGameState == GAME_END)
			{
				Send((char*)&g_iGameState, sizeof(g_iGameState), "GameState : GAME_END");
				if (!g_bSend)
				{
					Send((char*)&g_bWin, sizeof(g_bWin), "승패여부");
					g_bSend = true;
				}
			}
		}

		else
			Send((char*)&g_iGameState, sizeof(g_iGameState), "GameState : GAME_OK 1P만접속");
	}

	return 0;
}

void Send(const char * buf, int len, string str)
{
	int retval;

	for (int i = 0; i < PLAYERMAX; ++i)
	{
		if (g_socket[i] != NULL)
		{
			retval = send(g_socket[i], buf, len, 0);
			if (retval == SOCKET_ERROR)
			{
				cout << i + 1 << "P가 종료";
				return;
			}
			cout << i + 1 << "send " << str << endl;
		}
	}
}

void Recv(char * buf, int len, string str)
{
	int retval;

	for (int i = 0; i < PLAYERMAX; ++i)
	{
		if (g_socket[i] != NULL)
		{
			retval = recv(g_socket[i], buf, len, 0);
			if (retval == SOCKET_ERROR)
			{
				cout << i + 1 << "P가 종료";
				return;
			}
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
			BoomUpdate(i);
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
		PlayerColl[id] = { g_tData.player[id].x + 80, g_tData.player[id].y, g_tData.player[id].x + 120, g_tData.player[id].y + 100 };
	}

	if (keydata.key == LEFT_KEY)
	{
		if (Collision(Win, PlayerColl[id]))
		{
			g_tData.player[id].dir = -1;
			g_tData.player[id].x += PLAYER_SPEED * g_tData.player[id].dir;
			g_tData.player[id].state = 4;
		}

		else
		{
			g_tData.player[id].x -= PLAYER_SPEED * g_tData.player[id].dir;
		}
	}

	if (keydata.key == RIGHT_KEY)
	{
		if (Collision(Win, PlayerColl[id]))
		{
			g_tData.player[id].dir = 1;
			g_tData.player[id].x += PLAYER_SPEED * g_tData.player[id].dir;
			g_tData.player[id].state = 5;
		}

		else
		{
			g_tData.player[id].x -= PLAYER_SPEED * g_tData.player[id].dir;
		}
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
		PlayerColl[id] = { g_tData.player[id].x + 80, g_tData.player[id].y + 50, g_tData.player[id].x + 120, g_tData.player[id].y + 100 };
	}

	if (keydata.key == SHOOT_KEY)
	{
		if (g_tData.player[id].bulletcnt < 1)
			return;

		if (g_iBulletCount[id] >= MAXCOUNT)
			g_iBulletCount[id] = 0;

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

		g_tData.bullet[id][g_iBulletCount[id]].y = g_tData.player[id].y;		// id의 y
		g_tData.bullet[id][g_iBulletCount[id]].dir = g_tData.player[id].dir;	// id의 방향
		g_tData.bullet[id][g_iBulletCount[id]].shoot = true;					// 총알이 발사됨!

		g_iBulletCount[id] += 1;							// id의 총알 카운트 증가 
		//cout << "CreateBullet" << endl;
	}

	if (keydata.key == Q_KEY)
	{
		if (g_tData.player[id].boomcnt< 1)
			return;

		if (g_iBoomCount[id] >= BOOMCOUNT)
			g_iBoomCount[id] = 0;

		g_tData.player[id].boomcnt -= 1;	// id의 보유한 수류탄 감소

		if (g_tData.player[id].dir == -1)
		{
			g_tData.player[id].state = 10;
		}

		else
		{
			g_tData.player[id].state = 11;
		}

		if (id == 0)
		{
			g_fTargetPosX[id][g_iBoomCount[id]] = (PlayerColl[1].left + PlayerColl[1].right) / 2;
			g_fTargetPosY[id][g_iBoomCount[id]] = PLAYER_POS_Y + PLAYER_HEIGHT;
		}

		else
		{
			g_fTargetPosX[id][g_iBoomCount[id]] = (PlayerColl[0].left + PlayerColl[0].right) / 2;
			g_fTargetPosY[id][g_iBoomCount[id]] = PLAYER_POS_Y + PLAYER_HEIGHT;
		}

		g_fOrgPosX[id][g_iBoomCount[id]] = g_tData.player[id].x;
		g_fOrgPosY[id][g_iBoomCount[id]] = PLAYER_POS_Y;
		g_fMidPosX[id][g_iBoomCount[id]] = (g_tData.player[id].x + g_fTargetPosX[id][g_iBoomCount[id]]) / 2;
		g_fMidPosY[id][g_iBoomCount[id]] = PLAYER_POS_Y - DEFAULT_MID_HEIGHT;
		g_fMu[id][g_iBoomCount[id]] = 0.f;

		g_tData.boom[id][g_iBoomCount[id]].shoot = true;					// 수류탄이 발사됨!
		g_tData.boom[id][g_iBoomCount[id]].x = g_tData.player[id].x;
		g_tData.boom[id][g_iBoomCount[id]].y = g_tData.player[id].y;
		g_iBoomCount[id] += 1;
	}

	if (keydata.key == R_KEY)
	{
		if (g_tData.player[id].bulletcnt > 0)
			return;

		if (g_tData.player[id].magazinecnt < 0)
			return;

		g_tData.player[id].bulletcnt += 10;
		g_tData.player[id].magazinecnt -= 1;
	}
}

void IsWin()
{
	if (g_tData.player[0].hp > g_tData.player[1].hp)
	{
		g_bWin[0] = true;
		g_bWin[1] = false;
	}

	else if(g_tData.player[0].hp < g_tData.player[1].hp)
	{
		g_bWin[0] = false;
		g_bWin[1] = true;
	}

	else
	{
		g_bWin[0] = true;
		g_bWin[1] = true;
	}

}

bool Collision(RECT rc1, RECT rc2)
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

void HeliUpdate()
{
	g_tHeli.x += 10 * g_iHeliDir;

	if (g_tHeli.x == 440 && !g_tHeli.drop)
	{
		g_tHeli.drop = true;
		g_iHeliDir = 0; 
		g_iItemCount += 1;
		g_iItemCount %= 4;
		return;
	}

	if (g_tHeli.x >= WIN_WIDTH)
	{
		g_bHeli = false;
		g_tHeli.drop = false;
		g_tHeli.x = -300;
		return;
	}

	if (g_tHeli.drop && !g_tItem.Item[g_iItemCount].enable)
	{
		g_iHeliDir = 1;

		g_tHeli.drop = false;

		g_tItem.Item[g_iItemCount].num = g_iItemCount;
		g_tItem.Item[g_iItemCount].enable = true;
		g_tItem.Item[g_iItemCount].x = g_tHeli.x + 190;
		g_tItem.Item[g_iItemCount].y = 250;

		return;
	}
}

void ItemUpdate()
{
	for (int i = 0; i < 4; ++i)
	{
		if (!g_tItem.Item[i].enable)
		{
			continue;
		}

		g_tItem.Item[i].y += 10;

		if (g_tItem.Item[i].y >= PLAYER_POS_Y + 40)
		{
			g_tItem.Item[i].y = PLAYER_POS_Y + 40;
		}
	}
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

		for (int j = 0; j < BOOMCOUNT; ++j)
		{
			if (g_tData.boom[i][j].shoot)
			{
				BoomColl[i][j] = { (LONG)g_tData.boom[i][j].x, (LONG)g_tData.boom[i][j].y, (LONG)g_tData.boom[i][j].x + 38, (LONG)g_tData.boom[i][j].y + 38 };
			}
		}

		//PlayerColl[i] = { g_tData.player[i].x + 80, g_tData.player[i].y, g_tData.player[i].x + 120, g_tData.player[i].y + 100 };
	}
	
	for (int i = 0; i < 4; ++i)
	{
		ItemColl[i] = { g_tItem.Item[i].x, g_tItem.Item[i].y , g_tItem.Item[i].x + 50, g_tItem.Item[i].y + 50 };

		for (int j = 0; j < PLAYERMAX; ++j)
		{
			if (Collision(ItemColl[i], PlayerColl[j]) && g_tItem.Item[i].enable)
			{
				if (g_tItem.Item[i].num == 0)
				{
					g_tData.player[j].hp += 30;
					if (g_tData.player[j].hp > 100)
						g_tData.player[j].hp = 100;

				}

				else if (g_tItem.Item[i].num == 1)
				{
					g_tData.player[j].hp += 70;
					if (g_tData.player[j].hp > 100)
						g_tData.player[j].hp = 100;
				}

				else if (g_tItem.Item[i].num == 2)
				{
					g_tData.player[j].magazinecnt += 1;
				}

				else if (g_tItem.Item[i].num == 3)
				{
					g_tData.player[j].boomcnt += 1;
				}

				g_tItem.Item[i].enable = false;
			}
		}
	}
	
	
	for (int j = 0; j < MAXCOUNT; ++j)
	{
		// 총알과 플레이어 충돌
		if (Collision(PlayerColl[0], BulletColl[1][j]) && g_tData.bullet[1][j].shoot)
		{
			if (g_tData.player[0].hp > 0)
				g_tData.player[0].hp -= BULLET_DAMAGE;

			g_tData.bullet[1][j].shoot = false;
		}

		if (Collision(PlayerColl[1], BulletColl[0][j]) && g_tData.bullet[0][j].shoot)
		{
			if (g_tData.player[1].hp > 0)
				g_tData.player[1].hp -= BULLET_DAMAGE;

			g_tData.bullet[0][j].shoot = false;
		}

		// 총알이 맵밖으로벗어나는지 체크
		for (int i = 0; i < PLAYERMAX; ++i)
		{
			if (!Collision(Win, BulletColl[i][j]) && g_tData.bullet[i][j].shoot)
			{
				g_tData.bullet[i][j].x = -200;
				g_tData.bullet[i][j].y = -200;
				g_tData.bullet[i][j].dir = 0;
				g_tData.bullet[i][j].shoot = false;
			}

		}
	}

	for (int j = 0; j < BOOMCOUNT; ++j)
	{
		// 수류탄과 플레이어 충돌
		if (Collision(PlayerColl[0], BoomColl[1][j]) && g_tData.boom[1][j].shoot)
		{
			if (g_tData.player[0].hp > 0)
			{
				g_tData.player[0].hp -= BOOM_DAMAGE;

				if (g_tData.player[0].hp <= 0)
				{
					g_tData.player[0].hp = 0;
				}
			}

			g_tData.boom[1][j].shoot = false;
		}

		if (Collision(PlayerColl[1], BoomColl[0][j]) && g_tData.boom[0][j].shoot)
		{
			if (g_tData.player[1].hp > 0)
			{
				g_tData.player[1].hp -= BOOM_DAMAGE;

				if (g_tData.player[1].hp <= 0)
				{
					g_tData.player[1].hp = 0;
				}
			}

			g_tData.boom[0][j].shoot = false;
		}

		for (int i = 0; i < PLAYERMAX; ++i)
		{
			// 수류탄이 맵밖으로벗어나는지 체크
			if (!Collision(Win, BoomColl[i][j]) && g_tData.boom[i][j].shoot)
			{
				g_tData.boom[i][j].x = -300;
				g_tData.boom[i][j].y = -300;
				g_tData.boom[i][j].shoot = false;
			}

			// 수류탄이 땅에 닿았는지 체크
			if (BoomColl[i][j].bottom >= PLAYER_POS_Y + PLAYER_HEIGHT && g_tData.boom[i][j].shoot)
			{
				g_tData.boom[i][j].x = -300;
				g_tData.boom[i][j].y = -300;
				g_tData.boom[i][j].shoot = false;
			}

			if (g_tData.player[i].hp <= 0)
			{
				IsWin();
				g_iGameState = GAME_END;
			}
		}
	}
}

void DataInit()
{
	cout << "Data 초기화" << endl;

	srand((unsigned)time(NULL));

	//g_iConnectNum = 0;
	g_tData.num = 0;
	g_tData.player[0].num = -1;
	g_tData.player[0].x = 300;
	g_tData.player[0].y = PLAYER_POS_Y;
	g_tData.player[0].magazinecnt = 2;
	g_tData.player[0].boomcnt = 2;
	g_tData.player[0].bulletcnt = 10;
	g_tData.player[0].hp = 100;
	g_tData.player[0].dir = 1;

	g_tData.player[1].num = -1;
	g_tData.player[1].x = 700;
	g_tData.player[1].y = PLAYER_POS_Y;
	g_tData.player[1].magazinecnt = 2;
	g_tData.player[1].boomcnt = 2;
	g_tData.player[1].bulletcnt = 10;
	g_tData.player[1].hp = 100;
	g_tData.player[1].dir = -1;


	for (int i = 0; i < PLAYERMAX; ++i)
	{
		for (int j = 0; j < MAXCOUNT; ++j)
		{
			g_tData.bullet[i][j].x = -200;
			g_tData.bullet[i][j].y = -200;
			g_tData.bullet[i][j].dir = 0;
			g_tData.bullet[i][j].shoot = false;
		}

		for (int j = 0; j < BOOMCOUNT; ++j)
		{
			g_tData.boom[i][j].x = -300;
			g_tData.boom[i][j].y = -300;
			g_tData.boom[i][j].shoot = false;
		}

		g_iBulletCount[i] = 0;
		g_iBoomCount[i] = 0;
	}

	g_tHeli.drop = false;
	g_tHeli.x = -300;

}

void BulletUpdate(int id)
{
	for (int i = 0; i < MAXCOUNT; ++i)
	{
		if (!g_tData.bullet[id][i].shoot)
		{
			g_tData.bullet[id][i].x = -200;
			g_tData.bullet[id][i].y = -200;
			g_tData.bullet[id][i].dir = 0;

			continue;
		}

		g_tData.bullet[id][i].x += BULLET_SPEED * g_tData.bullet[id][i].dir;
	}
}

void BoomUpdate(int id)
{	
	for (int i = 0; i < BOOMCOUNT; ++i)
	{
		if (!g_tData.boom[id][i].shoot)
		{
			g_tData.boom[id][i].x = -300;
			g_tData.boom[id][i].y = -300;

			continue;
		}

		g_fMu[id][i] += 0.05f;

		num1[id][i] = 1 - g_fMu[id][i];
		num2[id][i] = num1[id][i] * num1[id][i];
		mu2[id][i] = g_fMu[id][i] * g_fMu[id][i];

		g_tData.boom[id][i].x = g_fOrgPosX[id][i] * num2[id][i] + 2 * g_fMidPosX[id][i] * num1[id][i] * g_fMu[id][i] + g_fTargetPosX[id][i] * mu2[id][i];
		g_tData.boom[id][i].y = g_fOrgPosY[id][i] * num2[id][i] + 2 * g_fMidPosY[id][i] * num1[id][i] * g_fMu[id][i] + g_fTargetPosY[id][i] * mu2[id][i];
	}
}