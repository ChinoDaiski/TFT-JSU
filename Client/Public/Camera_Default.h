#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Default final : public CCamera
{
private:
	explicit CCamera_Default(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CCamera_Default(const CCamera_Default& rhs);
	virtual ~CCamera_Default() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta);
	virtual void LateTick(_double TimeDelta);
	virtual HRESULT Render();

public:
	void SetMove(
		_fvector _StartEye, _fvector _StartAt, _fvector _StartUp,
		GXMVECTOR _EndEye, HXMVECTOR _EndAt, HXMVECTOR _EndUp,
		double duration
	);
	void SetPos(_fvector _Eye, _fvector _At, _fvector _Up);

	void ShowCameraPos(void);
	void Move(double deltaTime);

	void PressNumpad(void);

public:
	static CCamera_Default* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

private:
	bool		m_bMove = false;
	double		m_dCurTime = 0.0;
	double		m_dEndTime = 0.0;

	_float3		m_Start_Eye;
	_float3		m_Start_At;
	_float3		m_Start_Up;

	_float3		m_End_Eye;
	_float3		m_End_At;
	_float3		m_End_Up;

	_float4x4	m_StartMatrix;
	_float4x4	m_EndMatrix;
};

END