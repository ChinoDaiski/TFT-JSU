#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CMouse : public CGameObject {
public:
	typedef struct _tagMouseDesc {
		_float2		fSize;			// ���콺 UI�� ����/���� ����
	}MOUSEDESC;

protected:
	explicit CMouse(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CMouse(const CMouse& rhs);
	virtual ~CMouse() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();

private:
	void Device_Input(void);	// ���콺�� ������ �߻��ϴ� �ϵ�� ���õ� �Լ�
	void Cal_MousePos(void);	// ���콺�� ��ǥ�� �Ź� �������ִ� �Լ�
	void Move_PickingObject(void);	// Ŭ���ؼ� ���� ������Ʈ�� �����̴� �Լ�
	void Check_Collision();		// ��ŷ�� ������Ʈ�� Ÿ���� �浹 üũ

private:
	// ��ŷ ���� �Լ�
	CGameObject*	Picking_ChampAndItem(void);	// è�Ǿ�� �������� ��ŷ�ϸ� �ش� ������Ʈ ������ ��ȯ�ϴ� �Լ�

protected:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;	// �������� ���
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	POINT		Get_MousePos(void) { return m_ptMouse; }

private:
	POINT			m_ptMouse;	// ��ŷ�� ���� ���콺�� �ǽð� ��ġ ����
	_bool			m_bPicking = false;		// Ư�� ������Ʈ�� ������ ��쿡�� true
	CGameObject*	m_pPickekdObject = nullptr;		// ���õ� ������Ʈ
	_vector			m_vPickingObjectOriginPos;		// ���õ� ������Ʈ�� ���� �ڸ� ��ġ
	CGameObject*	m_pCollisionTile = nullptr;		// ���õ� ������Ʈ�� �浹�� Ÿ�� ����

public:
	static CMouse* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END