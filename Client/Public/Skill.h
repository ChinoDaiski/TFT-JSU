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

/*
	��ų - è�Ǿ𸶴� ��ų�� 1���� ����, è�Ǿ��� ������ �������� ��ų�� ����� �� ����.
	��ų�� è�Ǿ��� ���� �⺻ ���ݷ�, ���� UI�� ����, è�Ǿ��� �ֹ��¿� ���� �ٸ� �������� ����.
*/

class CSkill final : public CGameObject
{
public:
	enum ITEM_TYPE { FINISHED, UNFINISHED };

	// ������ ���� : �̸�, ���ݷ�, �ֹ���, ü��, ���ݼӵ�, �߰� ����, ġ��Ÿ Ȯ��, ȸ�� Ȯ��, ����, ���� ���׷�, �̵� �ӵ�
	typedef struct Item_Desc {
		string ItemName;

		_uint iAttackPower;		// ���ݷ�
		_uint iAbilityPower;	// �ֹ���

		_uint iHP;				// �߰� ü��
		_uint iMP;				// �߰� ����

		_uint iDefensive;		// ����
		_uint iMagicResistance;	// ���� ���׷�

		_uint iCritical;		// ġ��Ÿ Ȯ��
		_uint iAvoidance;		// ȸ�� Ȯ��

		_float	iAttackSpeed;	// �߰� ���� �ӵ�
		_uint	iMoveSpeed;		// �̵� �ӵ�

	} ITEMDESC, *PITEMDESC;

private:
	explicit CSkill(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CSkill(const CSkill& rhs);
	virtual ~CSkill() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	HRESULT SetUp_Components();

public:
	static CSkill* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


private:
	ITEM_TYPE			m_eType;
	ITEMDESC			m_ItemDesc;

public:
	const ITEMDESC&		Get_ItemDesc(void) const { return m_ItemDesc; }

};

END