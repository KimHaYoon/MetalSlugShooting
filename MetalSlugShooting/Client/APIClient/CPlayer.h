#pragma once
#include "CGameObject.h"
#include "CBullet.h"
#include "CBoomBullet.h"
#include "CBackground.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	CPlayer(const string& strTag);
	virtual ~CPlayer();

private:
	PlayerInfo	m_tInfo;
	int			m_iClientNum;

	float		m_fSpeed;
	
	bool		m_bChange;		// źâ ��ȯ
	float		m_fChangeTime;

	CBullet		m_pBullet[MAXCOUNT];
	CBoomBullet	m_pBoom[BOOMCOUNT];

	bool		m_bScene;		// ���� �� ������ ä���� �ʿ�����ϱ�
	int			m_iIdle;

	CBackground	m_pPoint;

public:
	PlayerInfo GetPlayerInfo()	const;
	int	GetHP() const;
	int GetNum() const;

public:
	void SetPlayerInfo(PlayerInfo info);
	void SetBulletInfo(BulletInfo* info);
	void SetBoomInfo(BoomInfo* info);
	void SetClientNum(int iNum);

private:
	void ChangeMagazine();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};
