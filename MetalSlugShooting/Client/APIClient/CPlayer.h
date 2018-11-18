#pragma once
#include "CGameObject.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	CPlayer(const string& strTag);
	virtual ~CPlayer();

private:
	PlayerInfo	m_tInfo;

	float		m_fSpeed;
	
	bool		m_bChange;		// ÅºÃ¢ ±³È¯
	float		m_fChangeTime;

	class CBullet	m_pBullet[100];

public:
	PlayerInfo GetPlayerInfo()	const;
	int	GetHP() const;
	int GetNum() const;

public:
	void SetPlayerInfo(PlayerInfo info);

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
