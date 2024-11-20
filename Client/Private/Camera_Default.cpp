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
	// �ػ󵵿� �°� ��Ⱦ�� �����ش�.
	RECT rt;
	GetWindowRect(g_hWnd, &rt);
	Set_Aspect(rt);

	// Ŭ���̾�Ʈ â�� ���ο� ī�޶��� ���� �󿡼��� ��ġ�� �����ش�.
	ShowCameraPos();

	// ī�޶󿡰� ��ǥ�� �Ѱ��� ��� �ش� ��ǥ�� ���� �����δ�.
	Move(TimeDelta);

	// Ű(NumPAD)�� ������ ���
	PressNumpad();

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	Safe_AddRef(pGameInstance);

	// Ű�� ������ ��� �����̵��� �Ѵ�.
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
	// ī�޶� �������� �ʴ´ٸ�
	if (!m_bMove) {
		// ī�޶��� ���� ��ġ�� �� ��ġ�� �����Ѵ�.
		XMStoreFloat3(&m_Start_Eye, _StartEye);
		XMStoreFloat3(&m_Start_At, _StartAt);
		XMStoreFloat3(&m_Start_Up, _StartUp);
		XMStoreFloat3(&m_End_Eye, _EndEye);
		XMStoreFloat3(&m_End_At, _EndAt);
		XMStoreFloat3(&m_End_Up, _EndUp);

		// ������ �ð��� �����Ѵ�.
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

	//lstrcpy(szText, TEXT("�ε� �����Դϴ� : "));
	sprintf_s(szText, "x : %f, y : %f, z : %f", fPos.x, fPos.y, fPos.z);

	//wchar_t�� ���� ����
	wchar_t pStr[MAX_PATH];

	//��Ƽ ����Ʈ ũ�� ��� ���� ��ȯ
	int strSize = MultiByteToWideChar(CP_ACP, 0, szText, -1, NULL, NULL);

	//�� ��ȯ
	MultiByteToWideChar(CP_ACP, 0, szText, strlen(szText) + 1, pStr, strSize);

	SetWindowText(g_hWnd, pStr);

	//POINT ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);

	//char		szText[MAX_PATH];

	//sprintf_s(szText, "x : %d, y : %d", ptMouse.x, ptMouse.y);

	////wchar_t�� ���� ����
	//wchar_t pStr[MAX_PATH];

	////��Ƽ ����Ʈ ũ�� ��� ���� ��ȯ
	//int strSize = MultiByteToWideChar(CP_ACP, 0, szText, -1, NULL, NULL);

	////�� ��ȯ
	//MultiByteToWideChar(CP_ACP, 0, szText, strlen(szText) + 1, pStr, strSize);

	//SetWindowText(g_hWnd, pStr);
}

void CCamera_Default::Move(double deltaTime)
{
	// �������̴� ���̶��
	if (m_bMove) {

		// ������ �ð��� �Ⱓ�� �Ѿ��ٸ�
		if (m_dCurTime >= m_dEndTime) {
			// ����� ���� �����Ѵ�.
			m_dCurTime = 0.0;
			m_dEndTime = 0.0;
			m_bMove = false;
			
			return;
		}

		// �������� �ð��� �����ϰ�
		m_dCurTime += deltaTime;

		// lerp �Լ��� ����� ���� ��ġ�� ��� ���� ����Ͽ� ī�޶� �����Ų��.
		_double		Ratio = m_dCurTime / m_dEndTime;

		if (Ratio > 1.0)
			Ratio = 1.0;

		// eye, at ,up�� ���� �����Ͽ� ����� ���� ī�޶� �ִ´�.

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

	// ���е忡�� 1 ~ 9�������� ���� �����ٸ�
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

		// �߾Ӹ�, ���θ�, ī�޶� ���� ������ �����´�.
		CLayer* pLayerMapCenter = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_MAP_CENTER);
		CLayer* pLayerMapPersonal = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_MAP_PEROSNAL);
		CLayer* pLayerCamera = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_CAMERA);
		CLayer* pLayerPlayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);

		// ���� �ϳ��� ������ return;
		if (nullptr == pLayerMapCenter ||
			nullptr == pLayerMapPersonal ||
			nullptr == pLayerCamera ||
			nullptr == pLayerPlayer)
			return;

		// �÷��̾�
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(*pLayerPlayer->Get_ObjectList().begin());

		// ī�޶�
		_vector vDefault = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		_vector vCameraEye = XMVectorSet(0.25f, 6.0f, -2.0f, 0.f);
		_vector vCameraAt = XMVectorSet(0.25f, 1.5f, 0.8f, 0.f);

		auto& ObjectList = pLayerMapPersonal->Get_ObjectList();
		CGameObject* pMapObject = nullptr;
		CTransform* pTransform = nullptr;


		// ���е� 5���� ������ CenterMap�� ���̴� ��ġ�� �̵�
		if (pGameInstance->Get_DIKeyState(DIK_NUMPAD5)) {
			// �߾� ���� ã�´�.
			pMapObject = *pLayerMapCenter->Get_ObjectList().begin();

			pPlayer->MoveMap(CPersonalMap::MAP_NAME_END);
		}

		// ���е� 5�� ������ 1 ~ 9 ������ ���� ������ PersonalMap�� ���̴� ��ġ�� �̵�

		// ���� ��(MAP_NAME_Celebrationtomorrow)�� ã�´�.
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

		// ���� ��(MAP_NAME_Hexbattle)�� ã�´�.
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

		// ���� ��(MAP_NAME_Zaunbotanicalgarden)�� ã�´�.
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

		// ���� ��(MAP_NAME_Jinx)�� ã�´�.
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

		// ���� ��(MAP_NAME_Vi)�� ã�´�.
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
			

		// ���� ��ġ ���� �����´�.
		pTransform = dynamic_cast<CTransform*>(pMapObject->Get_Component(TEXT("Com_Transform")));
		pTransform->Get_State(CTransform::STATE_POSITION);

		// ī�޶� ���� �ٶ󺸰� �����Ǿ��ִ� ���� ������ ���� �� ī�޶� �־��ش�.
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
