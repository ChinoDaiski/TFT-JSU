#include "stdafx.h"
#include "..\Public\Mouse.h"
#include "GameInstance.h"
#include "Champion.h"
#include "Player.h"
#include "PickingObject.h"

CMouse::CMouse(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CMouse::CMouse(const CMouse & rhs)
	: CGameObject(rhs)
	, m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CMouse::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));
	
	return S_OK;
}

HRESULT CMouse::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	MOUSEDESC MouseDesc;
	ZeroMemory(&MouseDesc, sizeof(MOUSEDESC));
	if (nullptr == pArg)
		return E_FAIL;
	
	MouseDesc = *((MOUSEDESC*)pArg);

	// 길이 설정
	m_fSizeX = MouseDesc.fSize.x;
	m_fSizeY = MouseDesc.fSize.y;

	m_fX = 0;
	m_fY = 0;

	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CMouse::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 마우스 좌표 갱신
	Cal_MousePos();

	// 피킹된 오브젝트가 있을 경우 움직이는 함수
	Move_PickingObject();

	// 피킹된 오브젝트와 타일의 충돌처리
	Check_Collision();


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet((m_fX + m_fSizeX * 0.5 - 3.f) - g_iWinCX * 0.5f, -(m_fY + m_fSizeY * 0.5 - 15.f) + g_iWinCY * 0.5f + (m_fY - g_iWinCY * 0.5) * 0.025f, 0.f, 1.f));
}

void CMouse::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	// 입력 관련 처리
	Device_Input();

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CMouse::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	_float4x4	ViewMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	_float4x4	ProjMatrixTP;
	XMStoreFloat4x4(&ProjMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &ProjMatrixTP, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(5)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMouse::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mouse"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CMouse::Device_Input(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 현재 왼쪽 마우스를 눌렀을 경우
	if (pGameInstance->Get_DIMCurButtonClick(CInput_Device::MOUSEBUTTONSTATE::DIMB_LBUTTON)) {
		// 이전에 왼쪽 마우스를 누른적이 없을 경우
		if (!pGameInstance->Get_DIMPreButtonClick(CInput_Device::MOUSEBUTTONSTATE::DIMB_LBUTTON)) {
			// 오브젝트를 검사하여 충돌하는지 확인
			// 충돌시 오브젝트 정보를 저장
			m_pPickekdObject = Picking_ChampAndItem();
			if (nullptr != m_pPickekdObject) {
				m_vPickingObjectOriginPos = dynamic_cast<CTransform*>(m_pPickekdObject->Get_Component(TEXT("Com_Transform")))
					->Get_State(CTransform::STATE_POSITION);
			}
		}
	}
	// 현재 왼쪽 마우스를 누르지 않은 경우
	else {
		// 왼쪽 마우스를 이전에 눌렀던 경우
		if (pGameInstance->Get_DIMPreButtonClick(CInput_Device::MOUSEBUTTONSTATE::DIMB_LBUTTON)) {
			// 들고 있는 오브젝트(챔피언 / 아이템)가 있을 경우
			if (nullptr != m_pPickekdObject) {

				// 마우스와 오브젝트가 충돌 했는지 확인한다.
				if (nullptr != m_pCollisionTile) {
					// 충돌된 타일의 위치정보를 가져온다.
					_vector vTilePos = dynamic_cast<CTransform*>(m_pCollisionTile->Get_Component(TEXT("Com_Transform")))
						->Get_State(CTransform::STATE_POSITION);

					// 충돌된 타일 정보중 x,z 좌표만 가져와 위치값을 바꾼다.
					dynamic_cast<CTransform*>(m_pPickekdObject->Get_Component(TEXT("Com_Transform")))
						->Set_State(CTransform::STATE_POSITION,
							XMVectorSet(
								XMVectorGetX(vTilePos),
								XMVectorGetY(m_vPickingObjectOriginPos),
								XMVectorGetZ(vTilePos),
								1.f
							));
				}
				else {
					// 충돌하지 않았다면 선택된 오브젝트를 원래 자리로 옮긴다.
					dynamic_cast<CTransform*>(m_pPickekdObject->Get_Component(TEXT("Com_Transform")))
						->Set_State(CTransform::STATE_POSITION, m_vPickingObjectOriginPos);
				}

				// 들고 있는 오브젝트가 챔피언인 경우
				if (m_pPickekdObject->Get_ObjectType() == OT_CHAMPION) {
					// 땐 장소가 빈 타일일 경우 해당 타일에 챔피언을 놓는다.
					// 땐 장소가 다른 챔피언이 존재하면 서로 위치를 교환한다.
					// 땐 장소가 상점 창이면 판매한다. 판매되는 챔피언이 아이템을 들고 있을 경우 해당 아이템들은 아이템 저장 공간으로 보낸다. 만약 아이템 저장 공간이 가득 차 있다면
					// 아이템을 포장하여 맵 가운데 배치한다.
					int a = 10;

				}
				// 들고 있는 오브젝트가 아이템인 경우
				else if (m_pPickekdObject->Get_ObjectType() == OT_ITEM) {
					// 떈 장소가 챔피언이 있는 경우 해당 챔피언에게 아이템을 장착한다.
					// 땐 장소가 다른 아이템이면 서로 위치를 교환한다.
					int a = 10;
				}

				// 다 했다면 피킹 오브젝트를 nullptr로 만든다.
				m_pPickekdObject = nullptr;
				
				// 충돌 관련된 타일 정보도 nullptr로 만든다.
				dynamic_cast<CTile*>(m_pCollisionTile)->Set_Collision(false);
				m_pCollisionTile = nullptr;
			}
		}
	}

	pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_TILE_PERSONAL01_FIELD);

	RELEASE_INSTANCE(CGameInstance);
}

