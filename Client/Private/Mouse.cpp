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

	// ���� ����
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

	// ���콺 ��ǥ ����
	Cal_MousePos();

	// ��ŷ�� ������Ʈ�� ���� ��� �����̴� �Լ�
	Move_PickingObject();

	// ��ŷ�� ������Ʈ�� Ÿ���� �浹ó��
	Check_Collision();


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet((m_fX + m_fSizeX * 0.5 - 3.f) - g_iWinCX * 0.5f, -(m_fY + m_fSizeY * 0.5 - 15.f) + g_iWinCY * 0.5f + (m_fY - g_iWinCY * 0.5) * 0.025f, 0.f, 1.f));
}

void CMouse::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	// �Է� ���� ó��
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

	// ���� ���� ���콺�� ������ ���
	if (pGameInstance->Get_DIMCurButtonClick(CInput_Device::MOUSEBUTTONSTATE::DIMB_LBUTTON)) {
		// ������ ���� ���콺�� �������� ���� ���
		if (!pGameInstance->Get_DIMPreButtonClick(CInput_Device::MOUSEBUTTONSTATE::DIMB_LBUTTON)) {
			// ������Ʈ�� �˻��Ͽ� �浹�ϴ��� Ȯ��
			// �浹�� ������Ʈ ������ ����
			m_pPickekdObject = Picking_ChampAndItem();
			if (nullptr != m_pPickekdObject) {
				m_vPickingObjectOriginPos = dynamic_cast<CTransform*>(m_pPickekdObject->Get_Component(TEXT("Com_Transform")))
					->Get_State(CTransform::STATE_POSITION);
			}
		}
	}
	// ���� ���� ���콺�� ������ ���� ���
	else {
		// ���� ���콺�� ������ ������ ���
		if (pGameInstance->Get_DIMPreButtonClick(CInput_Device::MOUSEBUTTONSTATE::DIMB_LBUTTON)) {
			// ��� �ִ� ������Ʈ(è�Ǿ� / ������)�� ���� ���
			if (nullptr != m_pPickekdObject) {

				// ���콺�� ������Ʈ�� �浹 �ߴ��� Ȯ���Ѵ�.
				if (nullptr != m_pCollisionTile) {
					// �浹�� Ÿ���� ��ġ������ �����´�.
					_vector vTilePos = dynamic_cast<CTransform*>(m_pCollisionTile->Get_Component(TEXT("Com_Transform")))
						->Get_State(CTransform::STATE_POSITION);

					// �浹�� Ÿ�� ������ x,z ��ǥ�� ������ ��ġ���� �ٲ۴�.
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
					// �浹���� �ʾҴٸ� ���õ� ������Ʈ�� ���� �ڸ��� �ű��.
					dynamic_cast<CTransform*>(m_pPickekdObject->Get_Component(TEXT("Com_Transform")))
						->Set_State(CTransform::STATE_POSITION, m_vPickingObjectOriginPos);
				}

				// ��� �ִ� ������Ʈ�� è�Ǿ��� ���
				if (m_pPickekdObject->Get_ObjectType() == OT_CHAMPION) {
					// �� ��Ұ� �� Ÿ���� ��� �ش� Ÿ�Ͽ� è�Ǿ��� ���´�.
					// �� ��Ұ� �ٸ� è�Ǿ��� �����ϸ� ���� ��ġ�� ��ȯ�Ѵ�.
					// �� ��Ұ� ���� â�̸� �Ǹ��Ѵ�. �ǸŵǴ� è�Ǿ��� �������� ��� ���� ��� �ش� �����۵��� ������ ���� �������� ������. ���� ������ ���� ������ ���� �� �ִٸ�
					// �������� �����Ͽ� �� ��� ��ġ�Ѵ�.
					int a = 10;

				}
				// ��� �ִ� ������Ʈ�� �������� ���
				else if (m_pPickekdObject->Get_ObjectType() == OT_ITEM) {
					// �� ��Ұ� è�Ǿ��� �ִ� ��� �ش� è�Ǿ𿡰� �������� �����Ѵ�.
					// �� ��Ұ� �ٸ� �������̸� ���� ��ġ�� ��ȯ�Ѵ�.
					int a = 10;
				}

				// �� �ߴٸ� ��ŷ ������Ʈ�� nullptr�� �����.
				m_pPickekdObject = nullptr;
				
				// �浹 ���õ� Ÿ�� ������ nullptr�� �����.
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
	// ���콺�� ��ŷ�� ������Ʈ�� �����Ѵٸ�
	if (nullptr != m_pPickekdObject) {
		// �ش� ������Ʈ�� �������� ���콺�� ��������Ѵ�.

		// ���콺�� ���� ������Ʈ�� �浹�� ��ġ�� �����´�.
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		// ���콺 ��ŷ ���� ���̾ �����´�.
		CLayer* pLayer_PickingObject = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PICKING_OBJECT);

		auto& ObjectList = pLayer_PickingObject->Get_ObjectList();

		_matrix WorldMatrix = XMMatrixIdentity();

		// ���̾ �ִ� ������Ʈ�� ���콺�� ��ŷ�� �˻��Ѵ�.
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

		// �Ÿ� ���� �ٲ��� �ʾҴٸ� �浹���� ���� ���̴�.
		if (XMVector3Equal(vDist, XMVectorSet(0.f, 0.f, 0.f, 1.f))) {
			;
		}
		// �Ÿ� ���� �ٲ���ٸ� �浹 �� ���̴�.
		else {
			// �ش� ��ġ�� ��ŷ�� ������Ʈ�� ��ġ�� ��´�.
			dynamic_cast<CTransform*>(m_pPickekdObject->Get_Component(TEXT("Com_Transform")))
				->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vDist, 1.f));
		}
	}
}

