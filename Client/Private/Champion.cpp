#include "stdafx.h"
#include "..\Public\Champion.h"
#include "GameInstance.h"

CChampion::CChampion(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
	m_eType = OT_CHAMPION;
}

CChampion::CChampion(const CChampion & rhs)
	: CGameObject(rhs)
	, m_bShow(rhs.m_bShow)
{
	m_eType = rhs.m_eType;
}

HRESULT CChampion::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CChampion::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_AnimationIndex(2);

	CHAMPDESC ChampDesc = *((CHAMPDESC*)pArg);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4((XMFLOAT4*)ChampDesc.TransformMatrix.m[3]));

	return S_OK;
}

void CChampion::Tick(_double TimeDelta)
{
	if (!m_bShow)
		return;

	__super::Tick(TimeDelta);

	// 챔피언의 FSM 적용
	Champion_FSM();

	// 공격속도가 증가됬는지 확인
	Check_IncreaseAttackSpeed(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f)) {
		// 모델의 애니메이션 행렬 업데이트
		m_pModelCom->Update(TimeDelta * 0.2);

		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CChampion::LateTick(_double TimeDelta)
{
	if (!m_bShow)
		return;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom &&
		true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CChampion::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 0)))
			return E_FAIL;
	}

#ifdef _DEBUG
	m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CChampion::Attack()
{
}

void CChampion::Skill()
{
}

void CChampion::Champion_FSM(void)
{
	switch (m_eState)
	{
	case Client::CChampion::CS_IDLE:
		// 스테이지 시작 전에 IDLE, 스테이지 시작 후엔 CS_MOVE로 변형
		break;
	case Client::CChampion::CS_RUN:
		// 마나가 가득찼는지 확인, 가득 찼다면, CS_SKILL로 변형
		if (m_ChampionDesc.iCurMP >= m_ChampionDesc.iMaxMP)
			m_eState = CS_SPELL1;

		// 마나가 가득차지 않았다면 사거리 내에 챔피언이 있는지 확인 있다면 CS_ATTACK로 변형
		else {
			
		}

		// 사거리 내에 챔피언이 있는지 확인하여 없으면 CS_MOVE로 계속 이동
		break;
	case Client::CChampion::CS_ATTACK1:
		// 사거리 내에 챔피언이 있다면 CS_ATTACK 지속, 없다면 CS_MOVE로 변형
		Attack();
		break;
	case Client::CChampion::CS_SPELL1:
		// 스킬 사용후 CS_ATTACK으로 이동
		Skill();
		break;
	case Client::CChampion::CS_CELEBRATION:
		// 스테이지가 끝났다면 다음 스테이지로 넘어가기 전까지 CS_DANCE 지속
		break;
	default:
		break;
	}
}

CChampion*  CChampion::Check_InRange(void)
{
	CChampion* pChamp = nullptr;

	

	return pChamp;
}

HRESULT CChampion::SetUp_Components(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CHAMPDESC ChampDesc = *((CHAMPDESC*)pArg);
	m_ChampionDesc = ChampDesc;

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, m_ChampionDesc.pPrototype_ObjectName, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.15f, 0.f);
	ColliderDesc.vSize = _float3(0.15f, 0.3f, 0.15f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;


	


	return S_OK;
}

