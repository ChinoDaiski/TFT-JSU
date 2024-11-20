#include "stdafx.h"
#include "..\Public\UI_GamePlay.h"
#include "GameInstance.h"
#include "Button_Champion.h"

CUI_GamePlay::CUI_GamePlay(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CUI_GamePlay::CUI_GamePlay(const CUI_GamePlay & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_GamePlay::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_GamePlay::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	Set_UI();

	return S_OK;
}

void CUI_GamePlay::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	for (auto& UI : m_vUI)
		UI.second->Tick(TimeDelta);
}

void CUI_GamePlay::LateTick(_double TimeDelta)
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

HRESULT CUI_GamePlay::Render()
{
	return S_OK;
}

HRESULT CUI_GamePlay::Set_UI()
{
	if (FAILED(Set_UI_Round())) {
		MSG_BOX(TEXT("Failed to Set Gameplay_UI_Round"));
		return E_FAIL;
	}
	if (FAILED(Set_UI_MiniMap())) {
		MSG_BOX(TEXT("Failed to Set Gameplay_UI_MiniMap"));
		return E_FAIL;
	}
	if (FAILED(Set_UI_Market())) {
		MSG_BOX(TEXT("Failed to Set Gameplay_UI_Market"));
		return E_FAIL;
	}
	if (FAILED(Set_UI_Players())) {
		MSG_BOX(TEXT("Failed to Set Gameplay_UI_Players"));
		return E_FAIL;
	}
	if (FAILED(Set_UI_Synergy())) {
		MSG_BOX(TEXT("Failed to Set Gameplay_UI_Synergy"));
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_GamePlay::Set_UI_Round(void)
{
	return S_OK;
}

HRESULT CUI_GamePlay::Set_UI_MiniMap(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUI::UIDesc Desc;
	ZeroMemory(&Desc, sizeof(CUI::UIDesc));

	Desc.eType = CUI::UT_NONHIDE;

	// 미니맵
	lstrcpy(Desc._tagTextureName, TEXT("Prototype_Component_Texture_UI_InGame_Minimap"));

	_float fX = 10.f;
	_float fY = 6.f;

	Desc.fX = g_iWinCX * (1 - 1.f / (fX * 2));
	Desc.fY = g_iWinCY * (1 - 1.f / (fY * 2));

	Desc.fSizeX = g_iWinCX / fX + 6.f;
	Desc.fSizeY = g_iWinCY / fY + 6.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI"), &Desc)))
		return E_FAIL;


	// 미니맵 커버
	lstrcpy(Desc._tagTextureName, TEXT("Prototype_Component_Texture_UI_InGame_MinimapCover"));

	fX = 10.f;
	fY = 6.f;

	Desc.fX = g_iWinCX * (1 - 1.f / (fX * 2));
	Desc.fY = g_iWinCY * (1 - 1.f / (fY * 2));

	Desc.fSizeX = g_iWinCX / fX + 6.f;
	Desc.fSizeY = g_iWinCY / fY + 6.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI"), &Desc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUI_GamePlay::Set_UI_Market(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUI::UIDesc Desc;
	ZeroMemory(&Desc, sizeof(CUI::UIDesc));

	Desc.eType = CUI::UT_HIDE;

	// 기본
	lstrcpy(Desc._tagTextureName, TEXT("Prototype_Component_Texture_UI_Market_Base"));

	Desc.fX = g_iWinCX * 0.5f;
	Desc.fY = g_iWinCY * 0.92;

	Desc.fSizeX = g_iWinCX / 7.f * 5;
	Desc.fSizeY = g_iWinCY * 0.16;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI"), &Desc)))
		return E_FAIL;


	// 골드
	lstrcpy(Desc._tagTextureName, TEXT("Prototype_Component_Texture_UI_InGame_Market_Gold"));

	Desc.fX = g_iWinCX * 0.5f;
	Desc.fY = g_iWinCY * 0.88 - 44.f;

	Desc.fSizeX = g_iWinCX * 0.10f;
	Desc.fSizeY = g_iWinCY * 0.05f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI"), &Desc)))
		return E_FAIL;


	int iGap = 20;


	// 레벨업
	lstrcpy(Desc._tagTextureName, TEXT("Prototype_Component_Texture_UI_InGame_Market_LevelUp"));
	RECT rt;
	rt.left = 186;
	rt.right = 339;
	rt.top = 627 - iGap;
	rt.bottom = 683 - iGap;
	
	Desc.fX = (rt.left + rt.right) * 0.5f;
	Desc.fY = (rt.top + rt.bottom) * 0.5f;

	Desc.fSizeX = rt.right - rt.left;
	Desc.fSizeY = rt.bottom - rt.top;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI"), &Desc)))
		return E_FAIL;



	// XP(경험치)구매
	lstrcpy(Desc._tagTextureName, TEXT("Prototype_Component_Texture_UI_InGame_Market_Reroll"));
	rt.left = 186;
	rt.right = 339;
	rt.top = 683 - iGap;
	rt.bottom = 740 - iGap;

	Desc.fX = (rt.left + rt.right) * 0.5f;
	Desc.fY = (rt.top + rt.bottom) * 0.5f;

	Desc.fSizeX = rt.right - rt.left;
	Desc.fSizeY = rt.bottom - rt.top;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI"), &Desc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUI_GamePlay::Set_UI_Players(void)
{
	return S_OK;
}

HRESULT CUI_GamePlay::Set_UI_Synergy(void)
{
	return S_OK;
}

CUI_GamePlay * CUI_GamePlay::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CUI_GamePlay*	pInstance = new CUI_GamePlay(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_GamePlay::Clone(void * pArg)
{
	CUI_GamePlay*	pInstance = new CUI_GamePlay(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_GamePlay::Free()
{
	__super::Free();
}
