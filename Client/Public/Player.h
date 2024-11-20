#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Champion.h"
#include "Item.h"
#include "Tile.h"
#include "PersonalMap.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

/*
	플레이어의 행동 
	01. 챔피언을 상점으로부터 구매할 수 있다. 구매한 챔피언은 플레이어의 필드에 놓이게 된다. 만약 필드에 자리가 없다면 상점으로부터 챔피언을 구매할 수 없다.
		 - 챔피언 구매

	02. 자신의 필드 위에 있는 챔피언을 옭길 수 있다. 챔피언을 선택 후 필드안에서 다른 챔피언과 충돌처리하면 서로 자리가 바뀐다.
	    필드위에 챔피언이 없는 공간에 옮기면 그 자리에 배치된다. 만약 해당 챔피언을 특정 공간에 놔두면 판매된다.
		 - 챔피언 선택, 내려놓기

	03. 아이템을 장착할 수 있다. 플레이어는 자신이 가지고 있는 아이템을 그래그 하여 챔피언과 충돌시켜 해당 챔피언에게 아이템을 줄 수 있다. 만약, 1개의 아이템을 가지고 있는
	    챔피언에게 준다면 아이템은 합성되어 완제품이된다. 아이템을 최대 갯수로 가지고 있는 챔피언에게 준다면, 아이템은 기존에 있던 자리로 돌아온다.
		 - 아이템 선택, 내려놓기

	04. UI와 상호작용할 수 있다. 플레이어에게는 상점 창과 리롤, 레벨업 버튼이 있는 UI가 존재한다. 해당 UI와 상호작용하여 상점의 갱신, 레벨업, 챔피언의 구매가 가능하다.
	     - 레벨업, 리롤, 구매
	
	피킹 및 상호작용
	01. UI
	02. 아이템
	03. 챔피언
*/


class CPlayer final : public CGameObject {
public:
	// 캐릭터의 상태 정보
	enum PLAYER_STATE { PS_IDLE, PS_CELEBRATION, PS_DANCE, PS_DEATH, PS_RUN_NORMAL, PS_RUN_FAST, PS_END };
	
	// 캐릭터 초기 할당 정보
	typedef struct _tagPlayerDesc {
		CharacterName CharacterName;				// 캐릭터의 이름
		_tchar* pPrototype_CharacterName = nullptr;	// 프로토타입 오브젝트 태그
		_float3	vPos;
	}PLAYERDESC;

private:
	explicit CPlayer(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pAABBcom = nullptr;	// Player 충돌은 챔피언, 그것도 초밥에 있는 챔피언과 AABB형식으로 한다.

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	HRESULT SetUp_Components(void * pArg);
	HRESULT SetUp_ConstantTable(void);

public:
	static CPlayer* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	void Level_Up();							// 레벨업
	bool Purchase(CChampion* pChamp);			// 챔피언 구매
	void Select_Object(CGameObject* pObject);	// 오브젝트 선택(챔피언, 아이템)

public:
	_uint Get_Level(void) const { return m_iLevel; }
	_uint Get_Gold(void) const { return m_iGold; }

private:
	void Player_FSM(_double TimeDelta);
	void PressKey(void);
	void Set_TargetPos(void);

	// 원하는 위치로 움직이는 함수
	void MoveTo(_vector vPos);

public:
	void MoveMap(CPersonalMap::MAP_NAME eMapName) { m_eCurrMapName = eMapName; }
	CPersonalMap::MAP_NAME Get_CurrMapName(void) { return m_eCurrMapName; }

private:
	_float4				m_TargetPos;
	_bool				m_bMove = false;
	PLAYER_STATE		m_eState = PS_IDLE;

public:
	const vector<CChampion*>&	Get_Champion_Field(void) { return m_Champions_Field; }
	const vector<CChampion*>&	Get_Champion_Lounge(void) { return m_Champions_Lounge; }

private:
	_uint				m_iLevel = 2;			// 플레이어의 레벨, 레벨에 따라 상점에서 나오는 챔피언의 종류와 필드에 놓을 수 있는 챔피언의 갯수가 달라진다.
	_uint				m_iGold = 0;			// 플레이어가 소지하고 있는 골드

	vector<CItem*>		m_Items;				// 현재 플레이어가 가지고 있는 아이템 정보
	vector<CChampion*>	m_Champions_Lounge;		// 현재 플레이어가 가지고 있는 챔피언의 정보
	vector<CChampion*>	m_Champions_Field;		// 현재 플레이어가 가지고 있는 챔피언의 정보

	CGameObject*		m_pSelectObject = nullptr;	// 플레이어가 선택한 오브젝트, 챔피언 / 아이템 중 하나를 선택할 수 있다.
													// 선택된 오브젝트의 종류에 따라 다른 상호작용을 한다.
													// 챔피언 선택시 판매창이 나오며, 아이템 선택시 챔피언에 아이템을 넣을 수 있다.

	CharacterName		m_eName = CharacterName::Character_Pet_END;

	CPersonalMap::MAP_NAME	m_eCurrMapName = CPersonalMap::MAP_NAME_END;

public:
	void	PushTile(CTile::TILE_TYPE eType, CTile* pTile, _uint iMapName);

public:
	const vector<CTile*>&	Get_Tile_Field(void) { return m_pTile_Field[m_eCurrMapName]; }
	const vector<CTile*>&	Get_Tile_Lounge(void) { return m_pTile_Lounge[m_eCurrMapName]; }

private:
	vector<CTile*>		m_pTile_Field[CPersonalMap::MAP_NAME::MAP_NAME_END];
	vector<CTile*>		m_pTile_Lounge[CPersonalMap::MAP_NAME::MAP_NAME_END]; 
};

END