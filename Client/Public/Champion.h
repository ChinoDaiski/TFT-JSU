#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Item.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CModel;
class CCollider;
END

BEGIN(Client)

/*
	è�Ǿ��� �⺻���� AI�� ����, �̸� ���� ���� ���¸� ��Ÿ���� ������ ���õ� FSM�� ����.
	���´� �⺻, �̵�, ����, ��ų���, �� 4����
	ó���� �⺻�����̴�.
	
	�⺻ - �� �״�� �⺻����, ������ ���ִ� �ִϸ��̼��� ���´�. �������� ���� ���� ����
	�̵����� - ���� ��Ÿ� ���� è�Ǿ��� ���� ��� �ش� è�Ǿ��� ������ ������, ���ݻ��·� ����
	���ݻ��� - ���� ������ ���� �������� ��ų���, �ƴ϶��, ������ �ִ� è�Ǿ���� �Ÿ��� ��ȿ�ҋ�, ��Ÿ�� ���. ���� �Ÿ��� ��ȿ���� �ʴٸ� �̵����·� ����
	��ų��� - è�Ǿ� ���ǵ� ��ų�� �����. ����� �̵����·� ����.
	�� - ���������� ������ �� ���� ���������� �Ѿ�� ������ �� ���·� ����

	�̵� ������ ��� AStar �˰������� ���� �Ÿ��� ª�� è�Ǿ��� ���� ���� ã�� Ÿ�� �̵�.
*/




class CChampion final : public CGameObject {
public:
	// è�Ǿ� ���� : �̸�, ���ݷ�, �ֹ���, ü��, ���ݼӵ�, �߰� ����, ġ��Ÿ Ȯ��, ȸ�� Ȯ��, ����, ���� ���׷�, �̵� �ӵ�
	// �ڽ�Ʈ ����, ��ų ����, ���� ������ ���� ��ġ �߰�

	typedef struct Champion_Desc {
		ChampionName ChampName;			// è�Ǿ��� �̸�

		_tchar* pPrototype_ObjectName = nullptr;	// ������Ÿ�� ������Ʈ �±�
		_float4x4	TransformMatrix;	// �̵� ���
		
		_uint iMaxHP;					// �ִ� ü��
		_uint iCurHP;					// ���� ü��
		_uint iAdd_Item_HP;				// ���������� ���� �߰� ü��
		_uint iAdd_Synergy_HP;			// �ó����� ���� �߰� ü��
		_uint iAdd_Characteristic_HP;	// Ư������ ���� �߰� ü��

		_uint iMaxMP;					// �ִ� ����
		_uint iCurMP;					// ���� ����
		_uint iAdd_Item_MP;				// ���������� ���� �߰� ����
		_uint iAdd_Synergy_MP;			// �ó����� ���� �߰� ����
		_uint iAddCharacteristic_MP;	// Ư������ ���� �߰� ����

		WORD iLevel;					// 1,2,3 ��

		_uint iAttackPower;						// ���ݷ�
		_uint iAdd_Item_AttackPower;			// ���������� ���� �߰� ���ݷ�
		_uint iAdd_Synergy_AttackPower;			// �ó����� ���� �߰� ���ݷ�
		_uint iAdd_Characteristic_AttackPower;	// Ư������ ���� �߰� ����

		_uint iAbilityPower;					// �ֹ���
		_uint iAdd_Item_AbilityPower;			// ���������� ���� �߰� �ֹ���
		_uint iAdd_Synergy_AbilityPower;		// �ó����� ���� �߰� �ֹ���
		_uint iAdd_Characteristic_AbilityPower;	// Ư������ ���� �߰� ����

		_uint iDefensive;						// ����
		_uint iAdd_Item_Defensive;				// ���������� ���� �߰� ����
		_uint iAdd_Synergy_Defensive;			// �ó����� ���� �߰� ����
		_uint iAdd_Characteristic_Defensive;	// Ư������ ���� �߰� ����

		_uint iMagicResistance;						// ���� ���׷�
		_uint iAdd_Item_MagicResistance;			// ���������� ���� �߰� ���� ���׷�
		_uint iAdd_Synergy_MagicResistance;			// �ó����� ���� �߰� ���� ���׷�
		_uint iAdd_Characteristic_MagicResistance;	// Ư������ ���� �߰� ����

		_uint iCritical;					// ġ��Ÿ Ȯ��
		_uint iAdd_Item_Critical;			// ���������� ���� �߰� ġ��Ÿ Ȯ��
		_uint iAdd_Synergy_Critical;		// �ó����� ���� �߰� ġ��Ÿ Ȯ��
		_uint iAdd_Characteristic_Critical;	// Ư������ ���� �߰� ����

		_uint iAvoidance;						// ȸ�� Ȯ��
		_uint iAdd_Item_Avoidance;				// ���������� ���� �߰� ȸ�� Ȯ��
		_uint iAdd_Synergy_Avoidance;			// �ó����� ���� �߰� ȸ�� Ȯ��
		_uint iAdd_Characteristic_Avoidance;	// Ư������ ���� �߰� ����

		_double iBaseAttackSpeed;					// �⺻ ���� �ӵ�
		_double iAdd_Item_AttackSpeed;				// ���������� ���� �߰� ���� �ӵ�
		_double iAdd_Synergy_AttackSpeed;			// �ó����� ���� �߰� ���� �ӵ�
		_double iAdd_Characteristic_AttackSpeed;	// Ư������ ���� �߰� ���� �ӵ�
		_double dFinalAttackSpeed;					// ���� ���� �ӵ�

		_uint iMoveSpeed;						// �̵� �ӵ�
		_uint iAdd_Item_MoveSpeed;				// ���������� ���� �߰� �̵� �ӵ�
		_uint iAdd_Synergy_MoveSpeed;			// �ó����� ���� �߰� �̵� �ӵ�
		_uint iAdd_Characteristic_MoveSpeed;	// Ư������ ���� �߰� ����

	}CHAMPDESC, *PCHAMPDESC;

