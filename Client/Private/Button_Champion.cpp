#include "stdafx.h"
#include "..\Public\Button_Champion.h"
#include "GameInstance.h"
#include "Camera_Default.h"
#include "Champion.h"
#include "Player.h"

CButton_Champion::CButton_Champion(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CButton(pDeviceOut, pDeviceContextOut)
{
}

CButton_Champion::CButton_Champion(const CButton_Champion & rhs)
	: CButton(rhs)
	, m_ProjMatrix(rhs.m_ProjMatrix)
	, m_bActivated(rhs.m_bActivated)
{
	Safe_AddRef(m_pRendererCom);
}

HRESULT CButton_Champion::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));
	
	return S_OK;
}

HRESULT CButton_Champion::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (nullptr == pArg) {
		MSG_BOX(TEXT("No Rect Info Button-Nativeconstruct"));
		return E_FAIL;
	}
	
	BtChampBorderDesc btDesc = *((BtChampBorderDesc*)pArg);
	m_eType = btDesc.eType;	// m_eType = BUTTON_TYPE::BT_CHAMPION;
	m_eName = btDesc.eName;
	m_iLevel = btDesc.iLevel;

	m_fSizeX = 0;
	m_fSizeY = 0;
	m_fX = 0;
	m_fY = 0;

	//m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f, 0.f, 1.f));

	return S_OK;
}

void CButton_Champion::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// Ȱ��ȭ ���̸�
	if (m_bActivated) {
		// ���� �߰��Ѵ�.
	}
}

void CButton_Champion::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (m_bActivated)
		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_UI, this);
}

HRESULT CButton_Champion::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	// �÷��̾��� ������ �����ͼ� �÷��̾ �� ��� �ִٸ� �׷����� �ʰ��Ѵ�.
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);
	
	if (nullptr == pLayer) {
		RELEASE_INSTANCE(CGameInstance);
	}
	else {
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(*pLayer->Get_ObjectList().begin());

		if (CPersonalMap::MAP_NAME_END == pPlayer->Get_CurrMapName()) {
			RELEASE_INSTANCE(CGameInstance);
			return S_OK;
		}
	}

	m_rtInfo.bottom = 715;
	m_fSizeY = m_rtInfo.bottom - m_rtInfo.top;
	m_fY = (m_rtInfo.top + m_rtInfo.bottom) * 0.5f;
	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	_uint iGap = 15.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f + iGap, 0.f, 1.f));

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

	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", (int)m_eName)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;







	m_rtInfo.bottom = 740;
	m_fSizeY = m_rtInfo.bottom - m_rtInfo.top;
	m_fY = (m_rtInfo.top + m_rtInfo.bottom) * 0.5f;
	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	iGap = 15.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f + iGap, 0.f, 1.f));

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	XMStoreFloat4x4(&ProjMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &ProjMatrixTP, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureComBorder->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", m_iLevel - 1)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

CChampion * CButton_Champion::Clone_Champion(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CChampion* pClonedChamp = nullptr;

	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_CHAMPION);

	// ������Ʈ�߿��� ���ϴ� �̸��� ���� è�Ǿ��� ã�´�.
	if (nullptr != pLayer) {
		// è�Ǿ� ���̾ ���� è�Ǿ� ����� �����´�.
		auto& ObjectList = pLayer->Get_ObjectList();

		// ��ư�� ������ �ִ� ������ è�Ǿ� ������Ʈ�� ã�´�.
		auto& iter = find_if(ObjectList.begin(), ObjectList.end(), [&](CGameObject* Champ) {
			return dynamic_cast<CChampion*>(Champ)->Get_ChampionDesc().ChampName == m_eName;
		});

		// ã�� ������Ʈ�� ���ٸ� �׳� �ѱ��.
		if (iter == ObjectList.end())
			return nullptr;

		// ������Ʈ�� ã�Ҵٸ� �����Ѵ�.

		// ������ ���� �ش� ������Ʈ�� ������ �����´�.
		CChampion::CHAMPDESC ChampDesc = dynamic_cast<CChampion*>(*iter)->Get_ChampionDesc();

		// ������Ʈ ������ ����Ͽ� è�Ǿ��� �����Ѵ�.
		pClonedChamp = dynamic_cast<CChampion*>((*iter)->Clone(&ChampDesc));
	}

	RELEASE_INSTANCE(CGameInstance);

	// ������ è�Ǿ��� ������ ��ȯ�Ѵ�.
	return pClonedChamp;
}

void CButton_Champion::Check_EnoughMoney(_uint iMoney)
{
	//// ���� ���ڷ� ���� �÷��̾��� ���� ���� �ش� è�Ǿ��� ��� ���� �ʿ��� ������ �۴ٸ�
	//if (iMoney < m_iLevel)
	//	// ��Ȱ��ȭ
	//	m_bActivated = false;
}

void CButton_Champion::Set_Index(_uint iIndex)
{
	switch (iIndex + 1)
	{
	case 1:
		m_rtInfo.left = 340;
		m_rtInfo.right = 490;
		break;
	case 2:
		m_rtInfo.left = 490;
		m_rtInfo.right = 640;
		break;
	case 3:
		m_rtInfo.left = 640;
		m_rtInfo.right = 790;
		break;
	case 4:
		m_rtInfo.left = 790;
		m_rtInfo.right = 940;
		break;
	case 5:
		m_rtInfo.left = 940;
		m_rtInfo.right = 1090;
		break;
	}
	m_rtInfo.top = 627;
	m_rtInfo.bottom = 740;	
	
	m_fSizeX = m_rtInfo.right - m_rtInfo.left;
	m_fSizeY = m_rtInfo.bottom - m_rtInfo.top;
	m_fX = (m_rtInfo.left + m_rtInfo.right) * 0.5f;
	m_fY = (m_rtInfo.top + m_rtInfo.bottom) * 0.5f;

	m_pTransformCom->Scaled(_float3(m_fSizeX, m_fSizeY, 1.f));
	_uint iGap = 15.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinCX * 0.5f, -m_fY + g_iWinCY * 0.5f + iGap, 0.f, 1.f));

	m_bActivated = true;
}

HRESULT CButton_Champion::SetUp_Components()
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
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ChampionBorders"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ChampionOutline"), (CComponent**)&m_pTextureComBorder)))
		return E_FAIL;
	
	return S_OK;
}

void CButton_Champion::Click(void)
{
	__super::Click();

	m_bClick = true;
}

CButton_Champion * CButton_Champion::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CButton_Champion*	pInstance = new CButton_Champion(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CButton_Champion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CButton_Champion::Clone(void * pArg)
{
	CButton_Champion*	pInstance = new CButton_Champion(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CButton_Champion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton_Champion::Free()
{
	__super::Free();

}
