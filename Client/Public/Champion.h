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
	챔피언은 기본적인 AI가 존재, 이를 위해 현재 상태를 나타내는 변수와 관련된 FSM이 있음.
	상태는 기본, 이동, 공격, 스킬사용, 춤 4가지
	처음엔 기본상태이다.
	
	기본 - 말 그대로 기본상태, 가만히 서있는 애니메이션이 나온다. 스테이지 시작 전의 상태
	이동상태 - 만약 사거리 내에 챔피언이 있을 경우 해당 챔피언의 정보를 가지고, 공격상태로 변형
	공격상태 - 만약 마나가 전부 차오르면 스킬사용, 아니라면, 가지고 있는 챔피언과의 거리가 유효할떄, 평타를 사용. 만약 거리가 유효하지 않다면 이동상태로 변형
	스킬사용 - 챔피언별 정의된 스킬을 사용함. 사용후 이동상태로 변형.
	춤 - 스테이지가 끝났을 시 다음 스테이지로 넘어가기 전까지 춤 상태로 변형

	이동 상태일 경우 AStar 알고리즘으로 가장 거리가 짧은 챔피언을 향한 길을 찾아 타일 이동.
*/




class CChampion final : public CGameObject {
public:
	// 챔피언 정보 : 이름, 공격력, 주문력, 체력, 공격속도, 추가 마나, 치명타 확률, 회피 확률, 방어력, 마법 저항력, 이동 속도
	// 코스트 정보, 스킬 정보, 성장 레벨에 따른 수치 추가

