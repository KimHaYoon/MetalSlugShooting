#include "stdafx.h"
#include "CBoom.h"
#include "CAnimation.h"


CBoom::CBoom()
{
}

CBoom::CBoom(const string & strTag) : CItem(strTag)
{
}

CBoom::~CBoom()
{
}

bool CBoom::Init()
{
	m_fTargetPos.x = m_fPos.x + BOOM_DIST * m_iDir;
	m_fTargetPos.y = 600.f;

	m_fMu = 0.f;

	m_bCalcPos = false;


	CreateAnimation("Boom", AT_LINE, AO_LOOP, 608, 19, 32, 1, 1.f);
	m_pAnimation->AddLineFrameCount(32);
	m_pAnimation->ChangeAnimation(0);
	return true;
}

void CBoom::Input(float fTime)
{
}

void CBoom::Update(float fTime)
{
	CItem::Update(fTime);

	if (m_bCalcPos == false)
		CalcPos();		// 초기 좌표 셋팅
	else
		MoveCurved(fTime);	// 베지어 곡선 이동

							// 여기서 충돌체크해서 충돌하면 터트리든 뭘하든~솰라솰라

	if (m_fPos.y >= m_fTargetPos.y)
	{
		Die();
	}
}

void CBoom::Render(HDC hDC, float fTime)
{
	CItem::Render(hDC, fTime);
}

bool CBoom::IsVaildTarget()
{
	if (m_fTargetPos.x == 0.f && m_fTargetPos.y == 0.f)
		return false;

	return true;
}

void CBoom::CalcPos()
{
	if (!IsVaildTarget())
		return;	// 타겟 정보 유효하지 않음

				// 초기 위치 셋팅
	m_fOrgPos.x = m_fPos.x;
	m_fOrgPos.y = m_fPos.y;

	// 중점 좌표 셋팅
	m_fMidPos.x = (m_fPos.x + m_fTargetPos.x) / 2;
	m_fMidPos.y = m_fPos.y - DEFAULT_MID_HEIGHT;

	m_bCalcPos = true;
}

void CBoom::MoveCurved(float fTime)
{
	// https://www.google.co.kr/imgres?imgurl=https://upload.wikimedia.org/wikipedia/commons/thumb/6/6b/B%25C3%25A9zier_2_big.svg/1200px-B%25C3%25A9zier_2_big.svg.png&imgrefurl=https://ko.wikipedia.org/wiki/%25EB%25B2%25A0%25EC%25A7%2580%25EC%2597%2590_%25EA%25B3%25A1%25EC%2584%25A0&h=500&w=1200&tbnid=RXu9Rt5a3-D4hM:&q=%EB%B2%A0%EC%A7%80%EC%96%B4+%EA%B3%A1%EC%84%A0&tbnh=127&tbnw=304&usg=AI4_-kSv75BavyxRgeB-RKjvDy1msM4ANw&vet=12ahUKEwiwi4WZy8feAhUZ7bwKHXHQAhsQ9QEwAHoECAkQBg..i&docid=b8aK2M11VPh9kM&sa=X&ved=2ahUKEwiwi4WZy8feAhUZ7bwKHXHQAhsQ9QEwAHoECAkQBg

	m_fMu += 2.f * fTime;

	float num1, num12, mu2;

	mu2 = m_fMu * m_fMu;
	num1 = 1 - m_fMu;
	num12 = num1 * num1;

	// 공식대로 이동
	m_fPos.x = m_fOrgPos.x * num12 + 2 * m_fMidPos.x * num1 * m_fMu + m_fTargetPos.x * mu2;
	m_fPos.y = m_fOrgPos.y * num12 + 2 * m_fMidPos.y * num1 * m_fMu + m_fTargetPos.y * mu2;
}
