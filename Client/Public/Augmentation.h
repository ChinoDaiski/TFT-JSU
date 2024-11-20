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
	실버 증강체
	
	
	계산된 패배
	전투에서 패배한 후 2골드와 무료 상점 새로고침 1회를 획득합니다.
	
	
	고대의 기록 보관소
	특성의 고서 1개를 획득합니다.
	
	
	지배
	플레이어 전투에서 승리한 후 살아남은 유닛 2명당 추가 1골드를 획득합니다.
	
	
	아이템 꾸러미 I
	무작위 완성 아이템 1개를 획득합니다.
	
	
	허수아비 전선
	2개의 훈련 봇을 획득합니다.
	
	
	초고속 모드
	라운드 종료 시 가진 골드가 10보다 적다면 2골드를 획득합니다.
	
	
	판도라의 아이템
	무작위 조합 아이템을 획득합니다. 라운드 시작 시 대기석의 아이템이 무작위로 변합니다. (전략가의 왕관, 뒤집개, 소모품 제외)
	
	
	초월
	전투 15초 후 아군 유닛의 피해량이 50% 증가합니다.
	
	
	다른 태생 I
	활성화된 특성이 없는 아군 유닛이 300의 체력과 60%의 공격 속도를 얻습니다.
	
	
	천상의 축복 I
	아군 유닛이 공격과 스킬로 입힌 피해의 12%만큼 체력을 회복합니다. 초과된 회복량은 최대 300의 피해를 흡수하는 보호막으로 전환됩니다.
	
	
	나이프의 날 I
	전방 2열에서 전투를 시작하는 아군 유닛이 30의 공격력을 얻습니다.
	
	
	사이버네틱 이식술 I
	아이템을 가진 챔피언이 150의 체력과 10의 공격력을 얻습니다.
	
	
	추방자 I
	인접한 아군 없이 전투를 시작하는 아군은 8초 동안 최대 체력의 30%에 해당하는 보호막을 획득합니다.
	
	
	경량급 I
	비용이 1과 2인 챔피언이 25%의 공격 속도 및 이동 속도를 획득합니다.
	
	
	응급처치 키트
	아군 유닛이 받는 모든 회복 효과와 보호막 효과가 35% 증가합니다.
	
	
	임시변통 방어구 I
	아이템이 없는 아군 유닛이 35의 방어력과 마법 저항력을 얻습니다.
	
	
	단결된 의지 I
	아군 유닛이 전체적으로 활성화된 특성 하나당 2의 공격력과 주문력을 획득합니다.
	
	
	사냥의 전율 I
	아군 유닛이 적을 처치하면 400의 체력을 회복합니다.
	
	
	약자
	살아남은 아군 유닛이 상대보다 적다면 아군 유닛이 매초 체력을 9%만큼 회복합니다. (최대: 150)
	
	
	약점 I
	아군 유닛이 공격 시 5초 동안 대상 방어력의 20%를 무시하고 대상이 받는 회복 효과를 50% 감소시킵니다.
	
	
	비전 마법사 심장
	팀에 비전 마법사 1명이 추가로 포함된 것으로 간주합니다. 스웨인을 획득합니다.
	
	
	룬 보호막 I
	전투 시작 시 비전 마법사가 8초 동안 주문력의 300%에 해당하는 보호막을 얻습니다. 스웨인을 획득합니다.
	
	
	암살자 심장
	팀에 암살자 1명이 추가로 포함된 것으로 간주합니다. 탈론을 획득합니다.
	
	
	자객
	암살자들이 첫 공격 대상의 마나를 강탈하여 대상이 스킬을 사용하기 전까지 최대 마나가 50% 증가합니다. 탈론을 획득합니다.
	
	
	경호대 심장
	팀에 경호대 1명이 추가로 포함된 것으로 간주합니다. 블리츠크랭크를 획득합니다.
	
	
	내가 지킨다
	경호대가 얻는 추가 방어력이 25% 증가합니다. 전투 시작 시 경호대가 바로 뒤에 있는 경호대가 아닌 아군들(경호대 제외)에게 100%의 방어력을 부여합니다. (중첩 불가) 블리츠크랭크를 획득합니다.
	
	
	난동꾼 심장
	팀에 난동꾼 1명이 추가로 포함된 것으로 간주합니다. 세주아니를 획득합니다.
	
	
	강인함
	난동꾼이 매초 최대 체력의 2.5%를 회복합니다. 세주아니를 획득합니다.
	
	
	도전자 심장
	팀에 도전자 1명이 추가로 포함된 것으로 간주합니다. 퀸을 획득합니다.
	
	
	준비 태세
	적이 처음으로 도전자에게 공격당하면 2.5초 동안 무장 해제됩니다. 워윅을 획득합니다.
	
	
	화학적 과부하 I
	화학공학이 사망 시 폭발하여 2칸 내 적에게 최대 체력의 20%만큼 마법 피해를 입힙니다. 워윅을 획득합니다.
	
	
	화학공학 심장
	팀에 화학공학 1명이 추가로 포함된 것으로 간주합니다. 워윅을 획득합니다.
	
	
	시계태엽 심장
	팀에 시계태엽 1명이 추가로 포함된 것으로 간주합니다. 질리언을 획득합니다.
	
	
	강화술사 심장
	팀에 강화술사 1명이 추가로 포함된 것으로 간주합니다. 룰루를 획득합니다.
	
	
	불타는 향로
	강화술사에 의해 회복 및 보호막 효과를 얻은 아군은 전투가 끝날 때 까지 중첩 가능한 공격 속도를 45% 획득합니다. (최대 3초에 1회) 룰루를 획득합니다.
	
	
	집행자 심장
	팀에 집행자 1명이 추가로 포함된 것으로 간주합니다. 세주아니를 획득합니다.
	
	
	자가 복구
	혁신적 로봇이 처치되면 대상으로 지정할 수 없는 상태가 되며 혁신가가 아직 살아있으면 스스로를 복구합니다. 질리언을 획득합니다.
	
	
	용병 심장
	팀에 용병 1명이 추가로 포함된 것으로 간주합니다.
	
	
	해적
	용병이 적을 처치하면 66%의 확률로 1골드가 떨어집니다. 퀸을 획득합니다.
	
	
	돌연변이 심장
	팀에 돌연변이 1명이 추가로 포함된 것으로 간주합니다. 렉사이를 획득합니다.
	
	
	불안정한 진화
	돌연변이는 2성이 되면 500의 체력, 30%의 공격 속도, 30의 공격력, 30의 주문력 중 하나의 효과를 무작위로 얻습니다. 이 효과는 중첩됩니다. 렉사이를 획득합니다.
	
	
	학자 심장
	팀에 학자 1명이 추가로 포함된 것으로 간주합니다. 자이라를 획득합니다.
	
	
	평생 학습
	전투가 끝난 후 학자가 영구적으로 2의 주문력을 얻습니다. 전투에서 생존한 경우 3의 주문력을 추가로 얻습니다. 신드라를 획득합니다.
	
	
	고물상 심장
	팀에 고물상 1명이 추가로 포함된 것으로 간주합니다. 블리츠크랭크를 획득합니다.
	
	
	저격수 심장
	팀에 저격수 1명이 추가로 포함된 것으로 간주합니다. 애쉬를 획득합니다.
	
	
	이중주
	사교계 스포트라이트를 추가로 1개 소환합니다. 스포트라이트를 받은 유닛은 체력을 200 얻습니다. 세나를 획득합니다.
	
	
	사교계 심장
	팀에 사교계 1명이 추가로 포함된 것으로 간주합니다.
	
	
	범죄 조직 심장
	팀에 범죄 조직 1명이 추가로 포함된 것으로 간주합니다. 자이라를 획득합니다.
	
	
	물려주기
	범죄 조직이 사망하면 다른 범죄 조직에게 15의 공격력과 주문력을 부여합니다. 애쉬를 획득합니다.
	
	
	수금
	전투에서 승리하고 살아남은 범죄 조직 하나당 1의 추가 골드를 획득합니다. 애쉬를 획득합니다.
	
	
	쌍발총 심장
	팀에 쌍발총 1명이 추가로 포함된 것으로 간주합니다. 코르키를 획득합니다.
	
	
	보물창고 I
	파란색 전리품 구 1개와 회색 전리품 구 1개를 획득합니다.
	
	
	대격변 생성기
	전장에 있는 챔피언들이 비용이 1 높은 무작위 챔피언으로 영구히 바뀝니다. 자석 제거기 2개를 획득합니다.
	
	
	수호자 I
	전투 시작 시 모든 아군 유닛이 인접한 아군에게 8초 동안 160의 피해를 흡수하는 보호막을 부여합니다.
	
	
	고전압 I
	아군 유닛이 치명타를 입으면 주변 적에게 60~105(현재 스테이지에 비례)의 마법 피해를 입힙니다. (재사용 대기시간 1초)
	
	
	분해기 I
	아군 유닛의 기본 공격이 대상 최대 체력의 1.5%에 해당하는 추가 마법 피해를 입힙니다.
	
	
	루덴의 메아리 I
	아군 유닛이 스킬을 사용해 스킬 피해를 입히면 처음 적중한 대상과 그 주변 적 하나가 70~130(현재 스테이지에 비례)의 추가 마법 피해를 입습니다.
	
	
	후방 지원 I
	후방 2열에서 전투를 시작하는 아군 유닛이 20%의 공격 속도를 얻습니다.
	
	
	방패진 I
	후방 2열에서 전투를 시작하는 아군 유닛이 25의 방어력 및 마법 저항력을 얻습니다.
	
	
	전투 마법사 I
	전방 2열에서 전투를 시작하는 아군 유닛이 25의 주문력을 얻습니다.
	
	
	명상 I
	아이템을 장착하지 않은 아군 유닛이 매초 5의 마나를 회복합니다.
	
	
	사이버네틱 통신 I
	아이템을 장착한 아군 챔피언이 150의 체력을 얻고 매초 2의 마나를 회복합니다.
	
	
	사이버네틱 외피 I
	아이템을 가진 챔피언이 150의 체력과 25의 방어력을 얻습니다.
	
	
	재생의 바람 I
	전투 시작 10초 후 아군 유닛이 잃은 체력의 50%를 회복합니다.
	
	
	문제가 두 배 I
	전장에 똑같은 챔피언이 정확히 둘 있다면 둘 다 공격력, 주문력, 방어력, 마법 저항력이 25 상승합니다. 챔피언을 3성으로 업그레이드하면 동일한 2성 챔피언 하나를 획득합니다.
	
	
	3에 깃든 힘 I
	아군 3단계 유닛의 체력이 133, 시작 마나가 13, 공격 속도가 13% 증가합니다.
	
	
	파란색 배터리 I
	아군 유닛이 스킬을 사용한 후 10의 마나를 회복합니다.
	
	
	진정한 정의
	아군 집행자가 체력이 80% 아래인 적에게 고정 피해를 입힙니다. 세주아니를 획득합니다.
	
	
	연미복 심장
	팀에 연미복 1명이 추가로 포함된 것으로 간주합니다. 탈론을 획득합니다.
	
	
	비축 에너지
	아군 마법공학 챔피언들이 아군 마공학 핵의 파동에 맞을 때 마다 1의 주문력을 영구적으로 얻습니다. 스웨인을 획득합니다.
	
	
	마법공학 방출
	마법공학 챔피언의 체력이 처음으로 60% 아래로 떨어지면 주변 적이 다음 스킬을 사용할 때까지 최대 마나가 50% 증가합니다. 녹턴을 획득합니다.
	
	
	넘치는 힘
	타격대가 2번 기본 공격할 때마다 다음 기본 공격의 치명타 확률이 75% 증가합니다. 렉사이를 획득합니다.
	
	
	타격대 심장
	팀에 타격대 1명이 추가로 포함된 것으로 간주합니다. 렉사이를 획득합니다.
	
	
	도전자 단합
	모든 아군 챔피언이 도전자 특성의 효과를 받습니다. 도전자의 수는 변하지 않습니다. 퀸을 획득합니다.
	
	
	사시사철
	무작위 4단계 챔피언 셋을 획득합니다.
	
	
	꼬마 거인
	전략가가 체력을 35 회복하고, 크기가 커지고, 최대 체력이 135까지 증가합니다.
