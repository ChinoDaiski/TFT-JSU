#include "stdafx.h"
#include "..\Public\Button_Reroll.h"
#include "GameInstance.h"
#include "Camera_Default.h"

CButton_Reroll::CButton_Reroll(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CButton(pDeviceOut, pDeviceContextOut)
{
}

CButton_Reroll::CButton_Reroll(const CButton_Reroll & rhs)
	: CButton(rhs)
	, m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CButton_Reroll::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));
	
	return S_OK;
}

HRESULT CButton_Reroll::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr == pArg) {
		MSG_BOX(TEXT("No Rect Info Button-Nativeconstruct"));
		return E_FAIL;
	}

	
	BtMiniMapDesc btDesc = *((BtMiniMapDesc*)pArg);
	m_eType = btDesc.eType;	// m_eType = BUTTON_TYPE::BT_MINIMAP;
	m_rtInfo = btDesc.rtButtonInfo;

	m_fSizeX = m_rtInfo.right - m_rtInfo.left;
	m_fSizeY = m_rtInfo.bottom - m_rtInfo.top;
	m_fX = (m_rtInfo.left + m_rtInfo.right) * 0.5f;
	m_fY = (m_rtInfo.top + m_rtInfo.bottom) * 0.5f;

	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f, 1.f));

	m_vEye = btDesc.vEye;
	m_vAt = btDesc.vAt;
	m_vUp = btDesc.vUp;

	return S_OK;
}

void CButton_Reroll::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

}

void CButton_Reroll::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	//if (nullptr != m_pRendererCom)
	//	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CButton_Reroll::Render()
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

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;
	
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CButton_Reroll::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Button"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CButton_Reroll::Click(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_CAMERA);

	if (nullptr == pLayer)
		return;

	dynamic_cast<CCamera_Default*>(*pLayer->Get_ObjectList().begin())->SetPos(
		XMLoadFloat3(&m_vEye),
		XMLoadFloat3(&m_vAt),
		XMLoadFloat3(&m_vUp)
	);
}

CButton_Reroll * CButton_Reroll::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CButton_Reroll*	pInstance = new CButton_Reroll(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CButton_Reroll"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CButton_Reroll::Clone(void * pArg)
{
	CButton_Reroll*	pInstance = new CButton_Reroll(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CButton_Reroll"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton_Reroll::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
