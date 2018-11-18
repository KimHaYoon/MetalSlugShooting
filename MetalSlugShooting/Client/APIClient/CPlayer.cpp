#include "stdafx.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CAnimation.h"
#include "CTexture.h"
#include "CBullet.h"
#include "CScene.h"
#include "CItem.h"
#include "CBoom.h"
#include "CNetwork.h"

CPlayer::CPlayer()
{
}

CPlayer::CPlayer(const string & strTag) : CGameObject(strTag)
{
}


CPlayer::~CPlayer()
{
}

PlayerInfo CPlayer::GetPlayerInfo() const
{
	return m_tInfo;
}

int CPlayer::GetHP() const
{
	return m_tInfo.hp;
}

int CPlayer::GetNum() const
{
	return m_tInfo.num;
}

void CPlayer::SetPlayerInfo(PlayerInfo info)
{
	m_tInfo = info;
}

void CPlayer::CreateBullet()
{
	if (m_tInfo.bulletcnt < 1)
		return;

	CBullet* pBullet = new CBullet("Bullet");

	if (!pBullet->Init(m_fPos, m_tInfo.dir))
	{
		if (pBullet)
		{
			delete pBullet;

			pBullet = NULL;
		}
	}
	pBullet->SetTexture("Bullet", m_pScene->GetInst(), m_pScene->GetHdc(), L"Texture/Bullet.bmp", true, RGB(0, 248, 0));

	m_pScene->AddObject(pBullet);
	m_tInfo.bulletcnt -= 1;
}

void CPlayer::ChangeMagazine()
{
	m_bChange = false;
	m_fChangeTime = 0.f;

	if (m_tInfo.bulletcnt > 0)
		return;
	if (m_tInfo.magazinecnt < 1)
		return;

	m_tInfo.magazinecnt -= 1;

	m_tInfo.bulletcnt += 10;
}

void CPlayer::CreateBoom()
{
	if (m_tInfo.boomcnt < 1)
		return;

	CBoom* pBoom = new CBoom("Boom");

	if (!pBoom->Init(m_fPos, m_tInfo.dir))
	{
		if (pBoom)
		{
			delete pBoom;

			pBoom = NULL;
		}
	}

	pBoom->SetTexture("Boom", m_pScene->GetInst(), m_pScene->GetHdc(), L"Texture/Boom.bmp", true);


	m_pScene->AddObject(pBoom);
	m_tInfo.boomcnt -= 1;
}

bool CPlayer::Init()
{
	m_fSpeed = 300.f;
	m_bChange = false;
	m_fChangeTime = 0.f;

	CreateAnimation("Man", AT_LINE, AO_LOOP, 1200, 1000, 6, 5, 1.f);
	m_pAnimation->AddLineFrameCount(4);
	m_pAnimation->AddLineFrameCount(6);
	m_pAnimation->AddLineFrameCount(6);
	m_pAnimation->AddLineFrameCount(2);
	m_pAnimation->AddLineFrameCount(2);

	m_pAnimation->ChangeAnimation(0);
	return true;
}

void CPlayer::Input(float fTime)
{
	if (m_tInfo.num != GET_NETWORKINST->GetClientNum())
	{
		int a = 0;
		return;
	}

	if (m_pInput->KeyPush("MoveLeft"))
	{
		GET_NETWORKINST->SetKeyData(LEFT_KEY);
		m_pAnimation->ChangeAnimation(1);
	}

	else if (m_pInput->KeyPush("MoveRight"))
	{
		GET_NETWORKINST->SetKeyData(RIGHT_KEY);
		m_pAnimation->ChangeAnimation(2);
	}

	else if (m_pInput->KeyDown("Shoot"))
	{
		GET_NETWORKINST->SetKeyData(SHOOT_KEY);
	}

	else if (m_pInput->KeyDown("ChangeMagazine"))
	{
		m_bChange = true;
	}

	else if (m_pInput->KeyDown("Boom"))
	{
		CreateBoom();
	}

	else if (m_pInput->KeyDown("HPUP"))
	{
		if (m_tInfo.hp < 100)
			++m_tInfo.hp;
	}

	else if (m_pInput->KeyDown("HPDOWN"))
	{
		if (m_tInfo.hp > 1)
			--m_tInfo.hp;
	}

	else
	{
		GET_NETWORKINST->SetKeyData(NONE_KEY);
		m_pAnimation->ChangeAnimation(0);
	}
}

void CPlayer::Update(float fTime)
{
	CGameObject::Update(fTime);

	m_fPos.x = m_tInfo.x;
	m_fPos.y = m_tInfo.y;

	if (m_bChange)
	{
		m_fChangeTime += fTime;

		if (m_fChangeTime > 2)			// 3ÃÊ°¡ Áö³ª¸é ÅºÃ¢ ±³È¯ÀÌ µÊ
		{
			ChangeMagazine();
		}
	}
}

void CPlayer::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);


	TCHAR str[128];
	if (m_tInfo.num == 1)
	{
		wsprintf(str, L"ÅºÃ¢ ¼ö:%d, ÃÑ¾Ë ¼ö:%d", m_tInfo.magazinecnt, m_tInfo.bulletcnt);
		TextOut(hDC, 50, 100, str, lstrlen(str));

		wsprintf(str, L"ÆøÅº ¼ö:%d", m_tInfo.boomcnt);
		TextOut(hDC, 50, 130, str, lstrlen(str));

		wsprintf(str, L"HP:%d", m_tInfo.hp);
		TextOut(hDC, 50, 160, str, lstrlen(str));
	}

	else if (m_tInfo.num == 2)
	{
		wsprintf(str, L"ÅºÃ¢ ¼ö:%d, ÃÑ¾Ë ¼ö:%d", m_tInfo.magazinecnt, m_tInfo.bulletcnt);
		TextOut(hDC, 1100, 100, str, lstrlen(str));

		wsprintf(str, L"ÆøÅº ¼ö:%d", m_tInfo.boomcnt);
		TextOut(hDC, 1100, 130, str, lstrlen(str));

		wsprintf(str, L"HP:%d", m_tInfo.hp);
		TextOut(hDC, 1100, 160, str, lstrlen(str));
	}

}