*/

class CAugmentation final : public CGameObject
{
public:
	enum AUGMENTATION {
		// 계산된 패배, 고대의 기록 보관소, 지배, 아이템 꾸러미 I, 허수아비 전선, 초고속 모드,
		Calculated_Loss, Ancient_Archives, Dominance, Item_Grab_Bag_I, Phony_Frontline, Hyper_Roll, 

		// 판도라의 아이템, 초월, 다른 태생 I, 천상의 축복 I, 나이프의 날 I
		Pandora_Items, Ascension, Built_Different_I, Celestial_Blessing_I, Knife_Edge_I,

		// 사이버네틱 이식술 I, 추방자 I, 경량급 I, 응급처치 키트, 임시변통 방어구 I
		Cybernetic_Implants_I, Exiles_I, Featherweights_I, First_Aid_Kit, Makeshift_Armor_I,
		
		// 단결된 의지 I, 사냥의 전율 I, 약자, 약점 I, 비전 마법사 심장
		Stand_United_I, Thrill_of_the_Hunt_I, Underdogs, Weakspot_I, Arcanist_Heart,

		// 룬 보호막 I, 암살자 심장, 자객, 경호대 심장, 내가 지킨다, 난동꾼 심장
		Runic_Shield_I, Assassin_Heart, Cutthroat, Bodyguard_Heart, Stand_Behind_Me, Bruiser_Heart,
		
