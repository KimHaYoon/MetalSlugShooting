#pragma once
#include "CItem.h"

class CBoom : public CItem
{
public:
	CBoom();
	CBoom(const string& strTag);
	virtual ~CBoom();

private:
	int			m_iDir;
	float2		m_fTargetPos;	// Ÿ����ǥ x,y
	float2		m_fMidPos;	// Ÿ�ٰ��� �߰� -y�� ��ǥ
	float2		m_fOrgPos;	// �������
	float		m_fMu;

	bool		m_bCalcPos;

public:
	virtual bool Init(float2 fPos, int dir = 0);
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);

public:
	bool	IsVaildTarget();

	void	CalcPos();
	void	MoveCurved(float fTime);
};

