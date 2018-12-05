#pragma once

#define PLAYER_WIDTH	150		// �÷��̾� ����ũ��
#define PLAYER_HEIGHT	100		// �÷��̾� ����ũ��
#define PLAYER_POS_Y	550		// �÷��̾� y��ǥ
#define PLAYER_SIT_HEIGHT 50	// �÷��̾� ������ ���� ũ��
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
#define SHOOT_KEY		6		// ���ڸ����� �Ѿ˹߻�
#define SHOOT_LEFT_KEY	7		// ���� �Ѿ� �߻�!
#define SHOOT_RIGHT_KEY 8		// ������ �Ѿ� �߻�!
#define BOOM_LEFT_KEY	9		// �������� ��ź �߻�!
#define BOOM_RIGHT_KEY	10		// ���������� ��ź �߻�!
#define CHANGEM_LEGT_KEY	11	// ���ʿ��� ����!
#define CHANGEM_RIGHt_KEY	12	// �����ʿ��� ����!

#define PLAYERMAX		2
#define MAXCOUNT		50
#define BOOMCOUNT		10

#define DEFAULT_MID_HEIGHT 300.f
#define BOOM_DIST 500.f

#define PLAYER_SPEED	15.f
#define BULLET_SPEED	30.f
#define BULLET_DAMAGE	10.f
#define BOOM_DAMAGE		30.f

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
	int state;			// ������� -> �ִϸ��̼� ��� ������...
};						

struct BulletInfo
{
	int x;				// ��ǥ x
	int y;				// ��ǥ y
	int dir;			// �Ѿ��� ����
	bool shoot;			// �߻��ߴ���
};					

struct ItemInfo
{
	int num;			// ������ ���� 0:HP30ȸ�� 1:HP70ȸ�� 2:źâ 3:����ź
	int x;				// ��ǥ	x
	int y;				// ��ǥ y
};						

struct BoomInfo
{
	float x;
	float y;
	bool shoot;
};

struct DATA
{
	int  num;									// 1P, 2P			4
	PlayerInfo	player[PLAYERMAX];				// �÷��̾� ����		72
	BulletInfo  bullet[PLAYERMAX][MAXCOUNT];	// �Ѿ� ����			3400
	BoomInfo	boom[PLAYERMAX][BOOMCOUNT];				// ����ź ����
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

