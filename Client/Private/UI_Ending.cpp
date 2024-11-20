#include "stdafx.h"
#include "..\Public\UI_Ending.h"
#include "GameInstance.h"

CUI_Ending::CUI_Ending(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CUI_Ending::CUI_Ending(const CUI_Ending & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_Ending::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Ending::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	Set_UI();

	return S_OK;
}

void CUI_Ending::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	for (auto& UI : m_vUI)
		UI.second->Tick(TimeDelta);
}

void CUI_Ending::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	// �켱 ���� ��� �������� �����Ѵ�.
	// ���ڰ� �������� �켱 ������ ����. 
	sort(m_vUI.begin(), m_vUI.end(), [](const pair<const _tchar*, CUI*>& a, const pair<const _tchar*, CUI*>& b) {
		return a.second->Get_PriorityNumber() > b.second->Get_PriorityNumber();
	});

	// �켱 ������ ���� UI�� ���� �׷�����, ���� UI�� ���߿� �׷����� ȭ���� ���´�. 
	for (auto& UI : m_vUI) {
		UI.second->LateTick(TimeDelta);
	}
}

HRESULT CUI_Ending::Render()
{
	return S_OK;
}

HRESULT CUI_Ending::Set_UI()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CUI::UIDesc Desc;
	ZeroMemory(&Desc, sizeof(CUI::UIDesc));

	lstrcpy(Desc._tagTextureName, TEXT("Prototype_Component_Texture_UI_Main_Background"));
	Desc.fX = g_iWinCX * 0.5f;
	Desc.fY = g_iWinCY * 0.5f;

	Desc.fSizeX = g_iWinCX;
	Desc.fSizeY = g_iWinCY;
	
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_ENDING, TEXT("Layer_UI"), TEXT("Prototype_GameObject_UI"), &Desc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CUI_Ending * CUI_Ending::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CUI_Ending*	pInstance = new CUI_Ending(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Ending::Clone(void * pArg)
{
	CUI_Ending*	pInstance = new CUI_Ending(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CBackGround"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Ending::Free()
{
	__super::Free();
}
