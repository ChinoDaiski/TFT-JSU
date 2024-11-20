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

/*
	필드는 챔피언이 배치 될 때마다 시너지의 활성 여부를 결정한다.
	또한 배치를 제외하면 해당 챔피언이 가지고 있는 시너지에 대해서만 검사를 하여 적용 여부를 결정한다.
	
	활성된 시너지는 필드에 있는 챔피언에게 적용시켜준다. 챔피언은 활성된 시너지에 대한 정보를 가지고 있다.
	
	또한 배치시 같은 종류의 같은 성의 챔피언이 3마리가 존재하면, 3마리가 합쳐서 다름 성의 챔피언으로 변신한다.
	변신된 챔피언은 능력치가 올라가며, 크기가 커지고 모습이 변하며, 스킬의 계수 또한 올라간다. 


	아이템은 크립을 잡으면 나오고, 필드에 놓여짐. 해당 아이템들은 플레이어가 피킹으로 배치 가능.
	배치 실패시 원래 놓여져 있던 자리로 돌아감.
*/

class CPersonalMap final : public CGameObject
{
public:
	enum MAP_NAME {
		MAP_NAME_Celebrationtomorrow,
		MAP_NAME_Hexbattle,
		MAP_NAME_ZaunCity,
		MAP_NAME_Jinx,
		MAP_NAME_Vi,
		MAP_NAME_Spiritblossom_kami,
		MAP_NAME_Spiritblossom_reality,
		MAP_NAME_Spiritblossom_yokai,
		MAP_NAME_END
	};

	typedef struct _tagPersonalMapDesc {
		MAP_NAME	eType;				// 맵의 타입
		_float4x4	TransformMatrix;	// 이동 행렬
	}PersonalMapDesc;

private:
	explicit CPersonalMap(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CPersonalMap(const CPersonalMap& rhs);
	virtual ~CPersonalMap() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CModel*				m_pModelCom = nullptr;

public:
	HRESULT SetUp_Components(void * pArg);
	HRESULT SetUp_ConstantTable();

public:
	static CPersonalMap* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	

public:
	MAP_NAME Get_Type(void) { return m_eMapName; }


public:
	void checkSynergy(void);	// 활성화된 시너지를 확인, 챔피언이 새로 배칠될 때 마다 해당 함수를 불러 최종적으로 활성화된 시너지의 정보를 확인한다.
	void AssignChampion(CChampion* pChampion);		// 상점에서 챔피언을 샀을 경우 실행되며, 인자로 산 챔피언을 받아 해당 챔피언을 필드에 배치하는 함수

public:
	// 시너지 관련 함수
	void Check_Rival();			// 경쟁자
	void Check_Scrap();			// 고물상
	void Check_Glutton();		// 대식가
	void Check_Mutant();		// 돌연변이
	void Check_Hextech();		// 마법공학
	void Check_Syndicate();		// 범죄 조직
	void Check_Socialite();		// 사교계
	void Check_Clockwork();		// 시계태엽
	void Check_Debonair();		// 연미복
	void Check_Yordle();		// 요들
	void Check_Yordle_Lord();	// 요들 군주
	void Check_Mercenary();		// 용병
	void Check_Enforcer();		// 집행자
	void Check_Chemtech();		// 화학공학
	void Check_Mastermind();	// 흑막
	void Check_Enchanter();		// 강화술사
	void Check_Colossus();		// 거신
	void Check_Bodyguard();		// 경호대
	void Check_Bruiser();		// 난동꾼
	void Check_Challenger();	// 도전자
	void Check_Transformer();	// 변형술사
	void Check_Arcanist();		// 비전마법사
	void Check_Twinshot();		// 쌍발총
	void Check_Assassin();		// 암살자
	void Check_Sniper();		// 저격수
	void Check_Striker();		// 타격대
	void Check_Scholar();		// 학자
	void Check_Innovator();		// 혁신가

private:
	map<SYNERGY, int>	m_ChampionSynergy;		// 필드에 배치된 챔피언의 시너지 모음
	map<SYNERGY, int>	m_ActivatedSynergy;		// 활성화된 시너지의 모음

	list<CChampion*>	m_Champions;			// 현재 필드에 있는 챔피언의 정보

	MAP_NAME			m_eMapName;				// 현 필드의 이름. 로드된 모델의 이름이기도 함.
};

END