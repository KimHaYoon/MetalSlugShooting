#pragma once

#define PLAYER_WIDTH	150		// 플레이어 가로크기
#define PLAYER_HEIGHT	100		// 플레이어 세로크기
#define PLAYER_POS_Y	550		// 플레이어 y좌표
#define PLAYER_SIT_HEIGHT 50	// 플레이어 앉을때 세로 크기
#define BULLET_SIZE		20		// 총알 크기 20x20
#define ITEM_SIZE		50		// 아이템 사이즈
#define WIN_WIDTH		1280	// 화면 가로크기
#define WIN_HEIGHT		720		// 화면 세로크기

#define NONE_KEY		0
#define Q_KEY			1		// 키보드'Q', 수류탄 던지기
#define R_KEY			2		// 키보드’R’, 탄창교환
#define LEFT_KEY		3		// 키보드 방향키 ←
#define RIGHT_KEY		4		// 키보드 방향키 →
#define DOWN_KEY		5		// 키보드 방향키 ↓
#define SHOOT_KEY		6		// 제자리에서 총알발사

#define PLAYERMAX		2
#define MAXCOUNT		10
#define BOOMCOUNT		3

#define DEFAULT_MID_HEIGHT 300.f
#define BOOM_DIST 500.f

#define PLAYER_SPEED	15.f
#define BULLET_SPEED	30.f
#define BULLET_DAMAGE	10.f
#define BOOM_DAMAGE		30.f

struct PlayerInfo
{
	int num;			// 1p인지 2p인지
	int x;				// 좌표 x
	int y;				// 좌표 y
	int magazinecnt;	// 탄창수
	int boomcnt;		// 수류탄수
	int bulletcnt;		// 총알수
	int hp;				// 현재 HP
	int dir;			// 바라보는 방향 -1:왼 1:오
	int state;			// 현재상태 -> 애니메이션 출력 때문에...
};						

struct BulletInfo
{
	int x;				// 좌표 x
	int y;				// 좌표 y
	int dir;			// 총알의 방향
	bool shoot;			// 발사했는지
};					

struct ItemInfo
{
	int num;			// 아이템 정보 0:HP30회복 1:HP70회복 2:탄창 3:수류탄
	int x;				// 좌표	x
	int y;				// 좌표 y
	bool enable;		// 그려지는 건지
};						

struct BoomInfo
{
	float x;
	float y;
	bool shoot;
};

struct HeliInfo
{
	int x;				// 좌표 x
	bool drop;			// 아이템 떨어트릴건지
};

struct DATA
{
	int  num;									// 1P, 2P			
	PlayerInfo	player[PLAYERMAX];				// 플레이어 정보		
	BulletInfo  bullet[PLAYERMAX][MAXCOUNT];	// 총알 정보			
	BoomInfo	boom[PLAYERMAX][BOOMCOUNT];		// 수류탄 정보		
};

struct ITEM_DATA
{
	ItemInfo	Item[4];
};

struct Key_DATA
{
	int num;			// 1P, 2P
	int key;			// key값
};

enum GAMESTATE
{
	GAME_READY = 0,		// 게임 대기중
	GAME_OK = 1,		// 게임 준비완료
	GAME_PLAY = 2,		// 게임 플레이상태
	GAME_END = -1		// 게임 종료상태
};

enum GAMERESULT
{
	NONE = 0,			// 아직 결과없음
	WIN = 1,			// 승
	LOSE = -1			// 패
};

enum PLAYER_STATE
{
	STAND,				// 서있는 상태
	SIT,				// 앉아있는 상태
	ATTACK,				// 공격하는 상태
	DIE					// 죽은 상태
};

