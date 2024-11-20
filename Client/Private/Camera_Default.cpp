#include "stdafx.h"
#include "..\Public\Camera_Default.h"
#include "GameInstance.h"

#include "CenterMap.h"
#include "PersonalMap.h"

#include "Layer.h"
#include "Player.h"

CCamera_Default::CCamera_Default(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CCamera(pDeviceOut, pDeviceContextOut)
{
}

CCamera_Default::CCamera_Default(const CCamera_Default & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Default::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Default::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 3.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(20.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

void CCamera_Default::Tick(_double TimeDelta)
{
	// 해상도에 맞게 종횡비를 맞춰준다.
	RECT rt;
	GetWindowRect(g_hWnd, &rt);
	Set_Aspect(rt);

	// 클라이언트 창의 라인에 카메라의 월드 상에서의 위치를 보여준다.
	ShowCameraPos();

	// 카메라에게 좌표를 넘겨줄 경우 해당 좌표를 따라 움직인다.
	Move(TimeDelta);

	// 키(NumPAD)를 눌렀을 경우
	PressNumpad();

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	// 키를 눌렀을 경우 움직이도록 한다.
	if (pGameInstance->Get_DIKeyState(DIK_W))
	{
		m_pTransformCom->Go_Straight(TimeDelta);
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_pTransformCom->Go_BackWard(TimeDelta);
	}
	if (GetKeyState('A') & 0x8000)
	{
		m_pTransformCom->Go_Left(TimeDelta);
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_pTransformCom->Go_Right(TimeDelta);
	}


	
	
	/*_long		MouseMove;

	if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_X))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * 0.1f);
	}

	if (MouseMove = pGameInstance->Get_DIMMoveState(CInput_Device::DIMM_Y))
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove* 0.1f);
	}*/

	__super::Tick(TimeDelta);

	Safe_Release(pGameInstance);
}

void CCamera_Default::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);
}

HRESULT CCamera_Default::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Default::SetMove(_fvector _StartEye, _fvector _StartAt, _fvector _StartUp, GXMVECTOR _EndEye, HXMVECTOR _EndAt, HXMVECTOR _EndUp, double duration)
{
	// 카메라가 움직이지 않는다면
	if (!m_bMove) {
		// 카메라의 시작 위치와 끝 위치를 설정한다.
		XMStoreFloat3(&m_Start_Eye, _StartEye);
		XMStoreFloat3(&m_Start_At, _StartAt);
		XMStoreFloat3(&m_Start_Up, _StartUp);
		XMStoreFloat3(&m_End_Eye, _EndEye);
		XMStoreFloat3(&m_End_At, _EndAt);
		XMStoreFloat3(&m_End_Up, _EndUp);

		// 움직일 시간도 세팅한다.
		m_dCurTime = 0.0;
		m_dEndTime = duration;

		m_bMove = true;
	}
}

void CCamera_Default::SetPos(_fvector vEye, _fvector vAt, _fvector vUpAxis)
{
	_vector		vLook = vAt - vEye;
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vRight = XMVector3Cross(vUpAxis, vLook);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector		vUp = XMVector3Cross(vLook, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	_vector		vPosition = XMVectorSetW(vEye, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
}

void CCamera_Default::ShowCameraPos(void)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_float3 fPos;
	XMStoreFloat3(&fPos, vPos);

	char		szText[MAX_PATH];

	//lstrcpy(szText, TEXT("로딩 레벨입니다 : "));
	sprintf_s(szText, "x : %f, y : %f, z : %f", fPos.x, fPos.y, fPos.z);

	//wchar_t형 변수 선언
	wchar_t pStr[MAX_PATH];

	//멀티 바이트 크기 계산 길이 반환
	int strSize = MultiByteToWideChar(CP_ACP, 0, szText, -1, NULL, NULL);

	//형 변환
	MultiByteToWideChar(CP_ACP, 0, szText, strlen(szText) + 1, pStr, strSize);

	SetWindowText(g_hWnd, pStr);

	//POINT ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);

	//char		szText[MAX_PATH];

	//sprintf_s(szText, "x : %d, y : %d", ptMouse.x, ptMouse.y);

	////wchar_t형 변수 선언
	//wchar_t pStr[MAX_PATH];

	////멀티 바이트 크기 계산 길이 반환
	//int strSize = MultiByteToWideChar(CP_ACP, 0, szText, -1, NULL, NULL);

	////형 변환
	//MultiByteToWideChar(CP_ACP, 0, szText, strlen(szText) + 1, pStr, strSize);

	//SetWindowText(g_hWnd, pStr);
}

void CCamera_Default::Move(double deltaTime)
{
	// 움직임이는 중이라면
	if (m_bMove) {

		// 움직인 시간이 기간을 넘었다면
		if (m_dCurTime >= m_dEndTime) {
			// 사용한 값을 정리한다.
			m_dCurTime = 0.0;
			m_dEndTime = 0.0;
			m_bMove = false;
			
			return;
		}

		// 움직였던 시간을 갱신하고
		m_dCurTime += deltaTime;

		// lerp 함수를 사용해 현재 위치한 행렬 값을 계산하여 카메라에 적용시킨다.
		_double		Ratio = m_dCurTime / m_dEndTime;

		if (Ratio > 1.0)
			Ratio = 1.0;

		// eye, at ,up을 선형 보간하여 행렬을 구해 카메라에 넣는다.

		_vector vStartEve = XMLoadFloat3(&m_Start_Eye);
		_vector vStartAt = XMLoadFloat3(&m_Start_At);
		_vector vStartUp = XMLoadFloat3(&m_Start_Up);

		_vector vEndEye = XMLoadFloat3(&m_End_Eye);
		_vector vEndAt = XMLoadFloat3(&m_End_At);
		_vector vEndUp = XMLoadFloat3(&m_End_Up);
		
		_vector		vEye = XMVectorLerp(vStartEve, vEndEye, Ratio);
		_vector		vAt = XMVectorLerp(vStartAt, vEndAt, Ratio);
		_vector		vUpAxis = XMVectorLerp(vStartUp, vEndUp, Ratio);

		_vector		vLook = vAt - vEye;
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

		_vector		vRight = XMVector3Cross(vUpAxis, vLook);
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

		_vector		vUp = XMVector3Cross(vLook, vRight);
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

		_vector		vPosition = XMVectorSetW(vEye, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}
}

void CCamera_Default::PressNumpad(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 넘패드에서 1 ~ 9번사이의 값을 눌렀다면
	if (
		pGameInstance->Get_DIKeyState(DIK_NUMPAD1) ||
		pGameInstance->Get_DIKeyState(DIK_NUMPAD2) ||
		pGameInstance->Get_DIKeyState(DIK_NUMPAD3) ||
		pGameInstance->Get_DIKeyState(DIK_NUMPAD4) ||
		pGameInstance->Get_DIKeyState(DIK_NUMPAD5) ||
		pGameInstance->Get_DIKeyState(DIK_NUMPAD6) ||
		pGameInstance->Get_DIKeyState(DIK_NUMPAD7) ||
		pGameInstance->Get_DIKeyState(DIK_NUMPAD8) ||
		pGameInstance->Get_DIKeyState(DIK_NUMPAD9)
		) {

		// 중앙맵, 개인맵, 카메라에 대한 정보를 가져온다.
		CLayer* pLayerMapCenter = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_MAP_CENTER);
		CLayer* pLayerMapPersonal = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_MAP_PEROSNAL);
		CLayer* pLayerCamera = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_CAMERA);
		CLayer* pLayerPlayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);

		// 셋중 하나라도 없을시 return;
		if (nullptr == pLayerMapCenter ||
			nullptr == pLayerMapPersonal ||
			nullptr == pLayerCamera ||
			nullptr == pLayerPlayer)
			return;

		// 플레이어
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(*pLayerPlayer->Get_ObjectList().begin());

		// 카메라
		_vector vDefault = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		_vector vCameraEye = XMVectorSet(0.25f, 6.0f, -2.0f, 0.f);
		_vector vCameraAt = XMVectorSet(0.25f, 1.5f, 0.8f, 0.f);

		auto& ObjectList = pLayerMapPersonal->Get_ObjectList();
		CGameObject* pMapObject = nullptr;
		CTransform* pTransform = nullptr;


		// 넘패드 5번을 누르면 CenterMap이 보이는 위치로 이동
		if (pGameInstance->Get_DIKeyState(DIK_NUMPAD5)) {
			// 중앙 맵을 찾는다.
			pMapObject = *pLayerMapCenter->Get_ObjectList().begin();

			pPlayer->MoveMap(CPersonalMap::MAP_NAME_END);
		}

		// 넘패드 5를 제외한 1 ~ 9 사이의 값을 누르면 PersonalMap이 보이는 위치로 이동

		// 개인 맵(MAP_NAME_Celebrationtomorrow)을 찾는다.
		if (pGameInstance->Get_DIKeyState(DIK_NUMPAD1)) {
			auto& iter = ObjectList.begin();
			for (; iter != ObjectList.end();) {
				CPersonalMap::MAP_NAME eName = dynamic_cast<CPersonalMap*>(*iter)->Get_Type();
				if (eName == CPersonalMap::MAP_NAME_Celebrationtomorrow)
					break;
				else
					++iter;
			}
			pMapObject = *iter;

			vCameraEye += XMVectorSet(0.f, -3.f, -0.2f, 0.f);
			vCameraAt += XMVectorSet(0.f, -5.5f, -0.2f, 0.f);

			pPlayer->MoveMap(CPersonalMap::MAP_NAME_Celebrationtomorrow);
		}

		// 개인 맵(MAP_NAME_Hexbattle)을 찾는다.
		if (pGameInstance->Get_DIKeyState(DIK_NUMPAD2)) {
			auto& iter = ObjectList.begin();
			for (; iter != ObjectList.end();) {
				CPersonalMap::MAP_NAME eName = dynamic_cast<CPersonalMap*>(*iter)->Get_Type();
				if (eName == CPersonalMap::MAP_NAME_Hexbattle)
					break;
				else
					++iter;
			}
			pMapObject = *iter;

			vCameraEye += XMVectorSet(-0.3f, -3.9f, 0.f, 0.f);
			vCameraAt += XMVectorSet(-0.3f, -5.2f, 0.f, 0.f);

			pPlayer->MoveMap(CPersonalMap::MAP_NAME_Hexbattle);
		}

		// 개인 맵(MAP_NAME_Zaunbotanicalgarden)을 찾는다.
		if (pGameInstance->Get_DIKeyState(DIK_NUMPAD3)) {
			auto& iter = ObjectList.begin();
			for (; iter != ObjectList.end();) {
				CPersonalMap::MAP_NAME eName = dynamic_cast<CPersonalMap*>(*iter)->Get_Type();
				if (eName == CPersonalMap::MAP_NAME_ZaunCity)
					break;
				else
					++iter;
			}
			pMapObject = *iter;

			vCameraEye += XMVectorSet(-0.31f, -1.7f, -0.27f, 0.f);
			vCameraAt += XMVectorSet(-0.31f, -2.8f, -0.27f, 0.f);

			pPlayer->MoveMap(CPersonalMap::MAP_NAME_ZaunCity);
		}

		// 개인 맵(MAP_NAME_Jinx)을 찾는다.
		if (pGameInstance->Get_DIKeyState(DIK_NUMPAD4)) {
			auto& iter = ObjectList.begin();
			for (; iter != ObjectList.end();) {
				CPersonalMap::MAP_NAME eName = dynamic_cast<CPersonalMap*>(*iter)->Get_Type();
				if (eName == CPersonalMap::MAP_NAME_Jinx)
					break;
				else
					++iter;
			}
			pMapObject = *iter;

			vCameraEye += XMVectorSet(-0.95f, -3.5f, -0.4f, 0.f);
			vCameraAt += XMVectorSet(-0.95f, -6.f, -0.4f, 0.f);

			pPlayer->MoveMap(CPersonalMap::MAP_NAME_Jinx);
		}

		// 개인 맵(MAP_NAME_Vi)을 찾는다.
		if (pGameInstance->Get_DIKeyState(DIK_NUMPAD6)) {
			auto& iter = ObjectList.begin();
			for (; iter != ObjectList.end();) {
				CPersonalMap::MAP_NAME eName = dynamic_cast<CPersonalMap*>(*iter)->Get_Type();
				if (eName == CPersonalMap::MAP_NAME_Vi)
					break;
				else
					++iter;
			}
			pMapObject = *iter;

			vCameraEye += XMVectorSet(0.f, -3.f, -0.2f, 0.f);
			vCameraAt += XMVectorSet(0.f, -5.5f, -0.2f, 0.f);

			pPlayer->MoveMap(CPersonalMap::MAP_NAME_Vi);
		}

		if (pGameInstance->Get_DIKeyState(DIK_NUMPAD7)) {
			auto& iter = ObjectList.begin();
			for (; iter != ObjectList.end();) {
				CPersonalMap::MAP_NAME eName = dynamic_cast<CPersonalMap*>(*iter)->Get_Type();
				if (eName == CPersonalMap::MAP_NAME_Spiritblossom_kami)
					break;
				else
					++iter;
			}
			pMapObject = *iter;
			
			vCameraEye += XMVectorSet(0.f, -2.8f, 0.2f, 0.f);
			vCameraAt += XMVectorSet(0.f, -5.f, 0.2f, 0.f);

			pPlayer->MoveMap(CPersonalMap::MAP_NAME_Spiritblossom_kami);
		}

		if (pGameInstance->Get_DIKeyState(DIK_NUMPAD8)) {
			auto& iter = ObjectList.begin();
			for (; iter != ObjectList.end();) {
				CPersonalMap::MAP_NAME eName = dynamic_cast<CPersonalMap*>(*iter)->Get_Type();
				if (eName == CPersonalMap::MAP_NAME_Spiritblossom_reality)
					break;
				else
					++iter;
			}
			pMapObject = *iter;

			vCameraEye += XMVectorSet(-0.5f, -3.f, 0.3f, 0.f);
			vCameraAt += XMVectorSet(-0.5f, -4.f, 0.3f, 0.f);

			pPlayer->MoveMap(CPersonalMap::MAP_NAME_Spiritblossom_reality);
		}

		if (pGameInstance->Get_DIKeyState(DIK_NUMPAD9)) {
			auto& iter = ObjectList.begin();
			for (; iter != ObjectList.end();) {
				CPersonalMap::MAP_NAME eName = dynamic_cast<CPersonalMap*>(*iter)->Get_Type();
				if (eName == CPersonalMap::MAP_NAME_Spiritblossom_yokai)
					break;
				else
					++iter;
			}
			pMapObject = *iter;

			vCameraEye += XMVectorSet(-0.5f, -3.2f, 0.f, 0.f);
			vCameraAt += XMVectorSet(-0.5f, -4.5f, 0.f, 0.f);

			pPlayer->MoveMap(CPersonalMap::MAP_NAME_Spiritblossom_yokai);
		}

		if (nullptr == pMapObject)
			return;
			

		// 맵의 위치 값을 가져온다.
		pTransform = dynamic_cast<CTransform*>(pMapObject->Get_Component(TEXT("Com_Transform")));
		pTransform->Get_State(CTransform::STATE_POSITION);

		// 카메라가 맵을 바라보게 설정되어있는 값을 가져와 더한 후 카메라에 넣어준다.
		SetPos(XMLoadFloat3((_float3*)&pTransform->Get_State(CTransform::STATE_POSITION)) + vCameraEye,
			XMLoadFloat3((_float3*)&pTransform->Get_State(CTransform::STATE_POSITION)) + vCameraAt,
			XMVectorSet(0.f, 1.f, 0.f, 0.f));
	}

	RELEASE_INSTANCE(CGameInstance);
}

CCamera_Default * CCamera_Default::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CCamera_Default*	pInstance = new CCamera_Default(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Default::Clone(void * pArg)
{
	CCamera_Default*	pInstance = new CCamera_Default(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CCamera_Default"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Default::Free()
{
	__super::Free();

}