void CMouse::Check_Collision()
{
	// �浹ü�� �ִٸ�
	if (nullptr != m_pPickekdObject) {

		// Ÿ�Ͽ� ���� ������ �÷��̾ ��� �����Ƿ�, �÷��̾ ���� ������ �����´�.
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		// �÷��̾� ���̾��� ������ �����´�.
		CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);

		if (nullptr == pLayer)
			return;

		// �÷��̾��� ������ �����´�.
		auto& ObjectList = pLayer->Get_ObjectList();

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(*ObjectList.begin());
		auto& FieldTile = pPlayer->Get_Tile_Field();
		auto& LoungeTile = pPlayer->Get_Tile_Lounge();

		// �ʵ� �� ������� �ִ� ��� Ÿ���� �浹ü�� Com_AABB�̴�.
		// ��ŷ ������Ʈ(������, è�Ǿ�)�� �浹ü ���� Com_AABB�̴�.
		// ��ŷ ������Ʈ�� �浹ü�� �����´�.

		CCollider* pCollider = dynamic_cast<CCollider*>(m_pPickekdObject->Get_Component(TEXT("Com_AABB")));

		// �ʵ忡 �ִ� Ÿ���� ��ȸ�ϸ� �˻��Ѵ�.
		for (auto& Tile : FieldTile) {
			if (dynamic_cast<CCollider*>(Tile->Get_Component(TEXT("Com_AABB")))->Collsion_AABB(pCollider))
				m_pCollisionTile = Tile;
			else
				dynamic_cast<CTile*>(Tile)->Set_Collision(false);
		}
		// ������� �ִ� Ÿ���� ��ȸ�ϸ� �˻��Ѵ�.
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

	// è�Ǿ�� �������� �浹ü�� ���콺�� ���̸� �浹ó���Ͽ� �浹�� ������Ʈ�� ���콺�� ��ŷ�Ѵ�.

	// �÷��̾��� ������ �����´�.
	CLayer* pLayerChampion = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);

	if (nullptr == pLayerChampion)
		return nullptr;

	// �÷��̾��� ������ ���´�.
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(*pLayerChampion->Get_ObjectList().begin());

	// �÷��̾ ������ �ִ� è�Ǿ��� ������ �����´�.
	auto& vChampion_Lounge = pPlayer->Get_Champion_Lounge();
	auto& vChampion_Field = pPlayer->Get_Champion_Field();

	// ���콺�� ����/���������� ������ ���´�.
	CMousePicking::RAYDESC RayDesc = pGameInstance->Get_vRayDesc();
	_vector vRay, vRayPos;
	_float fDist;
	vRay = XMVectorSetW(XMLoadFloat3(&RayDesc.vRay), 0.f);
	vRayPos = XMVectorSetW(XMLoadFloat3(&RayDesc.vRayPos), 1.f);

	// ���콺�� �浹�� ������Ʈ���� ����Ʈ
	map<_float, CGameObject*> vCollObjects;

	// ������� �ִ� Ÿ�ϵ��� ���ƴٴϸ� Ÿ���� ������ �ִ� è�Ǿ���� �浹ü�� ���콺�� ��ŷ�� �˻��Ѵ�.
	for (auto& pChampion : vChampion_Lounge) {
		// �ش� è�Ǿ��� ������ �ִ� �浹ü�� ������ �����´�.
		if (true == dynamic_cast<CCollider*>(pChampion->Get_Component(TEXT("Com_AABB")))->Collsion_AABB(vRay, vRayPos, fDist)) {
			vCollObjects.emplace(fDist, pChampion);
		}
	}
	// �ʵ忡 �ִ� Ÿ�ϵ��� ���ƴٴϸ� Ÿ���� ������ �ִ� è�Ǿ���� �浹ü�� ���콺�� ��ŷ�� �˻��Ѵ�.
	for (auto& pChampion : vChampion_Field) {
		// �ش� è�Ǿ��� ������ �ִ� �浹ü�� ������ �����´�.
		if (true == dynamic_cast<CCollider*>(pChampion->Get_Component(TEXT("Com_AABB")))->Collsion_AABB(vRay, vRayPos, fDist)) {
			vCollObjects.emplace(fDist, pChampion);
		}
	}

	// ������Ʈ ����Ʈ�� �����Ѵٸ�
	if (!vCollObjects.empty())
		// ���� �Ÿ��� ª�� ������Ʈ�� �����Ѵ�.
		return vCollObjects.begin()->second;

	// ������Ʈ ����Ʈ�� �������� �ʴ´ٸ�
	else
		// ������ ������Ʈ�� �������� �ʴ´�.
		return nullptr;

	// ���콺�� ���̿� è�Ǿ� / ������ �� �浹 ó���� ���
	// �ش� è�Ǿ� / ������ �� ���콺�� ��� �ְ��Ѵ�.

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