HRESULT CChampion::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CChampion * CChampion::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CChampion*	pInstance = new CChampion(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CChampion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CChampion::Clone(void * pArg)
{
	CChampion*	pInstance = new CChampion(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CChampion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChampion::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}

bool CChampion::Equip(CItem * _pItem)
{
	// 아이템 장착 여부 확인
	if (!Check_Equip(_pItem))
		return false;

	// 기존에 조각 아이템이 있는지 확인
	CItem* pItem = *find_if(m_Equipment.begin(), m_Equipment.end(), [](CItem* _pItem) {
		return _pItem->Get_ItemType() == CItem::ITEM_TYPE::PIECE;
	});

	// 조각 아이템이 존재한다면
	if (nullptr != _pItem) {
		// 추가한 아이템과 
	}

	// 인자로 받은 아이템의 정보를 챔피언에게 장착
	m_Equipment.push_back(_pItem);

	// 장착한 아이템의 정보를 챔피언의 정보에 더함.
	const CItem::ITEMDESC& Desc = _pItem->Get_ItemDesc();

	// 체력
	m_ChampionDesc.iAdd_Item_HP += Desc.iHP;

	// 마나
	m_ChampionDesc.iAdd_Item_MP += Desc.iMP;

	// 공격력, 주문력
	m_ChampionDesc.iAdd_Item_AttackPower += Desc.iAttackPower;
	m_ChampionDesc.iAdd_Item_AbilityPower += Desc.iAbilityPower;

	// 방어력, 마법저항력
	m_ChampionDesc.iAdd_Item_Defensive += Desc.iDefensive;
	m_ChampionDesc.iAdd_Item_MagicResistance += Desc.iMagicResistance;

	// 치명타, 회피
	m_ChampionDesc.iAdd_Item_Critical += Desc.iCritical;
	m_ChampionDesc.iAdd_Item_Avoidance += Desc.iAvoidance;

	// 공격속도, 이동속도
	m_ChampionDesc.iAdd_Item_AttackSpeed += Desc.iAttackSpeed;
	m_ChampionDesc.iAdd_Item_MoveSpeed += Desc.iMoveSpeed;

	// 최종 공격속도 갱신 - 공격속도 공식 적용(기본공속 * (1 + 추가 공격속도))
	m_ChampionDesc.dFinalAttackSpeed = m_ChampionDesc.iBaseAttackSpeed * (1.0 + m_ChampionDesc.iMoveSpeed + m_ChampionDesc.iAdd_Item_MoveSpeed + m_ChampionDesc.iAdd_Synergy_MoveSpeed);
}

bool CChampion::Check_Equip(CItem * pItem)
{
	// 추가하려는 아이템이 조각 아이템인지, 완품템인지에 따라 아이템 추가 가능 여부 확인

	// 추가 아이템이 조각 템이라면
	if (pItem->Get_ItemType() == CItem::ITEM_TYPE::PIECE) {
		// 아이템을 장착할 수 있는 칸이 있는지 확인

		// 아이템이 3개라면 장착못하지만
		if (m_Equipment.size() >= 3) {
			if (m_Equipment.at(0)->Get_ItemType() == CItem::ITEM_TYPE::PIECE) {

			}
		}
	}

	// 추가 아이템이 완품템이라면
	else {
		// 아이템을 장착할 수 있는 칸이 있는지 확인
		if (m_Equipment.size() >= 3)
			return false;
	}
	return false;
}

void CChampion::IncreaseStatus(CHAMPION_STATUS eStatus, int iStatus, double duration)
{
	INCREASEDESC IncreaseDesc;

	// 증가하는 스탯 타입
	IncreaseDesc.m_eStatus = eStatus;

	// 증가 지속시간
	IncreaseDesc.m_dwTimeStart = 0.0;
	IncreaseDesc.m_dwTimeEnd = duration;

	switch (eStatus)
	{
	case Client::CChampion::CI_HP:
		m_ChampionDesc.iCurHP += iStatus;
		m_ChampionDesc.iMaxHP += iStatus;
		break;
	case Client::CChampion::CI_MP:
		m_ChampionDesc.iCurMP += iStatus;
		m_ChampionDesc.iMaxMP += iStatus;
		break;
	case Client::CChampion::CI_AD:
		m_ChampionDesc.iAttackPower += iStatus;
		break;
	case Client::CChampion::CI_AP:
		m_ChampionDesc.iAbilityPower += iStatus;
		break;
	case Client::CChampion::CI_DF:
		m_ChampionDesc.iDefensive += iStatus;
		break;
	case Client::CChampion::CI_MR:
		m_ChampionDesc.iMagicResistance += iStatus;
		break;
	case Client::CChampion::CI_CRI:
		m_ChampionDesc.iCritical += iStatus;
		break;
	case Client::CChampion::CI_AVOID:
		m_ChampionDesc.iAvoidance += iStatus;
		break;
	case Client::CChampion::CI_AS:
		m_ChampionDesc.dFinalAttackSpeed += iStatus;
		break;
	case Client::CChampion::CI_MS:
		m_ChampionDesc.iMoveSpeed += iStatus;
		break;
	case Client::CChampion::CI_END:
		break;
	default:
		break;
	}
}

void CChampion::IncreaseAttackSpeed(int iAttackSpeed, double duration)
{
	// 공격속도 증가를 true로 함.
	m_bIncreaseAttackSpeed = true;

	// 공격속도 증가의 지속시간을 설정
	m_dwTimeStart_IncreaseAttackSpeed = 0;
	m_dwTimeEnd_IncreaseAttackSpeed = duration;

	// 증가한 공격속도 대입
	m_ChampionDesc.iAdd_Characteristic_AttackSpeed = iAttackSpeed;
}

void CChampion::Check_IncreaseAttackSpeed(double duration)
{
	// 공격 속도가 증가된다면
	if (m_bIncreaseAttackSpeed) {
		// 증가 지속시간이 끝났다면
		if (m_dwTimeEnd_IncreaseAttackSpeed < m_dwTimeStart_IncreaseAttackSpeed) {
			// 공격속도 증가를 멈춘다.
			m_bIncreaseAttackSpeed = false;
			return;
		}

		// 지속시간을 더한다.
		m_dwTimeStart_IncreaseAttackSpeed += duration;
	}
}