		// 강인함, 도전자 심장, 준비 태세, 화학적 과부하 I, 화학공학 심장
		Shrug_It_Off, Challenger_Heart, En_Garde, Chemical_Overload_I, Chemtech_Heart, 
		
		// 시계태엽 심장, 강화술사 심장, 불타는 향로, 집행자 심장, 자가 복구
		Clockwork_Heart, Enchanter_Heart, Ardent_Censer, Enforcer_Heart, Self_Repair,
		
		// 용병 심장, 해적, 돌연변이 심장, 불안정한 진화, 학자 심장
		Mercenary_Heart, Pirates, Mutant_Heart, Unstable_Evolution, Scholar_Heart,
		
		// 평생 학습, 고물상 심장, 저격수 심장, 이중주, 사교계 심장, 범죄 조직 심장
		Lifelong_Learning, Scrap_Heart, Sniper_Heart, Duet, Socialite_Heart, Syndicate_Heart,
		
		// 물려주기, 수금, 쌍발총 심장, 보물창고 I, 대격변 생성기, 수호자 I
		One_For_All, Payday, Twinshot_Heart, Treasure_Trove_I, Recombobulator, Keepers_I,
		
		// 고전압 I, 분해기 I, 루덴의 메아리 I, 후방 지원 I, 방패진 I, 전투 마법사 I
		Electrocharge_I, Disintegrator_I, Luden_Echo_I, Backfoot_I, Phalanx_I, Battlemage_I,
		
		// 명상 I, 사이버네틱 통신 I, 사이버네틱 외피 I, 재생의 바람 I, 문제가 두 배 I
		Meditation_I, Cybernetic_Uplink_I, Cybernetic_Shell_I, Second_Wind_I, Double_Trouble_I,
		
		// 3에 깃든 힘 I, 파란색 배터리 I, 진정한 정의, 연미복 심장, 비축 에너지, 마법공학 방출
		Tri_Force_I, Blue_Battery_I, True_Justice, Debonair_Heart, Stored_Power, Hexnova,
		
		// 넘치는 힘, 타격대 심장, 도전자 단합, 사시사철, 꼬마 거인
		Overpower, Striker_Heart, Challenger_Unity, Four_Score, Tiny_Titans
	};

private:
	explicit CAugmentation(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CAugmentation(const CAugmentation& rhs);
	virtual ~CAugmentation() = default;

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
	static CAugmentation* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


public:
	const list<AUGMENTATION>& Get_ActivatedAugmentation() const { return m_ActivatedAugmentation; }

private:
	list<AUGMENTATION> m_ActivatedAugmentation; // 활성화된 증강체 list

};

END