	typedef struct Champion_Desc {
		ChampionName ChampName;			// 챔피언의 이름

		_tchar* pPrototype_ObjectName = nullptr;	// 프로토타입 오브젝트 태그
		_float4x4	TransformMatrix;	// 이동 행렬
		
		_uint iMaxHP;					// 최대 체력
		_uint iCurHP;					// 현재 체력
		_uint iAdd_Item_HP;				// 아이템으로 인한 추가 체력
		_uint iAdd_Synergy_HP;			// 시너지로 인한 추가 체력
		_uint iAdd_Characteristic_HP;	// 특성으로 인한 추가 체력

		_uint iMaxMP;					// 최대 마나
		_uint iCurMP;					// 현재 마나
		_uint iAdd_Item_MP;				// 아이템으로 인한 추가 마나
		_uint iAdd_Synergy_MP;			// 시너지로 인한 추가 마나
		_uint iAddCharacteristic_MP;	// 특성으로 인한 추가 마나

		WORD iLevel;					// 1,2,3 성

		_uint iAttackPower;						// 공격력
		_uint iAdd_Item_AttackPower;			// 아이템으로 인한 추가 공격력
		_uint iAdd_Synergy_AttackPower;			// 시너지로 인한 추가 공격력
		_uint iAdd_Characteristic_AttackPower;	// 특성으로 인한 추가 마나

		_uint iAbilityPower;					// 주문력
		_uint iAdd_Item_AbilityPower;			// 아이템으로 인한 추가 주문력
		_uint iAdd_Synergy_AbilityPower;		// 시너지로 인한 추가 주문력
		_uint iAdd_Characteristic_AbilityPower;	// 특성으로 인한 추가 마나

		_uint iDefensive;						// 방어력
		_uint iAdd_Item_Defensive;				// 아이템으로 인한 추가 방어력
		_uint iAdd_Synergy_Defensive;			// 시너지로 인한 추가 방어력
		_uint iAdd_Characteristic_Defensive;	// 특성으로 인한 추가 마나

		_uint iMagicResistance;						// 마법 저항력
		_uint iAdd_Item_MagicResistance;			// 아이템으로 인한 추가 마법 저항력
		_uint iAdd_Synergy_MagicResistance;			// 시너지로 인한 추가 마법 저항력
		_uint iAdd_Characteristic_MagicResistance;	// 특성으로 인한 추가 마나

		_uint iCritical;					// 치명타 확률
		_uint iAdd_Item_Critical;			// 아이템으로 인한 추가 치명타 확률
		_uint iAdd_Synergy_Critical;		// 시너지로 인한 추가 치명타 확률
		_uint iAdd_Characteristic_Critical;	// 특성으로 인한 추가 마나

		_uint iAvoidance;						// 회피 확률
		_uint iAdd_Item_Avoidance;				// 아이템으로 인한 추가 회피 확률
		_uint iAdd_Synergy_Avoidance;			// 시너지로 인한 추가 회피 확률
		_uint iAdd_Characteristic_Avoidance;	// 특성으로 인한 추가 마나

		_double iBaseAttackSpeed;					// 기본 공격 속도
		_double iAdd_Item_AttackSpeed;				// 아이템으로 인한 추가 공격 속도
		_double iAdd_Synergy_AttackSpeed;			// 시너지로 인한 추가 공격 속도
		_double iAdd_Characteristic_AttackSpeed;	// 특성으로 인한 추가 공격 속도
		_double dFinalAttackSpeed;					// 최종 공격 속도

		_uint iMoveSpeed;						// 이동 속도
		_uint iAdd_Item_MoveSpeed;				// 아이템으로 인한 추가 이동 속도
		_uint iAdd_Synergy_MoveSpeed;			// 시너지로 인한 추가 이동 속도
		_uint iAdd_Characteristic_MoveSpeed;	// 특성으로 인한 추가 마나

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
	virtual void Attack();	// 기본 공격
	virtual void Skill();	// 스킬 공격

private:
	void Champion_FSM(void);			// 챔피언 자동 상태 변경 함수
	CChampion* Check_InRange(void);		// 사거리 내에 적이 있는지 확인하는 함수, 없다면 nullptr 반환

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CCollider*			m_pAABBCom = nullptr;	// 챔피언의 피킹은 AABB형식으로 한다.

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
	// 장착할 아이템의 정보를 받아 저장하는 함수, 장착이 성공적이었다면 true, 아니라면 false를 반환함.
	bool Equip(CItem* pItem);

	// 아이템을 장착할 수 있는지 확인하는 함수, 장착이 가능하다면 true, 아니라면 false 반환
	bool Check_Equip(CItem* pItem);

protected:
	CHAMPDESC			m_ChampionDesc;		// 챔피언 정보

	vector<SYNERGY>		m_vSynergy;			// 챔피언의 시너지
	vector<CItem*>		m_Equipment;		// 장착한 아이템의 정보

	CHAMPION_STATE		m_eState;			// 챔피언의 현재 상태, 상태에 따라 다른 행동을 한다.
											// 기본, 이동, 공격(평타), 공격(스킬), 춤 5가지 행동이 존재한다.
	
	list<INCREASEDESC>	m_lstIncreaseDesc;	// 챔피언 스탯 별 증가 리스트

private:
	CChampion*			m_pTargetChamp;		// 공격 대상인 챔피언 정보
	
public:
	void Set_Show(_bool bShow) { m_bShow = bShow; }
	_bool Get_Show(void) { return m_bShow; }

private:
	_bool				m_bShow = false;
	
public:
	void IncreaseStatus(CHAMPION_STATUS eStatus, int iStatus, double duration);		// 증가하는 스탯정보, 증가 수치, 증가 지속시간

	void IncreaseAttackSpeed(int iAttackSpeed, double duration);	// 공격속도 증가 함수, 인자로 받은 속도와 시간만큼 빨라진 공격속도가 유지된다.
	void Check_IncreaseAttackSpeed(double duration);

private:
	// 징크스
	bool m_bIncreaseAttackSpeed;				// 공격 속도 증가 여부, true면 증가한다.
	double m_dwTimeStart_IncreaseAttackSpeed;	// 공격 속도 증가 시 얼마나 지속하는지, 해당 값은 증가된 공격 속도가 유지된 시간이다.
	double m_dwTimeEnd_IncreaseAttackSpeed;		// 공격 속도 증가 시 얼마나 지속하는지, 해당 값만큼 증가된 공격 속도가 유지된다.
};

END


