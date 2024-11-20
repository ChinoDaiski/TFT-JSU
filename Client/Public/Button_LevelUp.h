#pragma once

#include "Client_Defines.h"
#include "Button.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CButton_LevelUp final : public CButton {
public:
	typedef struct _tagBtMiniMapDesc {
		RECT			rtButtonInfo;	// ��ư�� �»�� ~ ���ϴ� �簢�� ��ǥ

		BUTTON_TYPE		eType;			// ��ư�� ����, � ��ư���� �˼� ����

		_float3			vEye;			// �̴ϸʰ� ������ ī�޶��� ���� 
		_float3			vAt;
		_float3			vUp;
	} BtMiniMapDesc;

private:
	explicit CButton_LevelUp(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CButton_LevelUp(const CButton_LevelUp& rhs);
	virtual ~CButton_LevelUp() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

private:
	_float3				m_vAt, m_vEye, m_vUp;

public:
	HRESULT SetUp_Components();
	virtual void Click(void) override;

public:
	static CButton_LevelUp* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END