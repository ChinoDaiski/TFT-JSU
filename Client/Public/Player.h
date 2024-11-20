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
	�÷��̾��� �ൿ 
	01. è�Ǿ��� �������κ��� ������ �� �ִ�. ������ è�Ǿ��� �÷��̾��� �ʵ忡 ���̰� �ȴ�. ���� �ʵ忡 �ڸ��� ���ٸ� �������κ��� è�Ǿ��� ������ �� ����.
		 - è�Ǿ� ����

	02. �ڽ��� �ʵ� ���� �ִ� è�Ǿ��� �ı� �� �ִ�. è�Ǿ��� ���� �� �ʵ�ȿ��� �ٸ� è�Ǿ�� �浹ó���ϸ� ���� �ڸ��� �ٲ��.
	    �ʵ����� è�Ǿ��� ���� ������ �ű�� �� �ڸ��� ��ġ�ȴ�. ���� �ش� è�Ǿ��� Ư�� ������ ���θ� �Ǹŵȴ�.
		 - è�Ǿ� ����, ��������

	03. �������� ������ �� �ִ�. �÷��̾�� �ڽ��� ������ �ִ� �������� �׷��� �Ͽ� è�Ǿ�� �浹���� �ش� è�Ǿ𿡰� �������� �� �� �ִ�. ����, 1���� �������� ������ �ִ�
	    è�Ǿ𿡰� �شٸ� �������� �ռ��Ǿ� ����ǰ�̵ȴ�. �������� �ִ� ������ ������ �ִ� è�Ǿ𿡰� �شٸ�, �������� ������ �ִ� �ڸ��� ���ƿ´�.
		 - ������ ����, ��������

	04. UI�� ��ȣ�ۿ��� �� �ִ�. �÷��̾�Դ� ���� â�� ����, ������ ��ư�� �ִ� UI�� �����Ѵ�. �ش� UI�� ��ȣ�ۿ��Ͽ� ������ ����, ������, è�Ǿ��� ���Ű� �����ϴ�.
	     - ������, ����, ����
	
	��ŷ �� ��ȣ�ۿ�
	01. UI
	02. ������
	03. è�Ǿ�
*/


class CPlayer final : public CGameObject {
public:
	// ĳ������ ���� ����
	enum PLAYER_STATE { PS_IDLE, PS_CELEBRATION, PS_DANCE, PS_DEATH, PS_RUN_NORMAL, PS_RUN_FAST, PS_END };
	
	// ĳ���� �ʱ� �Ҵ� ����
	typedef struct _tagPlayerDesc {
		CharacterName CharacterName;				// ĳ������ �̸�
		_tchar* pPrototype_CharacterName = nullptr;	// ������Ÿ�� ������Ʈ �±�
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
	CCollider*			m_pAABBcom = nullptr;	// Player �浹�� è�Ǿ�, �װ͵� �ʹ信 �ִ� è�Ǿ�� AABB�������� �Ѵ�.

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
	void Level_Up();							// ������
	bool Purchase(CChampion* pChamp);			// è�Ǿ� ����
	void Select_Object(CGameObject* pObject);	// ������Ʈ ����(è�Ǿ�, ������)

public:
	_uint Get_Level(void) const { return m_iLevel; }
	_uint Get_Gold(void) const { return m_iGold; }

private:
	void Player_FSM(_double TimeDelta);
	void PressKey(void);
	void Set_TargetPos(void);

	// ���ϴ� ��ġ�� �����̴� �Լ�
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
	_uint				m_iLevel = 2;			// �÷��̾��� ����, ������ ���� �������� ������ è�Ǿ��� ������ �ʵ忡 ���� �� �ִ� è�Ǿ��� ������ �޶�����.
	_uint				m_iGold = 0;			// �÷��̾ �����ϰ� �ִ� ���

	vector<CItem*>		m_Items;				// ���� �÷��̾ ������ �ִ� ������ ����
	vector<CChampion*>	m_Champions_Lounge;		// ���� �÷��̾ ������ �ִ� è�Ǿ��� ����
	vector<CChampion*>	m_Champions_Field;		// ���� �÷��̾ ������ �ִ� è�Ǿ��� ����

	CGameObject*		m_pSelectObject = nullptr;	// �÷��̾ ������ ������Ʈ, è�Ǿ� / ������ �� �ϳ��� ������ �� �ִ�.
													// ���õ� ������Ʈ�� ������ ���� �ٸ� ��ȣ�ۿ��� �Ѵ�.
													// è�Ǿ� ���ý� �Ǹ�â�� ������, ������ ���ý� è�Ǿ� �������� ���� �� �ִ�.

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