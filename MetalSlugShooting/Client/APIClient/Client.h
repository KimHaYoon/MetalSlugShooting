#pragma once
#pragma comment(lib, "ws2_32")
#pragma warning(disable : 4996)
#include <winsock2.h>
#include <crtdbg.h>
#include <unordered_map>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <conio.h>
#include <time.h>
#include <iostream>
#include "Protocol.h"

using namespace std;

#pragma comment(lib, "msimg32")

#define PI		3.141592f
#define DEFAULT_MID_HEIGHT 200.f
#define BOOM_DIST 500.f

// network class 는 싱글톤으로 작성
#define GET_NETWORKINST		CNetwork::GetInstance()
#define DESTROY_NETWORK		CNetwork::DestroyInstance();

// CSceneManager
//#define GET_SCENEMANAGER	CSceneManager::GetInstance();
//#define DESTROY_SCENEMANAGER	CSceneManager::DestroyInstance()

//========================================

typedef struct FLOAT2
{
	float x;
	float y;

	FLOAT2() {};
	FLOAT2(float _x, float _y) : x(_x), y(_y) {};
}float2;

enum ANIMATION_TYPE
{
	AT_LINE,
	AT_ALL
};

enum ANIMATION_OPTION
{
	AO_ONCE,
	AO_LOOP
};

typedef struct _tagAnimation
{
	string				strName;
	ANIMATION_TYPE		eType;
	ANIMATION_OPTION	eOption;
	UINT	iImageWidth;
	UINT	iImageHeight;
	UINT	iFrameMaxX;
	UINT	iFrameMaxY;
	UINT	iFrameX;
	UINT	iFrameY;
	float	fLimitTime;
	float	fTime;
}ANIMATION, *PANIMATION;

// Scene Change
enum SCENE_CHANGE
{
	SC_NONE,
	SC_CHANGE
};

// 사용자 정의 데이터 수신 함수
static int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}