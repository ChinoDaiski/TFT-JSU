#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Default.h"

#include "PersonalMap.h"
#include "UI_GamePlay.h"

#include "Button.h"
#include "Button_MiniMap.h"
#include "Button_Champion.h"
#include "Button_LockUnlock.h"
#include "Button_LevelUp.h"
#include "Button_Reroll.h"

#include "MapObject.h"

#include "Tile.h"
#include "Player.h"

#include "PickingObject.h"

#include "Mouse.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{

}

HRESULT CLevel_GamePlay::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct()))
		return E_FAIL;	

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(LAYER_GAMEPLAY_CAMERA)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(LAYER_GAMEPLAY_PLAYER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Champion(LAYER_GAMEPLAY_CHAMPION)))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_CenterMap(LAYER_GAMEPLAY_MAP_CENTER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_PersonalMap(LAYER_GAMEPLAY_MAP_PEROSNAL)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Hexacore(LAYER_GAMEPLAY_HEXACORE)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_GAMEPLAY_UI)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button_Map(LAYER_GAMEPLAY_BUTTON_Map)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button_Champion(LAYER_GAMEPLAY_BUTTON_Champion)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Market(LAYER_GAMEPLAY_MARKET)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Tile()))
		return E_FAIL;

	if (FAILED(Ready_Layer_PickingObject(LAYER_GAMEPLAY_PICKING_OBJECT)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Mouse(LAYER_GAMEPLAY_MOUSE)))
		return E_FAIL;

	

	//if (FAILED(Ready_Layer_Effect(LAYER_GAMEPLAY_EFFECT)))
	//	return E_FAIL;
	

	return S_OK;
}

