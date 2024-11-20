#include "stdafx.h"
#include "..\Public\PickingObject.h"
#include "GameInstance.h"

CPickingObject::CPickingObject(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CPickingObject::CPickingObject(const CPickingObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPickingObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CPickingObject::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	PICKINGDESC PickingDesc;
	ZeroMemory(&PickingDesc, sizeof(PICKINGDESC));
	if (nullptr == pArg)
		return E_FAIL;
	
	PickingDesc = *((PICKINGDESC*)pArg);

	// 세로 타일 x축으로 90도 회전
	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 1.f), XMConvertToRadians(90.f));

	// 길이 설정
	m_pTransformCom->Scaled(_float3(PickingDesc.fSize.x, PickingDesc.fSize.x, 1.f));

	// 위치값 설정
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&PickingDesc.vPos), 1.f));

	// 중점과 가로 세로 길이로 만들어진 점 4개의 정보 저장
	// 0 : 좌상단, 1 : 우상단, 2 : 우하단, 3 : 좌하단
	m_RectPos[0] = _float3(
		PickingDesc.vPos.x - PickingDesc.fSize.x * 0.5,
		PickingDesc.vPos.y,
		PickingDesc.vPos.z + PickingDesc.fSize.y * 0.5
	);
	m_RectPos[1] = _float3(
		PickingDesc.vPos.x + PickingDesc.fSize.x * 0.5,
		PickingDesc.vPos.y,
		PickingDesc.vPos.z + PickingDesc.fSize.y * 0.5
	);
	m_RectPos[2] = _float3(
		PickingDesc.vPos.x + PickingDesc.fSize.x * 0.5,
		PickingDesc.vPos.y,
		PickingDesc.vPos.z - PickingDesc.fSize.y * 0.5
	);
	m_RectPos[3] = _float3(
		PickingDesc.vPos.x - PickingDesc.fSize.x * 0.5,
		PickingDesc.vPos.y,
		PickingDesc.vPos.z - PickingDesc.fSize.y * 0.5
	);

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

void CPickingObject::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CPickingObject::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);
}

HRESULT CPickingObject::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	// 필드에 있는 타일일 경우
	if (FAILED(m_pShaderCom->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPickingObject::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_Map"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPickingObject::SetUp_ConstantTable()
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

CPickingObject * CPickingObject::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CPickingObject*	pInstance = new CPickingObject(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPickingObject::Clone(void * pArg)
{
	CPickingObject*	pInstance = new CPickingObject(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPickingObject::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
