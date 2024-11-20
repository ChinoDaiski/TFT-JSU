#include "stdafx.h"
#include "..\Public\Player.h"
#include "GameInstance.h"
#include "PickingObject.h"
#include "Market.h"

CPlayer::CPlayer(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
	m_eType = OT_PLAYER;
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
	, m_ProjMatrix(rhs.m_ProjMatrix)
	, m_eState(rhs.m_eState)
{
	m_eType = rhs.m_eType;
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));


	
	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 1.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	PressKey();

	Player_FSM(TimeDelta);

	if (m_eCurrMapName != CPersonalMap::MAP_NAME_END) {
		for (auto& Tile : m_pTile_Lounge[m_eCurrMapName])
			Tile->Tick(TimeDelta);

		for (auto& Tile : m_pTile_Field[m_eCurrMapName])
			Tile->Tick(TimeDelta);
	}

	// 모델의 애니메이션 행렬 업데이트
	m_pModelCom->Update(TimeDelta * 0.3);
}

void CPlayer::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);


	if (m_eCurrMapName != CPersonalMap::MAP_NAME_END) {
		for (auto& Tile : m_pTile_Lounge[m_eCurrMapName])
			Tile->LateTick(TimeDelta);

		for (auto& Tile : m_pTile_Field[m_eCurrMapName])
			Tile->LateTick(TimeDelta);
	}

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CPlayer::Render()
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

		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, (int)m_eName)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer::SetUp_Components(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	PLAYERDESC PlayerDesc = *((PLAYERDESC*)pArg);

	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimCharacter"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, PlayerDesc.pPrototype_CharacterName, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	// 초기 위치 정보 세팅
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&PlayerDesc.vPos), 1.f));

	// 이름 설정
	m_eName = PlayerDesc.CharacterName;

	return S_OK;
}

HRESULT CPlayer::SetUp_ConstantTable(void)
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

CPlayer * CPlayer::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CPlayer*	pInstance = new CPlayer(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}

void CPlayer::Level_Up()
{
}

bool CPlayer::Purchase(CChampion * pChamp)
{
	_uint iMaxSize = 9;
	if (m_Champions_Lounge.size() >= iMaxSize)
		return false;

	// 인자로 받은 챔피언의 정보를 내가 가지고 있는 챔피언의 정보에 추가
	pChamp->Tick(0.0);
	m_Champions_Lounge.push_back(pChamp);

	// 플레이어가 가지고 있는 라운지 타일에 챔피언이 있는지 확인하고, 없다면 추가한다.
	for (auto& Tile : m_pTile_Lounge[m_eCurrMapName]) {
		if (nullptr != Tile->Get_ChampionInfo())
			continue;
		else {
			Tile->Set_ChampionInfo(pChamp);
			return true;
		}
	}
	
	return false;
}

void CPlayer::Select_Object(CGameObject * pObject)
{
}

void CPlayer::Player_FSM(_double TimeDelta)
{
	switch (m_eState)
	{
	case Client::CPlayer::PS_IDLE:
		break;
	case Client::CPlayer::PS_CELEBRATION:
		break;
	case Client::CPlayer::PS_DANCE:
		break;
	case Client::CPlayer::PS_DEATH:
		break;
	case Client::CPlayer::PS_RUN_NORMAL:
	case Client::CPlayer::PS_RUN_FAST:

		// 스피드를 설정한다.
		if (m_eState == PS_RUN_NORMAL) {
			m_pTransformCom->Set_Speed(0.5);
		}
		else {
			m_pTransformCom->Set_Speed(1);
		}

		// 이동한다. 이동이 끝난다면 더이상 이동하지 않는다.
		m_bMove = m_pTransformCom->Chase(XMLoadFloat4(&m_TargetPos), TimeDelta, 0.1);
		m_pTransformCom->LookAt(XMLoadFloat4(&m_TargetPos));

		if (!m_bMove) {
			//m_pModelCom->Update_NextAnim(TimeDelta, 1.0, (int)PS_IDLE, ((_uint)m_eState));
			m_eState = PS_IDLE;
		}
		break;
	default:
		break;
	}

	m_pModelCom->Set_AnimationIndex((int)m_eState);
	m_pModelCom->Update(TimeDelta);
}

void CPlayer::PressKey(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 오른쪽 마우스를 눌렀다면
	if (pGameInstance->Get_DIMCurButtonClick(CInput_Device::DIMB_RBUTTON)) {
		// 목표지점을 지정한다.
		Set_TargetPos();
	}

	// R 버튼을 눌렀을 경우
	if (pGameInstance->Get_DIKeyState(DIK_R)) {
		// 이번에 R버튼을 누르지 않았다면
		if (!pGameInstance->Get_DIKeyPreState(DIK_R)) {
			CLayer* pLayer_Market = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_MARKET);

			if (nullptr != pLayer_Market) {
				dynamic_cast<CMarket*>(*pLayer_Market->Get_ObjectList().begin())->Reroll(m_iLevel);
			}
		}
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::Set_TargetPos(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CLayer* pLayer_PickingObject = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PICKING_OBJECT);

	auto& ObjectList = pLayer_PickingObject->Get_ObjectList();

	_matrix WorldMatrix = XMMatrixIdentity();

	_vector vDist = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	for (auto& Object : ObjectList) {
		_float3* RectPos = dynamic_cast<CPickingObject*>(Object)->Get_RectPos();

		_float3 TriPos[3];
		TriPos[0] = RectPos[0];
		TriPos[1] = RectPos[1];
		TriPos[2] = RectPos[2];
		if (pGameInstance->Picking(TriPos, WorldMatrix, &vDist))
			break;


		TriPos[0] = RectPos[0];
		TriPos[1] = RectPos[2];
		TriPos[2] = RectPos[3];
		if (pGameInstance->Picking(TriPos, WorldMatrix, &vDist))
			break;
	}

	// 만약 거리 값이 바뀌지 않았다면
	if (XMVector3Equal(vDist, XMVectorSet(0.f, 0.f, 0.f, 1.f))) {
		m_bMove = false;
	}
	// 거리 값이 바뀌었다면
	else {
		// 해당 위치를 플레이어의 목표로 삼는다.
		XMStoreFloat4(&m_TargetPos, vDist);

		//바뀐 거리 값으로 플레이어의 위치를 옮긴다.
		m_bMove = true;

		// 플레이어의 상태를 움직이는것으로 한다.
		m_eState = PS_RUN_NORMAL;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::MoveTo(_vector vPos)
{
	
}

void CPlayer::PushTile(CTile::TILE_TYPE eType, CTile * pTile, _uint iMapName)
{
	switch (eType)
	{
	case Client::CTile::TILE_FIELD:
		m_pTile_Field[iMapName].push_back(pTile);
		break;
	case Client::CTile::TILE_LOUNGE:
		m_pTile_Lounge[iMapName].push_back(pTile);
		break;
	default:
		break;
	}

}
