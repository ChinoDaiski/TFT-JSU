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
	아이템 - 플레이어는 아이템을 배치할 수 있고, 아이템은 챔피언에게 들어가 장착 무기 처럼 능력치를 더해줌.
	아이템 마다 능력이 다름. 아이템은 기본으로 조각 아이템, 완제품 아이템이 존재함.
	조각 아이템이 있는 챔피언에게 다른 조각 아이템을 넣으면 아이템이 합쳐져서 새로운 아이템이 됨.
	한번 합쳐진 아이템은 일반적인 방법으로는 되돌릴수 없음.
*/

class CItem final : public CGameObject
{
public:
	// 조각템인지, 완제품인지
	enum ITEM_TYPE { PIECE, FINISHED };

	// 아이템 정보 : 이름, 공격력, 주문력, 체력, 공격속도, 추가 마나, 치명타 확률, 회피 확률, 방어력, 마법 저항력, 이동 속도
	typedef struct Item_Desc {
		ITEM_TYPE eType;		// 아이템 타입
		ITEM_NAME eName;		// 아이템 이름

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
	explicit CItem(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CItem(const CItem& rhs);
	virtual ~CItem() = default;

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
	static CItem* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;



public:
	const ITEM_TYPE&	Get_ItemType(void) const { return m_eType; }
	const ITEMDESC&		Get_ItemDesc(void) const { return m_ItemDesc; }


private:
	ITEM_TYPE			m_eType;
	ITEMDESC			m_ItemDesc;

};

END