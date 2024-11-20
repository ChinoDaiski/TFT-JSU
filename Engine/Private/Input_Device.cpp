#include "..\Public\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{
}

HRESULT CInput_Device::InitDevice(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->Acquire();


	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();

	return S_OK;
}

void CInput_Device::SetUp_InputDeviceState()
{
	if (nullptr == m_pKeyBoard ||
		nullptr == m_pMouse)
		return;

	// m_MouseCurState에 있는 정보를 m_MousePreState에 갱신
	memcpy(&m_MousePreState, &m_MouseCurState, sizeof(DIMOUSESTATE));
	
	// m_KeyBoardCurState에 있는 정보를 m_KeyBoardPreState에 갱신
	memcpy(m_KeyBoardPreState, m_KeyBoardCurState, sizeof(_char) * 256);

	// 현재 키보드 정보 갱신
	m_pKeyBoard->GetDeviceState(256, m_KeyBoardCurState);

	m_pMouse->GetDeviceState(sizeof(m_MouseCurState), &m_MouseCurState);
}

void CInput_Device::Free()
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);

	Safe_Release(m_pInputSDK);
}
