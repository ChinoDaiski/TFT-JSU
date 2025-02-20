#pragma once
#include "Base.h"
#include "PipeLine.h"

BEGIN(Engine)

class CMousePicking final : public CBase
{
public:
	typedef struct _tagRayDesc {
		_float3 vRay;
		_float3 vRayPos;
	}RAYDESC;

private:
	DECLARE_SINGLETON(CMousePicking)

private:
	CMousePicking();
	virtual ~CMousePicking() = default;

public:
	HRESULT InitPicking(HWND hWnd, _uint iWinCX, _uint iWinCY);
	void Make_Ray(void);
	_bool Picking(_float3* pVerticesPos, _fmatrix WorldMatrix, _vector* pOut);

public:
	const RAYDESC Get_vRayDesc(void);

private:
	_float3 m_vRay;
	_float3 m_vRayPos;

	HWND	m_hWnd;
	_uint	m_iWinCX;
	_uint	m_iWinCY;

private:
	POINT Get_Mouse(void);

public:
	virtual void Free() override;
};

END