void CLevel_GamePlay::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
		
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	//SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	//LightDesc.eType = tagLightDesc::TYPE_POINT;
	//LightDesc.vPosition = _float4(10.f, 4.f, 10.f, 1.f);
	//LightDesc.fRange = 7.f;
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.25f, 3.3f, -0.1f);
	CameraDesc.vAt = _float3(0.25f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	/*CameraDesc.vEye = _float3(0.f, 10.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);*/

	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 300.0f;
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc)))
		return E_FAIL;

	

	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, pLayerTag);
	if (nullptr != pLayer) {
		auto& ObjectLst = pLayer->Get_ObjectList();
		dynamic_cast<CCamera_Default*>(ObjectLst.front())
			->SetMove(
				XMLoadFloat3(&_float3(0.25f, 3.3f, -0.1f)),
				XMLoadFloat3(&_float3(0.25f, 0.f, 0.f)),
				XMLoadFloat3(&_float3(0.f, 1.f, 0.f)),
				XMLoadFloat3(&_float3(0.25f, 6.0f, -2.0f)),
				XMLoadFloat3(&_float3(0.25f, 1.5f, 0.8f)),
				XMLoadFloat3(&_float3(0.f, 1.f, 0.f)),
				0.5	// 5.0
			);
	
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Button_Map(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

#pragma region MiniMap

	CButton_MiniMap::BtMiniMapDesc btMiniMapDesc;
	ZeroMemory(&btMiniMapDesc, sizeof(CButton_MiniMap::BtMiniMapDesc));

	// 3x3 중 (0, 0), 0번째 개인맵으로 이동
	btMiniMapDesc.eMapName = (CPersonalMap::MAP_NAME)0;
	btMiniMapDesc.eType = CButton::BT_MINIMAP;
	RECT rt;
	rt.left = 1151;
	rt.right = 1194;
	rt.top = 617;
	rt.bottom = 658;
	btMiniMapDesc.rtButtonInfo = rt;
	_vector vPos = Find_PersonalMapTransform(CPersonalMap::MAP_NAME_Celebrationtomorrow)->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&btMiniMapDesc.vEye, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vAt, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f) + XMVectorSet(0.f, -5.5f, -0.2f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vUp, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Button_MiniMap"), &btMiniMapDesc)))
		return E_FAIL;



	// 3x3 중 (0, 1), 1번째 개인맵으로 이동
	btMiniMapDesc.eMapName = (CPersonalMap::MAP_NAME)1;
	rt.left = 1194;
	rt.right = 1237;
	rt.top = 617;
	rt.bottom = 658;
	btMiniMapDesc.rtButtonInfo = rt;
	vPos = Find_PersonalMapTransform(CPersonalMap::MAP_NAME_Hexbattle)->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&btMiniMapDesc.vEye, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vAt, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f) + XMVectorSet(0.f, -5.5f, -0.2f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vUp, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Button_MiniMap"), &btMiniMapDesc)))
		return E_FAIL;



	// 3x3 중 (0, 2), 2번째 개인맵으로 이동
	btMiniMapDesc.eMapName = (CPersonalMap::MAP_NAME)2;
	rt.left = 1237;
	rt.right = 1280;
	rt.top = 617;
	rt.bottom = 658;
	btMiniMapDesc.rtButtonInfo = rt;
	vPos = Find_PersonalMapTransform(CPersonalMap::MAP_NAME_ZaunCity)->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&btMiniMapDesc.vEye, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vAt, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f) + XMVectorSet(0.f, -5.5f, -0.2f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vUp, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Button_MiniMap"), &btMiniMapDesc)))
		return E_FAIL;



	// 3x3 중 (1, 0), 3번째 개인맵으로 이동
	btMiniMapDesc.eMapName = (CPersonalMap::MAP_NAME)3;
	rt.left = 1151;
	rt.right = 1194;
	rt.top = 658;
	rt.bottom = 699;
	btMiniMapDesc.rtButtonInfo = rt;
	vPos = Find_PersonalMapTransform(CPersonalMap::MAP_NAME_Jinx)->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&btMiniMapDesc.vEye, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vAt, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f) + XMVectorSet(0.f, -5.5f, -0.2f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vUp, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Button_MiniMap"), &btMiniMapDesc)))
		return E_FAIL;



	// 3x3 중 (1, 2), 4번째 개인맵으로 이동
	btMiniMapDesc.eMapName = (CPersonalMap::MAP_NAME)4;
	rt.left = 1237;
	rt.right = 1280;
	rt.top = 658;
	rt.bottom = 699;
	btMiniMapDesc.rtButtonInfo = rt;
	vPos = Find_PersonalMapTransform(CPersonalMap::MAP_NAME_Vi)->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&btMiniMapDesc.vEye, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vAt, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f) + XMVectorSet(0.f, -5.5f, -0.2f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vUp, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Button_MiniMap"), &btMiniMapDesc)))
		return E_FAIL;



	// 3x3 중 (2, 0), 5번째 개인맵으로 이동
	btMiniMapDesc.eMapName = (CPersonalMap::MAP_NAME)5;
	rt.left = 1151;
	rt.right = 1194;
	rt.top = 699;
	rt.bottom = 740;
	btMiniMapDesc.rtButtonInfo = rt;
	vPos = Find_PersonalMapTransform(CPersonalMap::MAP_NAME_Spiritblossom_kami)->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&btMiniMapDesc.vEye, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vAt, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f) + XMVectorSet(0.f, -5.5f, -0.2f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vUp, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Button_MiniMap"), &btMiniMapDesc)))
		return E_FAIL;



	// 3x3 중 (2, 1), 6번째 개인맵으로 이동
	btMiniMapDesc.eMapName = (CPersonalMap::MAP_NAME)6;
	rt.left = 1194;
	rt.right = 1237;
	rt.top = 699;
	rt.bottom = 740;
	btMiniMapDesc.rtButtonInfo = rt;
	vPos = Find_PersonalMapTransform(CPersonalMap::MAP_NAME_Spiritblossom_reality)->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&btMiniMapDesc.vEye, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vAt, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f) + XMVectorSet(0.f, -5.5f, -0.2f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vUp, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Button_MiniMap"), &btMiniMapDesc)))
		return E_FAIL;
	


	// 3x3 중 (2, 2), 7번째 개인맵으로 이동
	btMiniMapDesc.eMapName = (CPersonalMap::MAP_NAME)7;
	rt.left = 1237;
	rt.right = 1280;
	rt.top = 699;
	rt.bottom = 740;
	btMiniMapDesc.rtButtonInfo = rt;
	vPos = Find_PersonalMapTransform(CPersonalMap::MAP_NAME_Spiritblossom_yokai)->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&btMiniMapDesc.vEye, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vAt, vPos + XMVectorSet(0.25f, 3.f, 0.f, 0.f) + XMVectorSet(0.f, -5.5f, -0.2f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vUp, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Button_MiniMap"), &btMiniMapDesc)))
		return E_FAIL;

#pragma endregion

#pragma region CenterMap

	// 3x3 중 (1, 1), 0번째 센터맵으로 이동
	btMiniMapDesc.eType = CButton::BT_MINIMAP;
	btMiniMapDesc.eMapName = (CPersonalMap::MAP_NAME)8;
	rt;
	rt.left = 1194;
	rt.right = 1237;
	rt.top = 658;
	rt.bottom = 699;
	btMiniMapDesc.rtButtonInfo = rt;
	vPos = Find_CenterMapTransform()->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&btMiniMapDesc.vEye, vPos + XMVectorSet(0.25f, 6.0f, -2.0f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vAt, vPos + XMVectorSet(0.25f, 1.5f, 0.8f, 0.f));
	XMStoreFloat3(&btMiniMapDesc.vUp, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Button_MiniMap"), &btMiniMapDesc)))
		return E_FAIL;

#pragma endregion


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Button_Champion(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 챔피언 버튼 정보 추가
	for (int i = 0; i < ChampionName::ChampionName_END; ++i) {
		CButton_Champion::BtChampBorderDesc btDesc;
		btDesc.eName = (ChampionName)i;
		btDesc.eType = CButton::BUTTON_TYPE::BT_CHAMPION;

		//$5, 0 ~ 8
		if (0 <= i && i <= 8)
			btDesc.iLevel = 5;
		//$4, 9 ~ 20
		if (9 <= i && i <= 20)
			btDesc.iLevel = 4;
		//$3, 21 ~ 33
		if (21 <= i && i <= 33)
			btDesc.iLevel = 3;
		//$2, 34 ~ 46
		if (34 <= i && i <= 46)
			btDesc.iLevel = 2;
		//$1, 47 ~ 59
		if (47 <= i && i <= 59)
			btDesc.iLevel = 1;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Button_Champion"), &btDesc)))
			return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Button_LevelUp(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Button_Reroll(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Button_LockUnlock(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CLevel_GamePlay::Create_FieldTile(_vector vPos, _float2 fScale, const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTile::TILEDesc TileDesc;
	TileDesc.eType = CTile::TILE_FIELD;
	XMStoreFloat3(&TileDesc.vPos, vPos);
	TileDesc.vScale = fScale;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tile"), &TileDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Create_LoungeTile(_vector vPos, _float2 fScale, const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CTile::TILEDesc TileDesc;
	TileDesc.eType = CTile::TILE_LOUNGE;
	XMStoreFloat3(&TileDesc.vPos, vPos);
	TileDesc.vScale = fScale;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Tile"), &TileDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer::PLAYERDESC PlayerDesc;

	// 말랑이
	PlayerDesc.vPos = _float3(-0.75f, 4.5f, 0.f);
	PlayerDesc.CharacterName = Character_Pet_Sgcat;
	PlayerDesc.pPrototype_CharacterName = TEXT("Prototype_Component_Model_Player_Pet_Sgcat");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"), &PlayerDesc)))
		return E_FAIL;

	//// 아오신
	//PlayerDesc.vPos = _float3(-0.25f, 4.5f, 0.f);
	//PlayerDesc.CharacterName = Character_Pet_Aoshin;
	//PlayerDesc.pPrototype_CharacterName = TEXT("Prototype_Component_Model_Player_Pet_Aoshin");
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"), &PlayerDesc)))
	//	return E_FAIL;

	//// 배불뚝이
	//PlayerDesc.vPos = _float3(0.25f, 4.5f, 0.f);
	//PlayerDesc.CharacterName = Character_Pet_Choncc;
	//PlayerDesc.pPrototype_CharacterName = TEXT("Prototype_Component_Model_Player_Pet_Choncc");
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"), &PlayerDesc)))
	//	return E_FAIL;

	//// 크롱이
	//PlayerDesc.vPos = _float3(0.75f, 4.5f, 0.f);
	//PlayerDesc.CharacterName = Character_Pet_Umbra;
	//PlayerDesc.pPrototype_CharacterName = TEXT("Prototype_Component_Model_Player_Pet_Umbra");
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Player"), &PlayerDesc)))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Champion(const _tchar * pLayerTag)
{
	if (FAILED(Ready_Layer_Champion_Cost1(pLayerTag)))
		return E_FAIL;
	if (FAILED(Ready_Layer_Champion_Cost2(pLayerTag)))
		return E_FAIL;
	if (FAILED(Ready_Layer_Champion_Cost3(pLayerTag)))
		return E_FAIL;
	if (FAILED(Ready_Layer_Champion_Cost4(pLayerTag)))
		return E_FAIL;
	if (FAILED(Ready_Layer_Champion_Cost5(pLayerTag)))
		return E_FAIL;


	CGameInstance* pGameInstacne = GET_INSTANCE(CGameInstance);


	CLayer* pLayer = pGameInstacne->Find_Layer(LEVEL_GAMEPLAY, pLayerTag);

	if (nullptr != pLayer) {
		auto& ObjectList = pLayer->Get_ObjectList();

		for (auto& Object : ObjectList) {
			dynamic_cast<CChampion*>(Object)->Set_Show(false);
		}
	}


	RELEASE_INSTANCE(CGameInstance);




	/*ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Ahri");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.0f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Alistar");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.1f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Ashe");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.2f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Blitzcrank");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.3f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Braum");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.4f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Caitlyn");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.5f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Camille");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.6f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;*/

	/*ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Chogath");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Corki");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Darius");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Draven");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Echo");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Ezreal");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Galio");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Gangplank");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Gnar");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Illaoi");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Irelia");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Jarvan4");
	memcpy(ChampDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;*/


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Champion_Cost1(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CChampion::CHAMPDESC ChampDesc;
	ZeroMemory(&ChampDesc, sizeof(CChampion::CHAMPDESC));

	XMStoreFloat4x4(&ChampDesc.TransformMatrix, XMMatrixIdentity());

	memcpy(ChampDesc.TransformMatrix.m[3], &_float3(0.f, 4.f, 0.f), sizeof(_float3));

	// 녹턴, 다리우스, 브랜드, 뽀삐, 신지드, 이즈리얼, 일라오이, 자르반4세, 직스, 카밀, 카사딘, 케이틀린, 트위치

	ChampDesc.ChampName = ChampionName::Nocturne;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Nocturne");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Darius;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Darius");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Brand;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Brand");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Poppy;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Poppy");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Singed;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Singed");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Ezreal;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Ezreal");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Illaoi;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Illaoi");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Jarvan_IV;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Jarvan4");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Ziggs;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Ziggs");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Camille;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Camille");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Kassadin;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Kassadin");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Caitlyn;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Caitlyn");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;

	ChampDesc.ChampName = ChampionName::Twitch;
	ChampDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Champion_Twitch");
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Champion"), &ChampDesc)))
		return E_FAIL;












	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Champion_Cost2(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CChampion::CHAMPDESC ChampDesc;
	ZeroMemory(&ChampDesc, sizeof(CChampion::CHAMPDESC));

	XMStoreFloat4x4(&ChampDesc.TransformMatrix, XMMatrixIdentity());

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Champion_Cost3(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CChampion::CHAMPDESC ChampDesc;
	ZeroMemory(&ChampDesc, sizeof(CChampion::CHAMPDESC));

	XMStoreFloat4x4(&ChampDesc.TransformMatrix, XMMatrixIdentity());

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Champion_Cost4(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CChampion::CHAMPDESC ChampDesc;
	ZeroMemory(&ChampDesc, sizeof(CChampion::CHAMPDESC));

	XMStoreFloat4x4(&ChampDesc.TransformMatrix, XMMatrixIdentity());

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Champion_Cost5(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CChampion::CHAMPDESC ChampDesc;
	ZeroMemory(&ChampDesc, sizeof(CChampion::CHAMPDESC));

	XMStoreFloat4x4(&ChampDesc.TransformMatrix, XMMatrixIdentity());

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Mouse(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CMouse::MOUSEDESC MouseDesc;
	ZeroMemory(&MouseDesc, sizeof(CMouse::MOUSEDESC));
	
	// 마우스의 가로/세로 이미지 크기
	MouseDesc.fSize.x = 36.f;
	MouseDesc.fSize.y = 36.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Mouse"), &MouseDesc)))
		return E_FAIL;
	
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_CenterMap(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	// 맵의 위치 - float4(0.f, 0.f, 0.f, 1.f)
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_CenterMap"))))
		return E_FAIL;

	CMapObject::MAPOBJDesc MapObjDesc;
	XMStoreFloat4x4(&MapObjDesc.TransformMatrix, XMMatrixIdentity());

	MapObjDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_CenterMap_Circle");
	memcpy(MapObjDesc.TransformMatrix.m[3], &_float4(0.25f, 3.78f, -0.11f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_MapObject"), &MapObjDesc)))
		return E_FAIL;

	MapObjDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_CenterMap_Elevator");
	memcpy(MapObjDesc.TransformMatrix.m[3], &_float4(0.25f, 3.7f, -0.1f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_MapObject"), &MapObjDesc)))
		return E_FAIL;

	MapObjDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_CenterMap_Gears");
	memcpy(MapObjDesc.TransformMatrix.m[3], &_float4(0.27f, 3.75f, -0.13f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_MapObject"), &MapObjDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_PersonalMap(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	// 맵의 위치 - float4(MAP_GAP, 0.f, 0.f, 1.f) + 맵의 이름(순번) * float4(MAP_GAP, 0.f, 0.f, 0.f)
	_vector vPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	for (int i = 0; i < (int)CPersonalMap::MAP_NAME_END; ++i) {
		CPersonalMap::PersonalMapDesc PMDesc;
		PMDesc.eType = (CPersonalMap::MAP_NAME)i;
		vPos = XMVectorSet(MAP_GAP * (i + 1), 0.f, 0.f, 1.f);
		
		switch (PMDesc.eType)
		{
		case Client::CPersonalMap::MAP_NAME_Celebrationtomorrow:
			break;
		case Client::CPersonalMap::MAP_NAME_Hexbattle:
			vPos += XMVectorSet(0.235f, 0.38f, -0.31f, 0.f);
			break;
		case Client::CPersonalMap::MAP_NAME_ZaunCity:
			vPos += XMVectorSet(0.21f, -1.4f, -0.195f, 0.f);
			break;
		case Client::CPersonalMap::MAP_NAME_Jinx:
			vPos += XMVectorSet(0.815f, 0.39f, 0.15f, 0.f);
			break;
		case Client::CPersonalMap::MAP_NAME_Vi:
			vPos += XMVectorSet(0.f, 0.77f, -0.65f, 0.f);
			break;
		case Client::CPersonalMap::MAP_NAME_Spiritblossom_kami:
			vPos += XMVectorSet(-0.05f, -0.2f, -0.5f, 0.f);
			break;
		case Client::CPersonalMap::MAP_NAME_Spiritblossom_reality:
			vPos += XMVectorSet(0.265f, -0.45f, -0.805f, 0.f);
			break;
		case Client::CPersonalMap::MAP_NAME_Spiritblossom_yokai:
			vPos += XMVectorSet(0.105f, -0.2f, -0.41f, 0.f);
			break;
		}

		XMStoreFloat4x4(&PMDesc.TransformMatrix, XMMatrixIdentity());
		XMStoreFloat4((XMFLOAT4*)PMDesc.TransformMatrix.m[CTransform::STATE_POSITION], vPos);

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_PersonalMap"), &PMDesc)))
			return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Hexacore(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CMapObject::MAPOBJDesc MapObjDesc;
	XMStoreFloat4x4(&MapObjDesc.TransformMatrix, XMMatrixIdentity());

	MapObjDesc.pPrototype_ObjectName = TEXT("Prototype_Component_Model_Hexacore");
	memcpy(MapObjDesc.TransformMatrix.m[3], &_float4(0.0f, 4.1f, 0.0f, 1.f), sizeof(_float4));
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_MapObject"), &MapObjDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	for (_uint i = 0; i < 20; ++i)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Effect"))))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_UI_GamePlay"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Market(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Market"))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Tile()
{
	if (FAILED(Ready_Layer_Tile_PersonalMap01()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Tile_PersonalMap02()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Tile_PersonalMap03()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Tile_PersonalMap04()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Tile_PersonalMap05()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Tile_PersonalMap06()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Tile_PersonalMap07()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Tile_PersonalMap08()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Tile_PersonalMap01()
{
	// 타일 까는것은 좌하단부터 시작, 타일이 정육각형이라는 가정하에 작성. 
	// 좌하단의 첫번째 타일의 중점을 0.f, 0.f라 한다.

	// 타일의 세로 길이를 Tile_Vertical 이라 한다.
	_float Tile_Vertical = 0.35f;
	_float2 vScaled;
	vScaled.x = Tile_Vertical * 0.92;
	vScaled.y = Tile_Vertical * 1.15;

	// 1번째 맵 - 필드 타일
	_float3 vMapPos;
	vMapPos.x = MAP_GAP - 0.71f;
	vMapPos.y = -4.08f;
	vMapPos.z = -1.53f;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 7; ++j) {
			//Create_FieldTile(XMVectorSet(Tile_Vertical * j - (i % 2) * Tile_Vertical * 0.5f, 4.f, Tile_Vertical * 3.f * 0.25f * i, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f), Tile_Vertical);
			Create_FieldTile(
				XMVectorSet(
				(Tile_Vertical * j - (i % 2) * Tile_Vertical * 0.5f) * 0.92f, 4.f, Tile_Vertical * 3.f * 0.25f * 1.15f * i, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
				vScaled,
				LAYER_GAMEPLAY_TILE_PERSONAL01_FIELD
			);
		}
	}

	// 1번째 맵 - 라운지 타일
	vMapPos.x = MAP_GAP - 1.15f;
	vMapPos.z = -1.94f;
	Tile_Vertical = 0.32f;
	vScaled.x = Tile_Vertical * 0.8;
	vScaled.y = Tile_Vertical * 0.95;

	for (int i = 0; i < 9; ++i) {
		Create_LoungeTile(XMVectorSet((Tile_Vertical * i) * 0.95f, 4.f, 0.f, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
			vScaled,
			LAYER_GAMEPLAY_TILE_PERSONAL01_LOUNGE
		);
	}

	if (FAILED(Ready_PushTileInfo(LAYER_GAMEPLAY_TILE_PERSONAL01_FIELD, LAYER_GAMEPLAY_TILE_PERSONAL01_LOUNGE, CPersonalMap::MAP_NAME_Celebrationtomorrow)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Tile_PersonalMap02()
{
	_float Tile_Vertical = 0.35f;
	_float2 vScaled;
	vScaled.x = Tile_Vertical * 0.92;
	vScaled.y = Tile_Vertical * 1.15;

	// 2번째 맵 - 필드 타일
	_float3 vMapPos;
	vMapPos.x = MAP_GAP * 2 - 0.71f;
	vMapPos.y = -4.08f;
	vMapPos.z = -1.53f;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 7; ++j) {
			Create_FieldTile(
				XMVectorSet(
				(Tile_Vertical * j - (i % 2) * Tile_Vertical * 0.5f) * 0.92f, 4.f, Tile_Vertical * 3.f * 0.25f * 1.15f * i, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
				vScaled,
				LAYER_GAMEPLAY_TILE_PERSONAL02_FIELD
			);
		}
	}

	// 2번째 맵 - 라운지 타일
	vMapPos.x = MAP_GAP * 2 - 1.15f;
	vMapPos.y = -4.1f;
	vMapPos.z = -1.94f;
	Tile_Vertical = 0.32f;
	vScaled.x = Tile_Vertical * 0.8;
	vScaled.y = Tile_Vertical * 0.95;

	for (int i = 0; i < 9; ++i) {
		Create_LoungeTile(XMVectorSet((Tile_Vertical * i) * 0.95f, 4.f, 0.f, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
			vScaled,
			LAYER_GAMEPLAY_TILE_PERSONAL02_LOUNGE
		);
	}

	if (FAILED(Ready_PushTileInfo(LAYER_GAMEPLAY_TILE_PERSONAL02_FIELD, LAYER_GAMEPLAY_TILE_PERSONAL02_LOUNGE, CPersonalMap::MAP_NAME_Hexbattle)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Tile_PersonalMap03()
{
	_float Tile_Vertical = 0.35f;
	_float2 vScaled;
	vScaled.x = Tile_Vertical * 0.92;
	vScaled.y = Tile_Vertical * 1.15;

	// 3번째 맵 - 필드 타일
	_float3 vMapPos;
	vMapPos.x = MAP_GAP * 3 - 0.71f;
	vMapPos.y = -4.08f;
	vMapPos.z = -1.53f;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 7; ++j) {
			Create_FieldTile(
				XMVectorSet(
				(Tile_Vertical * j - (i % 2) * Tile_Vertical * 0.5f) * 0.92f, 4.f, Tile_Vertical * 3.f * 0.25f * 1.15f * i, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
				vScaled,
				LAYER_GAMEPLAY_TILE_PERSONAL03_FIELD
			);
		}
	}

	// 3번째 맵 - 라운지 타일
	vMapPos.x = MAP_GAP * 3 - 1.1f;
	vMapPos.z = -1.94f;
	Tile_Vertical = 0.32f;
	vScaled.x = Tile_Vertical * 0.8;
	vScaled.y = Tile_Vertical * 0.95;

	for (int i = 0; i < 9; ++i) {
		Create_LoungeTile(XMVectorSet((Tile_Vertical * i) * 0.95f, 4.f, 0.f, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
			vScaled,
			LAYER_GAMEPLAY_TILE_PERSONAL03_LOUNGE
		);
	}

	if (FAILED(Ready_PushTileInfo(LAYER_GAMEPLAY_TILE_PERSONAL03_FIELD, LAYER_GAMEPLAY_TILE_PERSONAL03_LOUNGE, CPersonalMap::MAP_NAME_ZaunCity)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Tile_PersonalMap04()
{
	_float Tile_Vertical = 0.35f;
	_float2 vScaled;
	vScaled.x = Tile_Vertical * 0.92;
	vScaled.y = Tile_Vertical * 1.15;

	// 4번째 맵 - 필드 타일
	_float3 vMapPos;
	vMapPos.x = MAP_GAP * 4 - 0.8f;
	vMapPos.y = -4.08f;
	vMapPos.z = -1.53f;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 7; ++j) {
			Create_FieldTile(
				XMVectorSet(
				(Tile_Vertical * j - (i % 2) * Tile_Vertical * 0.5f) * 0.92f, 4.f, Tile_Vertical * 3.f * 0.25f * 1.15f * i, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
				vScaled,
				LAYER_GAMEPLAY_TILE_PERSONAL04_FIELD
			);
		}
	}

	// 4번째 맵 - 라운지 타일
	vMapPos.x = MAP_GAP * 4 - 1.15f;
	vMapPos.z = -1.94f;
	Tile_Vertical = 0.32f;
	vScaled.x = Tile_Vertical * 0.8;
	vScaled.y = Tile_Vertical * 0.95;

	for (int i = 0; i < 9; ++i) {
		Create_LoungeTile(XMVectorSet((Tile_Vertical * i) * 0.95f, 4.f, 0.f, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
			vScaled,
			LAYER_GAMEPLAY_TILE_PERSONAL04_LOUNGE
		);
	}

	if (FAILED(Ready_PushTileInfo(LAYER_GAMEPLAY_TILE_PERSONAL04_FIELD, LAYER_GAMEPLAY_TILE_PERSONAL04_LOUNGE, CPersonalMap::MAP_NAME_Jinx)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Tile_PersonalMap05()
{
	_float Tile_Vertical = 0.35f;
	_float2 vScaled;
	vScaled.x = Tile_Vertical * 0.92;
	vScaled.y = Tile_Vertical * 1.15;

	// 5번째 맵 - 필드 타일
	_float3 vMapPos;
	vMapPos.x = MAP_GAP * 5 - 0.83f;
	vMapPos.y = -4.08f;
	vMapPos.z = -1.53f;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 7; ++j) {
			Create_FieldTile(
				XMVectorSet(
				(Tile_Vertical * j - (i % 2) * Tile_Vertical * 0.5f) * 0.92f, 4.f, Tile_Vertical * 3.f * 0.25f * 1.15f * i, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
				vScaled,
				LAYER_GAMEPLAY_TILE_PERSONAL05_FIELD
			);
		}
	}

	// 2번째 맵 - 라운지 타일
	vMapPos.x = MAP_GAP * 5 - 1.15f;
	vMapPos.z = -1.94f;
	Tile_Vertical = 0.32f;
	vScaled.x = Tile_Vertical * 0.8;
	vScaled.y = Tile_Vertical * 0.95;

	for (int i = 0; i < 9; ++i) {
		Create_LoungeTile(XMVectorSet((Tile_Vertical * i) * 0.95f, 4.f, 0.f, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
			vScaled,
			LAYER_GAMEPLAY_TILE_PERSONAL05_LOUNGE
		);
	}

	if (FAILED(Ready_PushTileInfo(LAYER_GAMEPLAY_TILE_PERSONAL05_FIELD, LAYER_GAMEPLAY_TILE_PERSONAL05_LOUNGE, CPersonalMap::MAP_NAME_Vi)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Tile_PersonalMap06()
{
	_float Tile_Vertical = 0.35f;
	_float2 vScaled;
	vScaled.x = Tile_Vertical * 0.92;
	vScaled.y = Tile_Vertical * 1.15;

	// 6번째 맵 - 필드 타일
	_float3 vMapPos;
	vMapPos.x = MAP_GAP * 6 - 0.8f;
	vMapPos.y = -4.08f;
	vMapPos.z = -1.53f;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 7; ++j) {
			Create_FieldTile(
				XMVectorSet(
				(Tile_Vertical * j - (i % 2) * Tile_Vertical * 0.5f) * 0.92f, 4.f, Tile_Vertical * 3.f * 0.25f * 1.15f * i, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
				vScaled,
				LAYER_GAMEPLAY_TILE_PERSONAL06_FIELD
			);
		}
	}

	// 6번째 맵 - 라운지 타일
	vMapPos.x = MAP_GAP * 6 - 1.15f;
	vMapPos.z = -1.94f;
	Tile_Vertical = 0.32f;
	vScaled.x = Tile_Vertical * 0.8;
	vScaled.y = Tile_Vertical * 0.95;

	for (int i = 0; i < 9; ++i) {
		Create_LoungeTile(XMVectorSet((Tile_Vertical * i) * 0.95f, 4.f, 0.f, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
			vScaled,
			LAYER_GAMEPLAY_TILE_PERSONAL06_LOUNGE
		);
	}

	if (FAILED(Ready_PushTileInfo(LAYER_GAMEPLAY_TILE_PERSONAL06_FIELD, LAYER_GAMEPLAY_TILE_PERSONAL06_LOUNGE, CPersonalMap::MAP_NAME_Spiritblossom_kami)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Tile_PersonalMap07()
{
	_float Tile_Vertical = 0.35f;
	_float2 vScaled;
	vScaled.x = Tile_Vertical * 0.92;
	vScaled.y = Tile_Vertical * 1.15;

	// 7번째 맵 - 필드 타일
	_float3 vMapPos;
	vMapPos.x = MAP_GAP * 7 - 0.75f;
	vMapPos.y = -4.08f;
	vMapPos.z = -1.53f;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 7; ++j) {
			Create_FieldTile(
				XMVectorSet(
				(Tile_Vertical * j - (i % 2) * Tile_Vertical * 0.5f) * 0.92f, 4.f, Tile_Vertical * 3.f * 0.25f * 1.15f * i, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
				vScaled,
				LAYER_GAMEPLAY_TILE_PERSONAL07_FIELD
			);
		}
	}

	// 7번째 맵 - 라운지 타일
	vMapPos.x = MAP_GAP * 7 - 1.15f;
	vMapPos.z = -1.94f;
	Tile_Vertical = 0.32f;
	vScaled.x = Tile_Vertical * 0.8;
	vScaled.y = Tile_Vertical * 0.95;

	for (int i = 0; i < 9; ++i) {
		Create_LoungeTile(XMVectorSet((Tile_Vertical * i) * 0.95f, 4.f, 0.f, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
			vScaled,
			LAYER_GAMEPLAY_TILE_PERSONAL07_LOUNGE
		);
	}

	if (FAILED(Ready_PushTileInfo(LAYER_GAMEPLAY_TILE_PERSONAL07_FIELD, LAYER_GAMEPLAY_TILE_PERSONAL07_LOUNGE, CPersonalMap::MAP_NAME_Spiritblossom_reality)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Tile_PersonalMap08()
{
	_float Tile_Vertical = 0.35f;
	_float2 vScaled;
	vScaled.x = Tile_Vertical * 0.92;
	vScaled.y = Tile_Vertical * 1.15;

	// 8번째 맵 - 필드 타일
	_float3 vMapPos;
	vMapPos.x = MAP_GAP * 8 - 0.8f;
	vMapPos.y = -4.08f;
	vMapPos.z = -1.53f;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 7; ++j) {
			Create_FieldTile(
				XMVectorSet(
				(Tile_Vertical * j - (i % 2) * Tile_Vertical * 0.5f) * 0.92f, 4.f, Tile_Vertical * 3.f * 0.25f * 1.15f * i, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
				vScaled,
				LAYER_GAMEPLAY_TILE_PERSONAL08_FIELD
			);
		}
	}

	// 8번째 맵 - 라운지 타일
	vMapPos.x = MAP_GAP * 8 - 1.15f;
	vMapPos.z = -1.94f;
	Tile_Vertical = 0.32f;
	vScaled.x = Tile_Vertical * 0.8;
	vScaled.y = Tile_Vertical * 0.95;

	for (int i = 0; i < 9; ++i) {
		Create_LoungeTile(XMVectorSet((Tile_Vertical * i) * 0.95f, 4.f, 0.f, 1.f) + XMVectorSet(vMapPos.x, vMapPos.y, vMapPos.z, 1.f),
			vScaled,
			LAYER_GAMEPLAY_TILE_PERSONAL08_LOUNGE
		);
	}

	if (FAILED(Ready_PushTileInfo(LAYER_GAMEPLAY_TILE_PERSONAL08_FIELD, LAYER_GAMEPLAY_TILE_PERSONAL08_LOUNGE, CPersonalMap::MAP_NAME_Spiritblossom_yokai)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_PushTileInfo(const _tchar * pLayerTagField, const _tchar * pLayerTagLounge, CPersonalMap::MAP_NAME eName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 플레이어의 정보를 가져온다.
	CLayer* pLayerPlayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(*pLayerPlayer->Get_ObjectList().begin());

	// 필드 타일의 정보를 가져온다.
	CLayer* pLayerField = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, pLayerTagField);
	if (nullptr == pLayerField)
		return E_FAIL;

	// 가져온 모든필드 타일의 정보를 플레이어게 넣는다.
	for (auto& Tile : pLayerField->Get_ObjectList())
		pPlayer->PushTile(CTile::TILE_FIELD, dynamic_cast<CTile*>(Tile), eName);

	// 라운지 타일의 정보를 가져온다.
	CLayer* pLayerLounge = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, pLayerTagLounge);
	if (nullptr == pLayerLounge)
		return E_FAIL;

	// 가져온 모든필드 타일의 정보를 플레이어게 넣는다.
	for (auto& Tile : pLayerLounge->Get_ObjectList())
		pPlayer->PushTile(CTile::TILE_LOUNGE, dynamic_cast<CTile*>(Tile), eName);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_PickingObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPickingObject::PICKINGDESC PickingDesc;
	PickingDesc.fSize = _float2(7.f, 7.f);
	PickingDesc.vPos = _float3(0.f, 4.f, 0.f);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Picking"), &PickingDesc)))
		return E_FAIL;

	PickingDesc.vPos = _float3(MAP_GAP, -0.18f, 0.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pLayerTag, TEXT("Prototype_GameObject_Picking"), &PickingDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CTransform * CLevel_GamePlay::Find_PersonalMapTransform(CPersonalMap::MAP_NAME eName)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_MAP_PEROSNAL);

	auto&  ObjectList = pLayer->Get_ObjectList();
	auto& iter = ObjectList.begin();
	for (; iter != ObjectList.end();) {
		CPersonalMap::MAP_NAME eMapName = dynamic_cast<CPersonalMap*>(*iter)->Get_Type();
		if (eMapName == eName)
			break;
		else
			++iter;
	}

	return dynamic_cast<CTransform*>((*iter)->Get_Component(TEXT("Com_Transform")));
}

CTransform * CLevel_GamePlay::Find_CenterMapTransform(void)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_MAP_CENTER);

	auto&  ObjectList = pLayer->Get_ObjectList();
	return dynamic_cast<CTransform*>((*ObjectList.begin())->Get_Component(TEXT("Com_Transform")));
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();


}
