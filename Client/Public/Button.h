#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END



/*
	��ư�� ������ true �ƴϸ� false, ���콺�� �÷����°Ͱ��� ����̾���.
	��ư��


*/

BEGIN(Client)

class CButton : public CGameObject {
public:
	enum BUTTON_TYPE {
		BT_LOCK_UNLCOK,		// ���, ��� ���� - ����â�� ��״� ��ư
		BT_MINIMAP,			// �̴ϸ� - ����� ��ġ�� ī�޶� �̵��ϴ� ��ư
		BT_CHAMPION,		// è�Ǿ� ���� ��ư - ���ϴ� è�Ǿ��� �����ϴ� ��ư
		BT_LEVELUP,			// ������ ��ư - �������� �ϴ� ��ư
		BT_REROLL,			// ���� ��ư - ����â�� ���ΰ�ħ�ϴ� ��ư

		BT_END
	};

protected:
	explicit CButton(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CButton(const CButton& rhs);
	virtual ~CButton() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CTexture*			m_pTextureCom = nullptr;
	CTexture*			m_pTextureComBorder = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

protected:
	_float4x4			m_ProjMatrix;

protected:
	virtual void Click(void);

private:
	void CheckClick(void);

protected:
	RECT				m_rtInfo;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	HRESULT SetUp_Components(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg = nullptr);

public:
	static CButton* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	BUTTON_TYPE		Get_Type(void) { return m_eType; }

protected:
	BUTTON_TYPE		m_eType;
};

END