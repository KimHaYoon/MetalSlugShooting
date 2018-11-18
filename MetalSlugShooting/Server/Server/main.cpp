#include "Server.h"
#include "Protocol.h"
#include "CTimer.h"

#define SERVERPORT 9000
#define BUFSIZE    100000


//=====================================================================
int g_iGameState = GAME_READY;

CTimer* g_Timer = NULL;				// Timer class

float g_fTime = 0.f;				// 현재 시간
float g_fTimeLimit = 90.f;			// 한계 시간

float g_fInputTime = 0.f;


CRITICAL_SECTION cs;
DATA g_tData;
Key_DATA g_tKeyData;
int	g_iConnectNum = 0;
char buf[BUFSIZE] = {};
//=====================================================================

// 클라이언트와 데이터 통신 스레드함수
DWORD WINAPI ProcessClient(LPVOID arg);

void Logic(int client_id, SOCKET sock);
void Init();
void Delete();
void Input(int id, SOCKET sock, float fTime);			// RecvInput
void Update(int id, SOCKET sock, float fTime);


void DataInit();

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

	// 데이터 통신에 사용할 변수
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
			cout << "accept 에러" << endl;
			break;
		}


		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
		g_tData.num = g_iConnectNum + 1;
		send(client_sock, (char*)&g_tData, sizeof(DATA), 0);			// 초기 데이터 전송

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);

		// closesocket() - client socket
		if (hThread == NULL)
		{
			closesocket(client_sock);
		}

		else
		{
			// 종료한 클라이언트 출력
			CloseHandle(hThread);
		}

	}
	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();

	return 0;
}

DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen;
	int client_id;

	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	client_id = g_iConnectNum;
	++g_iConnectNum;

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

	float fTime = g_Timer->GetDeltaTime();
	cout << fTime << endl;
	Input(client_id, sock, fTime);
	Update(client_id, sock, fTime);
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

	DeleteCriticalSection(&cs);
	cout << "Delete() 호출" << endl;
}

void Input(int id, SOCKET sock, float fTime)
{
	if (g_iGameState != GAME_PLAY)
		return;

	recvn(sock, (char*)&g_tKeyData, sizeof(Key_DATA), 0);

	cout << id + 1 << "P : recvKeyData " << g_tKeyData.key << endl;

	if (g_tKeyData.key == LEFT_KEY)
	{
		g_tData.player[id].dir = -1;
		g_tData.player[id].x -= PLAYER_SPEED;
	}

	if (g_tKeyData.key == RIGHT_KEY)
	{
		g_tData.player[id].dir = 1;
		g_tData.player[id].x += PLAYER_SPEED;
	}

	if (g_tKeyData.key == SHOOT_KEY)
	{
		if (g_tData.player[id].bulletcnt < 1)
			return;
		g_tData.player[id].bulletcnt -= 1;
		cout << "CreateBullet" << endl;
	}
}

void Update(int id, SOCKET sock, float fTime)
{
	send(sock, (char*)&g_iGameState, sizeof(int), 0);
	cout << id + 1 << "P : SendGameState -> " << g_iGameState << endl;

	if (g_iGameState == GAME_READY)
		g_iGameState = GAME_OK;

	if (g_iGameState == GAME_OK)
	{
		g_fTime += fTime;
		cout << g_fTime << endl;
		if (g_fTime > 10.f)
		{
			g_iGameState = GAME_PLAY;
		}
	}

	if (g_iGameState == GAME_PLAY)
	{
		g_fTime -= fTime;

		for (int i = 0; i < g_tData.bulletcnt[id]; ++i)
		{
			g_tData.bullet[id][i].x += BULLET_SPEED * g_tData.bullet[id][i].dir;
		}

		send(sock, (char*)&g_tData, sizeof(DATA), 0);
		cout << id + 1 << "P : SendData" << endl;
	}
}

void DataInit()
{
	cout << "Data 초기화" << endl;

	g_iConnectNum = 0;
	g_iGameState = GAME_READY;

	g_tData.num = 0;

	g_tData.player[0].num = 1;
	g_tData.player[0].x = 300;
	g_tData.player[0].y = 500;
	g_tData.player[0].magazinecnt = 2;
	g_tData.player[0].boomcnt = 0;
	g_tData.player[0].bulletcnt = 10;
	g_tData.player[0].hp = 100;
	g_tData.player[0].dir = 1;

	g_tData.player[1].num = 2;
	g_tData.player[1].x = 900;
	g_tData.player[1].y = 500;
	g_tData.player[1].magazinecnt = 2;
	g_tData.player[1].boomcnt = 0;
	g_tData.player[1].bulletcnt = 10;
	g_tData.player[1].hp = 100;
	g_tData.player[1].dir = -1;


	g_tData.bullet[0] = NULL;

	g_tData.bullet[1] = NULL;
}
