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
	스킬 - 챔피언마다 스킬이 1개씩 존재, 챔피언은 마나가 가득차면 스킬을 사용할 수 있음.
	스킬엔 챔피언의 성별 기본 공격력, 설명 UI가 존재, 챔피언의 주문력에 따라 다른 데미지를 입힘.
*/

class CSkill final : public CGameObject
{
public:
	enum ITEM_TYPE { FINISHED, UNFINISHED };

	// 아이템 정보 : 이름, 공격력, 주문력, 체력, 공격속도, 추가 마나, 치명타 확률, 회피 확률, 방어력, 마법 저항력, 이동 속도
	typedef struct Item_Desc {
		string ItemName;

		_uint iAttackPower;		// 공격력
		_uint iAbilityPower;	// 주문력

		_uint iHP;				// 추가 체력
		_uint iMP;				// 추가 마나

		_uint iDefensive;		// 방어력
		_uint iMagicResistance;	// 마법 저항력

		_uint iCritical;		// 치명타 확률
		_uint iAvoidance;		// 회피 확률

		_float	iAttackSpeed;	// 추가 공격 속도
		_uint	iMoveSpeed;		// 이동 속도

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