#include "stdafx.h"
#include "..\Public\PersonalMap.h"
#include "GameInstance.h"

CPersonalMap::CPersonalMap(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CPersonalMap::CPersonalMap(const CPersonalMap & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPersonalMap::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPersonalMap::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPersonalMap::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

}

void CPersonalMap::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom &&
		true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CPersonalMap::Render()
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


		if (FAILED(m_pModelCom->Render(m_pShaderCom, nullptr, i, 2)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPersonalMap::SetUp_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	PersonalMapDesc PMDesc;
	ZeroMemory(&PMDesc, sizeof(PersonalMapDesc));
	if (nullptr != pArg) {
		PMDesc = *(PersonalMapDesc*)pArg;
	}

	// 우선 이동 행렬만 적용, 크기, 회전은 나중에 처리 *****
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4((XMFLOAT4*)PMDesc.TransformMatrix.m[3]));


	// 지형에 따라 다른 피킹을 위한 rect를 깐다.
	switch (PMDesc.eType)
	{
	case Client::CPersonalMap::MAP_NAME_Celebrationtomorrow:
		/* For.Com_Model */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_CelebrationTomorrow"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case Client::CPersonalMap::MAP_NAME_Hexbattle:
		/* For.Com_Model */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Hexbattle"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case Client::CPersonalMap::MAP_NAME_ZaunCity:
		/* For.Com_Model */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_ZaunCity"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case Client::CPersonalMap::MAP_NAME_Jinx:
		/* For.Com_Model */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Jinx"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case Client::CPersonalMap::MAP_NAME_Vi:
		/* For.Com_Model */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Vi"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case Client::CPersonalMap::MAP_NAME_Spiritblossom_kami:
		/* For.Com_Model */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Spiritblossom_kami"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case Client::CPersonalMap::MAP_NAME_Spiritblossom_reality:
		/* For.Com_Model */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Spiritblossom_reality"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case Client::CPersonalMap::MAP_NAME_Spiritblossom_yokai:
		/* For.Com_Model */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Spiritblossom_yokai"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	default:
		return E_FAIL;
		break;
	}

	m_pModelCom->Update(0.0);

	m_eMapName = PMDesc.eType;

	return S_OK;
}

HRESULT CPersonalMap::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPositionFloat4(), sizeof(_float4))))
		return E_FAIL;

	/* For.LightDesc */
	const LIGHTDESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightPos", &pLightDesc->vPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fRange", &pLightDesc->fRange, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CPersonalMap * CPersonalMap::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CPersonalMap*	pInstance = new CPersonalMap(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPersonalMap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPersonalMap::Clone(void * pArg)
{
	CPersonalMap*	pInstance = new CPersonalMap(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPersonalMap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPersonalMap::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}

void CPersonalMap::checkSynergy(void)
{
	// 필드에 있는 챔피언들을 순회하며
	for (auto& Champion : m_Champions) {
		// 챔피언이 가지고 있는 시너지에 맞춰 검사한다.
		for (auto& Synergy : Champion->Get_Synergy()) {
			// 검사시, 시너지가 활성화되었다면, 추가 능력치를 더한다.
			if (m_ChampionSynergy[Synergy]) {
				switch (Synergy)
				{
				case Client::Rival:
					Check_Rival();
					break;
				case Client::Scrap:
					Check_Scrap();
					break;
				case Client::Glutton:
					Check_Glutton();
					break;
				case Client::Mutant:
					Check_Mutant();
					break;
				case Client::Hextech:
					Check_Hextech();
					break;
				case Client::Syndicate:
					Check_Syndicate();
					break;
				case Client::Socialite:
					Check_Socialite();
					break;
				case Client::Clockwork:
					Check_Clockwork();
					break;
				case Client::Debonair:
					Check_Debonair();
					break;
				case Client::Yordle:
					Check_Yordle();
					break;
				case Client::Yordle_Lord:
					Check_Yordle_Lord();
					break;
				case Client::Mercenary:
					Check_Mercenary();
					break;
				case Client::Enforcer:
					Check_Enforcer();
					break;
				case Client::Chemtech:
					Check_Chemtech();
					break;
				case Client::Mastermind:
					Check_Mastermind();
					break;
				case Client::Enchanter:
					Check_Enchanter();
					break;
				case Client::Colossus:
					Check_Colossus();
					break;
				case Client::Bodyguard:
					Check_Bodyguard();
					break;
				case Client::Bruiser:
					Check_Bruiser();
					break;
				case Client::Challenger:
					Check_Challenger();
					break;
				case Client::Transformer:
					Check_Transformer();
					break;
				case Client::Arcanist:
					Check_Arcanist();
					break;
				case Client::Twinshot:
					Check_Twinshot();
					break;
				case Client::Assassin:
					Check_Assassin();
					break;
				case Client::Sniper:
					Check_Sniper();
					break;
				case Client::Striker:
					Check_Striker();
					break;
				case Client::Scholar:
					Check_Scholar();
					break;
				case Client::Innovator:
					Check_Innovator();
					break;
				default:
					break;
				}
			}
		}
	}
}

void CPersonalMap::AssignChampion(CChampion * pChampion)
{
	if (nullptr == pChampion)
		return;

	// 인자로 받은 챔피언을 넣고 멤버변수에 넣고
	m_Champions.push_back(pChampion);

	// 해당 챔피언의 시너지를 활성화시킨다.
	for (auto& Synergy : pChampion->Get_Synergy()) {
		++m_ChampionSynergy[Synergy];
	}

	// 활성화된 시너지 적용
	checkSynergy();
}

void CPersonalMap::Check_Rival()
{
	/* 
		이 특성은 정확히 하나의 경쟁자 유닛이 있을 때만 활성화됩니다.경쟁자들은 서로 같은 팀에 있기 싫어하니까요.

		바이의 마나 소모량 20 감소

		징크스가 처치 관여 시 3초 동안 공격 속도 40 % 증가
	*/

	if (m_ChampionSynergy[SYNERGY::Rival] == 1) {
		for (auto& Champion : m_Champions) {
			ChampionName ChampName = Champion->Get_ChampionDesc().ChampName;
			if (ChampName == ChampionName::Jinx) {
				Champion->IncreaseAttackSpeed(40, 3.0);
			}
			if(ChampName == ChampionName::Bye) {

			}
		}
	}
}

void CPersonalMap::Check_Scrap()
{
}

void CPersonalMap::Check_Glutton()
{
}

void CPersonalMap::Check_Mutant()
{
}

void CPersonalMap::Check_Hextech()
{
}

void CPersonalMap::Check_Syndicate()
{
}

void CPersonalMap::Check_Socialite()
{
}

void CPersonalMap::Check_Clockwork()
{
}

void CPersonalMap::Check_Debonair()
{
}

void CPersonalMap::Check_Yordle()
{
}

void CPersonalMap::Check_Yordle_Lord()
{
}

void CPersonalMap::Check_Mercenary()
{
}

void CPersonalMap::Check_Enforcer()
{
}

void CPersonalMap::Check_Chemtech()
{
}

void CPersonalMap::Check_Mastermind()
{
}

void CPersonalMap::Check_Enchanter()
{
}

void CPersonalMap::Check_Colossus()
{
}

void CPersonalMap::Check_Bodyguard()
{
}

void CPersonalMap::Check_Bruiser()
{
}

void CPersonalMap::Check_Challenger()
{
}

void CPersonalMap::Check_Transformer()
{
}

void CPersonalMap::Check_Arcanist()
{
}

void CPersonalMap::Check_Twinshot()
{
}

void CPersonalMap::Check_Assassin()
{
}

void CPersonalMap::Check_Sniper()
{
}

void CPersonalMap::Check_Striker()
{
}

void CPersonalMap::Check_Scholar()
{
}

void CPersonalMap::Check_Innovator()
{
}