	enum CHAMPION_STATUS { CI_HP, CI_MP, CI_AD, CI_AP, CI_DF, CI_MR, CI_CRI, CI_AVOID, CI_AS, CI_MS, CI_END };
	enum CHAMPION_STATE { CS_ATTACK1, CS_ATTACK2, CS_CELEBRATION, CS_CRIT, CS_DEATH, CS_IDLE, CS_RUN, CS_SPELL1, CS_SPELL2, CS_SPELL3, CS_SPELL4, CS_END };
	enum CHAMPION_LEVEL { CLEVEL_1, CLEVEL_2, CLEVEL_3, CLEVEL_4, CLEVEL_5, CLEVEL_END };

	typedef struct _tagIncreaseDesc {
		double m_dwTimeStart;
		double m_dwTimeEnd;

		CHAMPION_STATUS m_eStatus;
	}INCREASEDESC;

private:
	explicit CChampion(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CChampion(const CChampion& rhs);
	virtual ~CChampion() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Attack();	// �⺻ ����
	virtual void Skill();	// ��ų ����

private:
	void Champion_FSM(void);			// è�Ǿ� �ڵ� ���� ���� �Լ�
	CChampion* Check_InRange(void);		// ��Ÿ� ���� ���� �ִ��� Ȯ���ϴ� �Լ�, ���ٸ� nullptr ��ȯ

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pAABBCom = nullptr;	// è�Ǿ��� ��ŷ�� AABB�������� �Ѵ�.

public:
	HRESULT SetUp_Components(void * pArg);
	HRESULT SetUp_ConstantTable();

public:
	static CChampion* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	const CHAMPDESC& Get_ChampionDesc(void) const { return m_ChampionDesc; }
	const vector<SYNERGY>& Get_Synergy(void) const { return m_vSynergy; }

public:
	// ������ �������� ������ �޾� �����ϴ� �Լ�, ������ �������̾��ٸ� true, �ƴ϶�� false�� ��ȯ��.
	bool Equip(CItem* pItem);

	// �������� ������ �� �ִ��� Ȯ���ϴ� �Լ�, ������ �����ϴٸ� true, �ƴ϶�� false ��ȯ
	bool Check_Equip(CItem* pItem);

protected:
	CHAMPDESC			m_ChampionDesc;		// è�Ǿ� ����

	vector<SYNERGY>		m_vSynergy;			// è�Ǿ��� �ó���
	vector<CItem*>		m_Equipment;		// ������ �������� ����

	CHAMPION_STATE		m_eState;			// è�Ǿ��� ���� ����, ���¿� ���� �ٸ� �ൿ�� �Ѵ�.
											// �⺻, �̵�, ����(��Ÿ), ����(��ų), �� 5���� �ൿ�� �����Ѵ�.
	
	list<INCREASEDESC>	m_lstIncreaseDesc;	// è�Ǿ� ���� �� ���� ����Ʈ

private:
	CChampion*			m_pTargetChamp;		// ���� ����� è�Ǿ� ����
	
public:
	void Set_Show(_bool bShow) { m_bShow = bShow; }
	_bool Get_Show(void) { return m_bShow; }

private:
	_bool				m_bShow = false;
	
public:
	void IncreaseStatus(CHAMPION_STATUS eStatus, int iStatus, double duration);		// �����ϴ� ��������, ���� ��ġ, ���� ���ӽð�

	void IncreaseAttackSpeed(int iAttackSpeed, double duration);	// ���ݼӵ� ���� �Լ�, ���ڷ� ���� �ӵ��� �ð���ŭ ������ ���ݼӵ��� �����ȴ�.
	void Check_IncreaseAttackSpeed(double duration);

private:
	// ¡ũ��
	bool m_bIncreaseAttackSpeed;				// ���� �ӵ� ���� ����, true�� �����Ѵ�.
	double m_dwTimeStart_IncreaseAttackSpeed;	// ���� �ӵ� ���� �� �󸶳� �����ϴ���, �ش� ���� ������ ���� �ӵ��� ������ �ð��̴�.
	double m_dwTimeEnd_IncreaseAttackSpeed;		// ���� �ӵ� ���� �� �󸶳� �����ϴ���, �ش� ����ŭ ������ ���� �ӵ��� �����ȴ�.
};

END