void CMouse::Cal_MousePos()
{
	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd, &m_ptMouse);

	m_fX = m_ptMouse.x;
	m_fY = m_ptMouse.y;
}

void CMouse::Move_PickingObject(void)
{
	// 마우스가 피킹한 오브젝트가 존재한다면
	if (nullptr != m_pPickekdObject) {
		// 해당 오브젝트의 움직임을 마우스를 따라오게한다.

		// 마우스와 지형 오브젝트가 충돌된 위치를 가져온다.
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		// 마우스 피킹 관련 레이어를 가져온다.
		CLayer* pLayer_PickingObject = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PICKING_OBJECT);

		auto& ObjectList = pLayer_PickingObject->Get_ObjectList();

		_matrix WorldMatrix = XMMatrixIdentity();

		// 레이어에 있는 오브젝트와 마우스의 피킹을 검사한다.
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

		// 거리 값이 바꾸지 않았다면 충돌하지 않은 겻이다.
		if (XMVector3Equal(vDist, XMVectorSet(0.f, 0.f, 0.f, 1.f))) {
			;
		}
		// 거리 값이 바뀌었다면 충돌 한 것이다.
		else {
			// 해당 위치를 피킹된 오브젝트의 위치로 삼는다.
			dynamic_cast<CTransform*>(m_pPickekdObject->Get_Component(TEXT("Com_Transform")))
				->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vDist, 1.f));
		}
	}
}

void CMouse::Check_Collision()
{
	// 충돌체가 있다면
	if (nullptr != m_pPickekdObject) {

		// 타일에 대한 정보를 플레이어가 들고 있으므로, 플레이어에 대한 정보를 가져온다.
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		// 플레이어 레이어의 정보를 가져온다.
		CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);

		if (nullptr == pLayer)
			return;

		// 플레이어의 정보를 가져온다.
		auto& ObjectList = pLayer->Get_ObjectList();

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(*ObjectList.begin());
		auto& FieldTile = pPlayer->Get_Tile_Field();
		auto& LoungeTile = pPlayer->Get_Tile_Lounge();

		// 필드 및 라운지에 있는 모든 타일의 충돌체는 Com_AABB이다.
		// 피킹 오브젝트(아이템, 챔피언)의 충돌체 또한 Com_AABB이다.
		// 피킹 오브젝트의 충돌체를 가져온다.

		CCollider* pCollider = dynamic_cast<CCollider*>(m_pPickekdObject->Get_Component(TEXT("Com_AABB")));

		// 필드에 있는 타일을 순회하며 검사한다.
		for (auto& Tile : FieldTile) {
			if (dynamic_cast<CCollider*>(Tile->Get_Component(TEXT("Com_AABB")))->Collsion_AABB(pCollider))
				m_pCollisionTile = Tile;
			else
				dynamic_cast<CTile*>(Tile)->Set_Collision(false);
		}
		// 라운지에 있는 타일을 순회하며 검사한다.
		for (auto& Tile : LoungeTile) {
			if (dynamic_cast<CCollider*>(Tile->Get_Component(TEXT("Com_AABB")))->Collsion_AABB(pCollider))
				m_pCollisionTile = Tile;
			else
				dynamic_cast<CTile*>(Tile)->Set_Collision(false);
		}

		dynamic_cast<CTile*>(m_pCollisionTile)->Set_Collision(true);

		RELEASE_INSTANCE(CGameInstance);
	}
}

