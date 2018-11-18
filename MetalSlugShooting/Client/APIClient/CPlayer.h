#pragma once
#include "CGameObject.h"
#include "CBullet.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	CPlayer(const string& strTag);
	virtual ~CPlayer();

private:
	PlayerInfo	m_tInfo;

	float		m_fSpeed;
	
	bool		m_bChange;		// źâ ��ȯ
	float		m_fChangeTime;

	CBullet		m_pBullet[MAXCOUNT];

	bool		m_bScene;		// ���� �� ������ ä���� �ʿ�����ϱ�

public:
	PlayerInfo GetPlayerInfo()	const;
	int	GetHP() const;
	int GetNum() const;

public:
	void SetPlayerInfo(PlayerInfo info);
	void SetBulletInfo(BulletInfo* info);

private:
	void CreateBullet();
	void ChangeMagazine();
	void CreateBoom();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};
