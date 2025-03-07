#include "stdafx.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CAnimation.h"
#include "CTexture.h"
#include "CScene.h"
#include "CItem.h"
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

void CPlayer::SetBulletInfo(BulletInfo* info)
{
	for (int i = 0; i < MAXCOUNT; ++i)
	{
		m_pBullet[i].SetPos(info[i].x, info[i].y);
	}
}

void CPlayer::SetBoomInfo(BoomInfo * info)
{
	for (int i = 0; i < BOOMCOUNT; ++i)
	{
		m_pBoom[i].SetInfo(info[i]);
	}
}

void CPlayer::SetClientNum(int iNum)
{
	m_iClientNum = iNum;
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

bool CPlayer::Init()
{
	m_fSpeed = 300.f;
	m_bChange = false;
	m_fChangeTime = 0.f;
	m_bScene = false;
	m_iIdle = -1;		// �ƹ��͵� �Է� �ȹ��� ���� (�ʱ�ȭ)

	CreateAnimation("Man", AT_LINE, AO_LOOP, 2000, 1200, 10, 12, 1.f);
	m_pAnimation->AddLineFrameCount(4);								// ���� ������ ����
	m_pAnimation->AddLineFrameCount(4);
	m_pAnimation->AddLineFrameCount(8);
	m_pAnimation->AddLineFrameCount(8);
	m_pAnimation->AddLineFrameCount(6);
	m_pAnimation->AddLineFrameCount(6);
	m_pAnimation->AddLineFrameCount(10);
	m_pAnimation->AddLineFrameCount(10);
	m_pAnimation->AddLineFrameCount(4);
	m_pAnimation->AddLineFrameCount(4);
	m_pAnimation->AddLineFrameCount(6);
	m_pAnimation->AddLineFrameCount(6);


	m_pAnimation->ChangeAnimation(0);

	
	return true;
}

void CPlayer::Input(float fTime)
{
	if (m_tInfo.num != GET_NETWORKINST->GetClientNum())
	{
		return;
	}

	if (m_pInput->KeyPush("MoveLeft"))
	{
		if (m_iIdle == 0)
			return;

		GET_NETWORKINST->SetKeyData(LEFT_KEY);
	}

	else if (m_pInput->KeyPush("MoveRight"))
	{
		if (m_iIdle == 0)
			return;

		GET_NETWORKINST->SetKeyData(RIGHT_KEY);
	}

	else if (m_pInput->KeyPush("MoveDown"))
	{
		if (m_iIdle == 0)
			return;

		GET_NETWORKINST->SetKeyData(DOWN_KEY); 
	}

	else if (m_pInput->KeyDown("Shoot"))
	{
		if (m_iIdle == 0)
			return;

		GET_NETWORKINST->SetKeyData(SHOOT_KEY);
	}

	else if (m_pInput->KeyDown("ChangeMagazine"))
	{
		if (m_iIdle == 0)
			return;

		GET_NETWORKINST->SetKeyData(R_KEY);
	}

	else if (m_pInput->KeyDown("Boom"))
	{
		if (m_iIdle == 0)
			return;

		GET_NETWORKINST->SetKeyData(Q_KEY);
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
		if (m_iIdle == 0)
			return;

		GET_NETWORKINST->SetKeyData(NONE_KEY);
	}
}

void CPlayer::Update(float fTime)
{
	CGameObject::Update(fTime);

	if (m_pScene && !m_bScene)
	{
		for (int i = 0; i < MAXCOUNT; ++i)
		{
			m_pBullet[i].Init();
			m_pBullet[i].SetTexture("Bullet", m_pScene->GetInst(), m_pScene->GetHdc(), L"Texture/Bullet.bmp", true, RGB(0, 248, 0));
		}		

		for (int i = 0; i < BOOMCOUNT; ++i)
		{
			m_pBoom[i].Init();	
			m_pBoom[i].SetTexture("Boom", m_pScene->GetInst(), m_pScene->GetHdc(), L"Texture/Boom.bmp", true);

		}


		if (m_tInfo.num == GET_NETWORKINST->GetClientNum())
		{
			// �÷��̾� ǥ��
			m_pPoint.Init();
			m_pPoint.SetTexture("Point", m_pScene->GetInst(), m_pScene->GetHdc(), L"Texture/ME.bmp", true, RGB(255.f, 0.f, 255.f));
		}

		m_bScene = true;			// �� ä�������� true 

	}

	m_fPos.x = m_tInfo.x;
	m_fPos.y = m_tInfo.y;

	m_pPoint.SetPos(m_fPos.x + 60.f, m_fPos.y - 130.f);

	if (m_bChange)
	{
		m_fChangeTime += fTime;

		if (m_fChangeTime > 2)			// 3�ʰ� ������ źâ ��ȯ�� ��
		{
			ChangeMagazine();
		}
	}

	for (int i = 0; i < MAXCOUNT; ++i)
	{
		m_pBullet[i].Update(fTime);
	}

	for (int i = 0; i < BOOMCOUNT; ++i)
	{
		m_pBoom[i].Update(fTime);
	}

	if (m_pAnimation->GetCurrentAnimation() == 2 || m_pAnimation->GetCurrentAnimation() == 3 || 
		m_pAnimation->GetCurrentAnimation() == 10 || m_pAnimation->GetCurrentAnimation() == 11)
	{
		m_iIdle = 0;
		if (m_pAnimation->GetFrameX() == 3)
		{
			m_iIdle = 1;
		}
	}

	if (m_iIdle == 0)
		return;

	m_pAnimation->ChangeAnimation(m_tInfo.state);

}

void CPlayer::Render(HDC hDC, float fTime)
{
	CGameObject::Render(hDC, fTime);


	if (GET_NETWORKINST->GetGameState() == GAME_PLAY)
	{
		for (int i = 0; i < MAXCOUNT; ++i)
		{
			m_pBullet[i].Render(hDC, fTime);
		}

		for (int i = 0; i < BOOMCOUNT; ++i)
		{
			m_pBoom[i].Render(hDC, fTime);
		}

		m_pPoint.Render(hDC, fTime);
	}

	TCHAR str[128];
	if (m_tInfo.num == 1)
	{
		wsprintf(str, L"źâ ��:%d, �Ѿ� ��:%d", m_tInfo.magazinecnt, m_tInfo.bulletcnt);
		TextOut(hDC, 50, 100, str, lstrlen(str));

		wsprintf(str, L"��ź ��:%d", m_tInfo.boomcnt);
		TextOut(hDC, 50, 130, str, lstrlen(str));

		//wsprintf(str, L"HP:%d", m_tInfo.hp);
		//TextOut(hDC, 50, 160, str, lstrlen(str));

		//for (int i = 0; i < BOOMCOUNT; ++i)
		//{
		//	wsprintf(str, L"��ź %d : %d %d", i, (int)m_pBoom[i].GetPos().x, (int)m_pBoom[i].GetPos().y);
		//	TextOut(hDC, 50, 200 + i * 30, str, lstrlen(str));
		//}

		//Rectangle(hDC, m_tInfo.x + 80, m_tInfo.y, m_tInfo.x + 120, m_tInfo.y + 100);

	}

	else if (m_tInfo.num == 2)
	{
		wsprintf(str, L"źâ ��:%d, �Ѿ� ��:%d", m_tInfo.magazinecnt, m_tInfo.bulletcnt);
		TextOut(hDC, 1100, 100, str, lstrlen(str));

		wsprintf(str, L"��ź ��:%d", m_tInfo.boomcnt);
		TextOut(hDC, 1100, 130, str, lstrlen(str));

		//wsprintf(str, L"HP:%d", m_tInfo.hp);
		//TextOut(hDC, 1100, 160, str, lstrlen(str));

		//Rectangle(hDC, m_tInfo.x + 80, m_tInfo.y, m_tInfo.x + 120, m_tInfo.y + 100);
	}
}
