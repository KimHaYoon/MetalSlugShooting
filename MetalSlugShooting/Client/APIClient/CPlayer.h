#pragma once
#include "CGameObject.h"
#include "CBullet.h"
#include "CBoom.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	CPlayer(const string& strTag);
	virtual ~CPlayer();

private:
	PlayerInfo	m_tInfo;

	float		m_fSpeed;
	
	bool		m_bChange;		// 탄창 교환
	float		m_fChangeTime;

	CBullet		m_pBullet[MAXCOUNT];
	CBoom		m_pBoom[3];

	bool		m_bScene;		// 굳이 씬 여러번 채워줄 필요없으니까
	int			m_iIdle;

public:
	PlayerInfo GetPlayerInfo()	const;
	int	GetHP() const;
	int GetNum() const;

public:
	void SetPlayerInfo(PlayerInfo info);
	void SetBulletInfo(BulletInfo* info);

private:
	//void CreateBullet();
	void ChangeMagazine();
	void CreateBoom();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};
