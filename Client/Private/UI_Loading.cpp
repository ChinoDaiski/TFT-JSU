#include "stdafx.h"
#include "..\Public\UI_Loading.h"
#include "GameInstance.h"

CUI_Loading::CUI_Loading(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CUI_Loading::CUI_Loading(const CUI_Loading & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_Loading::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Loading::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	Set_UI();

	return S_OK;
}

void CUI_Loading::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	for (auto& UI : m_vUI)
		UI.second->Tick(TimeDelta);
}

void CUI_Loading::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	// 우선 순위 대로 내림차순 정렬한다.
	// 숫자가 낮을수록 우선 순위가 높다. 
	sort(m_vUI.begin(), m_vUI.end(), [](const pair<const _tchar*, CUI*>& a, const pair<const _tchar*, CUI*>& b) {
		return a.second->Get_PriorityNumber() > b.second->Get_PriorityNumber();
	});

	// 우선 순위가 낮은 UI가 먼저 그려지고, 높은 UI가 나중에 그려져서 화면을 덮는다. 
	for (auto& UI : m_vUI) {
		UI.second->LateTick(TimeDelta);
	}
}

HRESULT CUI_Loading::Render()
{
	return S_OK;
}

HRESULT CUI_Loading::Set_UI()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUI::UIDesc Desc;
	ZeroMemory(&Desc, sizeof(CUI::UIDesc));

	Desc.eType = CUI::UT_NONHIDE;

	lstrcpy(Desc._tagTextureName, TEXT("Prototype_Component_Texture_UI_Main_Loading"));
	Desc.fX = g_iWinCX * 0.5f;
	Desc.fY = g_iWinCY * 0.5f;

	Desc.fSizeX = g_iWinCX;
	Desc.fSizeY = g_iWinCY;
	
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOADING, TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI"), &Desc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CUI_Loading * CUI_Loading::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CUI_Loading*	pInstance = new CUI_Loading(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Loading::Clone(void * pArg)
{
	CUI_Loading*	pInstance = new CUI_Loading(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading::Free()
{
	__super::Free();
}
