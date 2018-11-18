#pragma once

#define PLAYER_WIDTH	150		// �÷��̾� ����ũ��
#define PLAYER_HEIGHT	100		// �÷��̾� ����ũ��
#define BULLET_SIZE		20		// �Ѿ� ũ�� 20x20
#define ITEM_SIZE		50		// ������ ������
#define WIN_WIDTH		1280	// ȭ�� ����ũ��
#define WIN_HEIGHT		720		// ȭ�� ����ũ��

#define NONE_KEY		0
#define Q_KEY			1		// Ű����'Q', ����ź ������
#define R_KEY			2		// Ű���塯R��, źâ��ȯ
#define LEFT_KEY		3		// Ű���� ����Ű ��
#define RIGHT_KEY		4		// Ű���� ����Ű ��
#define DOWN_KEY		5		// Ű���� ����Ű ��
#define SHOOT_KEY		6		// �Ѿ˹߻�

#define PLAYERMAX		2
#define MAXCOUNT		100

#define PLAYER_SPEED	3.f
#define BULLET_SPEED	3.f
#define BULLET_DAMAGE	10.f

struct PlayerInfo
{
	int num;			// 1p���� 2p����
	int x;				// ��ǥ x
	int y;				// ��ǥ y
	int magazinecnt;	// źâ��
	int boomcnt;		// ����ź��
	int bulletcnt;		// �Ѿ˼�
	int hp;				// ���� HP
	int dir;			// �ٶ󺸴� ���� -1:�� 1:��
};						// 32byte

struct BulletInfo
{
	int num;			// �Ѿ��� ����	1:1p 2:2p
	int x;				// ��ǥ x
	int y;				// ��ǥ y
	int dir;			// �Ѿ��� ����
	bool shoot;			// �߻��ߴ���
};						// 17btye

struct ItemInfo
{
	int num;			// ������ ���� 0:HP30ȸ�� 1:HP70ȸ�� 2:źâ 3:����ź
	int x;				// ��ǥ	x
	int y;				// ��ǥ y
};						// 12byte

struct DATA
{
	int  num;									// 1P, 2P
	PlayerInfo	player[PLAYERMAX];				// �÷��̾� ����
	BulletInfo  bullet[PLAYERMAX][MAXCOUNT];	// �Ѿ� ����
};

struct Key_DATA
{
	int num;			// 1P, 2P
	int key;			// key��
};

enum GAMESTATE
{
	GAME_READY = 0,		// ���� �����
	GAME_OK = 1,		// ���� �غ�Ϸ�
	GAME_PLAY = 2,		// ���� �÷��̻���
	GAME_END = -1		// ���� �������
};

enum GAMERESULT
{
	NONE = 0,			// ���� �������
	WIN = 1,			// ��
	LOSE = -1			// ��
};

enum PLAYER_STATE
{
	STAND,				// ���ִ� ����
	SIT,				// �ɾ��ִ� ����
	ATTACK,				// �����ϴ� ����
	DIE					// ���� ����
};

