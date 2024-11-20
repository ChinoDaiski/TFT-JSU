#include "stdafx.h"
#include "..\Public\Tile.h"
#include "GameInstance.h"

CTile::CTile(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CTile::CTile(const CTile & rhs)
	: CGameObject(rhs)
	, m_eType(rhs.m_eType)
	, m_isCollision(false)
{
}

HRESULT CTile::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CTile::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	TILEDesc TileDesc;
	ZeroMemory(&TileDesc, sizeof(TILEDesc));
	if (nullptr == pArg)
		return E_FAIL;
	
	TileDesc = *((TILEDesc*)pArg);
	
	// Ÿ���� Ÿ�� ����
	m_eType = TileDesc.eType;

	// ���� Ÿ�� x������ 90�� ȸ��
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 1.f), XMConvertToRadians(90.f));

	// ���� ����
	m_pTransformCom->Scaled(_float3(TileDesc.vScale.x, TileDesc.vScale.y, 1.f));

	// ��ġ�� ����
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&TileDesc.vPos), 1.f));

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CTile::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if (nullptr != m_pChamp)
		m_pChamp->Tick(TimeDelta);

	// �ʵ忡 �ִ� Ÿ���� ���
	if (m_eType == TILE_FIELD) {

	}
	// ��⼮���� ��ٸ��� Ÿ���� ���
	else if (m_eType == TILE_LOUNGE) {

	}

	// �浹ü�� ������ ��
	if (nullptr != m_pAABBCom) {
		// �浹�� �Ǿ��ٸ�
		if (m_pAABBCom->isCollision())
			// ������ ���� ����
			Change_CurrEffectFrame(0.1, TimeDelta);
		// �浹�� ���� �ʾҴٸ�
		else {
			// ������ �ʱ�ȭ
			m_iCurrEffectFrame = 0;
		}
	}

	m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CTile::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pChamp)
		m_pChamp->LateTick(TimeDelta);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom &&
		true == pGameInstance->isInFrustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_ALPHABLEND, this);

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CTile::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	// �ʵ忡 �ִ� Ÿ���� ���
	if (m_eType == TILE_FIELD) {
		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;
	}
	// ��⼮���� ��ٸ��� Ÿ���� ���
	else if (m_eType == TILE_LOUNGE) {
		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	// �浹ü�� �浹�� �Ǿ��ٸ�
	if (m_isCollision) {

		// �ؽ��İ� ������ ���
		if (nullptr != m_pEffectTextureCom) {
			// �ؽ��� ����
			if (FAILED(m_pEffectTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", m_iCurrEffectFrame)))
				return E_FAIL;

			// ����Ʈ �н� ����
			if (FAILED(m_pShaderCom->Begin(6)))
				return E_FAIL;

			// ����
			if (FAILED(m_pVIBufferCom->Render()))
				return E_FAIL;
		}
	}


#ifdef _DEBUG
	m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CTile::SetUp_Components()
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

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	// �ʵ忡 �ִ� Ÿ���� ���
	if (m_eType == TILE_FIELD) {
		/* For.Com_Texture */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_Map"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;

		ColliderDesc.vPosition = _float3(0.f, 0.05f, 0.f);
		ColliderDesc.vSize = _float3(0.75f, 0.1f, 0.23f);

		if (FAILED(__super::SetUp_Components(TEXT("Com_EffectTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Tile"), (CComponent**)&m_pEffectTextureCom)))
			return E_FAIL;
	}

	// ��⼮���� ��ٸ��� Ÿ���� ���
	else if (m_eType == TILE_LOUNGE) {
		/* For.Com_Texture */
		if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_Lounge"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;

		ColliderDesc.vPosition = _float3(0.f, 0.05f, 0.f);
		ColliderDesc.vSize = _float3(0.9f, 0.1f, 0.2f);

		m_pEffectTextureCom = nullptr;
	}

	/* For.Com_AABB */
	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTile::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CTile * CTile::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CTile*	pInstance = new CTile(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTile::Clone(void * pArg)
{
	CTile*	pInstance = new CTile(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTile::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}

void CTile::Set_ChampionInfo(CChampion * pChamp)
{ 
	// Ÿ���� ������ �ִ� è�Ǿ� ������ ���ڷ� ���� è�Ǿ� ������ �ִ´�.
	m_pChamp = pChamp;

	// ���� è�Ǿ��� ���̰� �Ѵ�.
	m_pChamp->Set_Show(true);
	dynamic_cast<CModel*>(m_pChamp->Get_Component(TEXT("Com_Model")))->Set_AnimationIndex((int)CChampion::CS_IDLE);

	// Ÿ���� ��ġ�� �°� è�Ǿ��� ��ġ�Ѵ�.
	dynamic_cast<CTransform*>(m_pChamp->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
}

void CTile::Change_CurrEffectFrame(_double dFrame, _double DeltaTime)
{
	m_dwEffectTime += DeltaTime;

	if (m_dwEffectTime > dFrame) {
		++m_iCurrEffectFrame;

		// 16�� �Ǹ� �ٽ� 0���� ���ư���. 0 ~ 15 ������ ���� ����
		m_iCurrEffectFrame %= 16;

		m_dwEffectTime -= dFrame;
	}
}
