#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Champion.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CTile : public CGameObject {
public:
	enum TILE_TYPE {
		TILE_FIELD,
		TILE_LOUNGE,
		TILE_END
	};

	typedef struct _tagTileDesc {
		TILE_TYPE	eType;
		_float3		vPos;
		_float2		vScale;
	}TILEDesc;

protected:
	explicit CTile(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CTile(const CTile& rhs);
	virtual ~CTile() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CCollider*			m_pAABBCom = nullptr;	// 타일의 피킹은 AABB형식으로 한다.


	CTexture*			m_pEffectTextureCom = nullptr;

public:
	HRESULT SetUp_Components();
	HRESULT SetUp_ConstantTable();

public:
	static CTile* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	TILE_TYPE			Get_Type(void) { return m_eType; }
	CChampion*			Get_ChampionInfo(void)  { return m_pChamp; }
	void				Set_ChampionInfo(CChampion* pChamp);

private:
	TILE_TYPE		m_eType;
	CChampion*		m_pChamp = nullptr;

private:
	void Change_CurrEffectFrame(_double dFrame, _double DeltaTime);

public:
	void Set_Collision(_bool bColl) { m_isCollision = bColl; }

private:
	_bool			m_isCollision = false;
	_uint			m_iCurrEffectFrame = 0;
	_double			m_dwEffectTime = 0.0;
};

END