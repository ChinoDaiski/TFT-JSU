#include "stdafx.h"
#include "..\Public\Button.h"
#include "GameInstance.h"
#include "Player.h"

CButton::CButton(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CButton::CButton(const CButton & rhs)
	: CGameObject(rhs)
	, m_ProjMatrix(rhs.m_ProjMatrix)
	, m_eType(rhs.m_eType)
{
	Safe_AddRef(m_pRendererCom);
}

HRESULT CButton::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));
	
	return S_OK;
}

HRESULT CButton::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CButton::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 플레이어가 가운데 맵에 있을 경우 미니맵 버튼을 제외하고 무효 처리
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);
	if (nullptr == pLayer) {
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(*pLayer->Get_ObjectList().begin());

	// 플레이어가 맵의 가운데 있을 경우
	if (pPlayer->Get_CurrMapName() == CPersonalMap::MAP_NAME_END) {
		// 미니맵만 활성화
		if (m_eType == BT_MINIMAP) {
			CheckClick();
		}
		else {
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
	}
	// 플레이어가 맵의 가운데 없을 경우
	else {
		// 모든 버튼 활성화
		CheckClick();
	}

	RELEASE_INSTANCE(CGameInstance);

}

void CButton::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CButton::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CButton::Click(void)
{
}

void CButton::CheckClick(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	// 현재 누르고
	if (pGameInstance->Get_DIMCurButtonClick(CInput_Device::MOUSEBUTTONSTATE::DIMB_LBUTTON)) {
		// 이전에 누른적이 없을 경우
		if (!pGameInstance->Get_DIMPreButtonClick(CInput_Device::MOUSEBUTTONSTATE::DIMB_LBUTTON)) {
			if ((ptMouse.x > m_rtInfo.left) && (ptMouse.x < m_rtInfo.right) && (ptMouse.y > m_rtInfo.top) && (ptMouse.y < m_rtInfo.bottom))
				Click();
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CButton::SetUp_Components(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg)
{
	if (FAILED(__super::SetUp_Components(pComponentTag, iLevelIndex, pPrototypeTag, ppOut, pArg)))
		return E_FAIL;

	return S_OK;
}

CButton * CButton::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CButton*	pInstance = new CButton(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CButton::Clone(void * pArg)
{
	CButton*	pInstance = new CButton(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CButton::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