CGameObject * CMouse::Picking_ChampAndItem(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 챔피언과 아이템의 충돌체와 마우스의 레이를 충돌처리하여 충돌된 오브젝트를 마우스로 피킹한다.

	// 플레이어의 정보를 가져온다.
	CLayer* pLayerChampion = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);

	if (nullptr == pLayerChampion)
		return nullptr;

	// 플레이어의 정보를 들고온다.
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(*pLayerChampion->Get_ObjectList().begin());

	// 플레이어가 가지고 있는 챔피언의 정보를 가져온다.
	auto& vChampion_Lounge = pPlayer->Get_Champion_Lounge();
	auto& vChampion_Field = pPlayer->Get_Champion_Field();

	// 마우스의 레이/레이포스의 정보를 들고온다.
	CMousePicking::RAYDESC RayDesc = pGameInstance->Get_vRayDesc();
	_vector vRay, vRayPos;
	_float fDist;
	vRay = XMVectorSetW(XMLoadFloat3(&RayDesc.vRay), 0.f);
	vRayPos = XMVectorSetW(XMLoadFloat3(&RayDesc.vRayPos), 1.f);

	// 마우스와 충돌된 오브젝트들의 리스트
	map<_float, CGameObject*> vCollObjects;

	// 라운지에 있는 타일들을 돌아다니며 타일이 가지고 있는 챔피언들의 충돌체와 마우스의 피킹을 검사한다.
	for (auto& pChampion : vChampion_Lounge) {
		// 해당 챔피언이 가지고 있는 충돌체의 정보를 가져온다.
		if (true == dynamic_cast<CCollider*>(pChampion->Get_Component(TEXT("Com_AABB")))->Collsion_AABB(vRay, vRayPos, fDist)) {
			vCollObjects.emplace(fDist, pChampion);
		}
	}
	// 필드에 있는 타일들을 돌아다니며 타일이 가지고 있는 챔피언들의 충돌체와 마우스의 피킹을 검사한다.
	for (auto& pChampion : vChampion_Field) {
		// 해당 챔피언이 가지고 있는 충돌체의 정보를 가져온다.
		if (true == dynamic_cast<CCollider*>(pChampion->Get_Component(TEXT("Com_AABB")))->Collsion_AABB(vRay, vRayPos, fDist)) {
			vCollObjects.emplace(fDist, pChampion);
		}
	}

	// 오브젝트 리스트가 존재한다면
	if (!vCollObjects.empty())
		// 가장 거리가 짧은 오브젝트를 선택한다.
		return vCollObjects.begin()->second;

	// 오브젝트 리스트가 존재하지 않는다면
	else
		// 선택한 오브젝트가 존재하지 않는다.
		return nullptr;

	// 마우스의 레이와 챔피언 / 아이템 이 충돌 처리된 경우
	// 해당 챔피언 / 아이템 을 마우스가 들고 있게한다.

	RELEASE_INSTANCE(CGameInstance);
}

CMouse * CMouse::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CMouse*	pInstance = new CMouse(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMouse::Clone(void * pArg)
{
	CMouse*	pInstance = new CMouse(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMouse::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
