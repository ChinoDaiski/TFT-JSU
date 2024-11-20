#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"

// 카메라
#include "Camera_Default.h"

// 플레이어
#include "Player.h"

// 맵 관련
#include "CenterMap.h"
#include "PersonalMap.h"
#include "MapObject.h"

// UI 관련
#include "UI_Logo.h"
#include "UI_Loading.h"
#include "UI_GamePlay.h"
#include "UI_Ending.h"

// 버튼 관련
#include "Button_MiniMap.h"
#include "Button_Champion.h"
#include "Button_LevelUp.h"
#include "Button_Reroll.h"
#include "Button_LockUnlock.h"

// 상점 관련
#include "Market.h"

// Tile 관련
#include "Tile.h"

// Picking 관련
#include "PickingObject.h"

// Mouse 관련
#include "Mouse.h"


CLoader::CLoader(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: m_pDevice(pDeviceOut), m_pDeviceContext(pDeviceContextOut)
{
	Safe_AddRef(m_pDeviceContext);
	Safe_AddRef(m_pDevice);
}

unsigned int APIENTRY ThreadEntryFunc(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_LevelIndex())
	{
	case LEVEL_LOADING:
		pLoader->Loading_ForLoadingLevel();
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlayLevel();
		break;
	/*case LEVEL_BOSS:
		pLoader->Loading_ForBossLevel();
		break;*/
	}


	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::NativeConstruct(LEVEL eLevel)
{
	m_eLevel = eLevel;

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntryFunc, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLoadingLevel()
{
	lstrcpy(m_szLoading, TEXT("텍스쳐를 로드중입니다. "));
	if (FAILED(Loading_Loading_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("모델을 로드중입니다. "));
	if (FAILED(Loading_Loading_Model()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("셰이더를 로드중입니다. "));
	if (FAILED(Loading_Loading_Shader()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("게임오브젝트를 로드중입니다. "));
	if (FAILED(Loading_Loading_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	lstrcpy(m_szLoading, TEXT("텍스쳐를 로드중입니다. "));
	if (FAILED(Loading_GamePlay_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("이펙트 텍스쳐를 로드중입니다. "));
	if (FAILED(Loading_GamePlay_Effect_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("모델을 로드중입니다. "));
	if (FAILED(Loading_GamePlay_Model()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("셰이더를 로드중입니다. "));
	if (FAILED(Loading_GamePlay_Shader()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("게임오브젝트를 로드중입니다. "));
	if (FAILED(Loading_GamePlay_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("컴포넌트를 로드중입니다. "));
	if (FAILED(Loading_GamePlay_Component()))
		return E_FAIL;
	
	
	

	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForEndingLevel()
{
	lstrcpy(m_szLoading, TEXT("텍스쳐를 로드중입니다. "));
	if (FAILED(Loading_Ending_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("모델을 로드중입니다. "));
	if (FAILED(Loading_Ending_Model()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("셰이더를 로드중입니다. "));
	if (FAILED(Loading_Ending_Shader()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("게임오브젝트를 로드중입니다. "));
	if (FAILED(Loading_Ending_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("로딩이 완료되었습니다.. "));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Loading_Texture()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	// 로딩
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Main_Loading"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Main_Loading%d.dds"), 1))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_Loading_Object()
{
	return S_OK;
}

HRESULT CLoader::Loading_Loading_Shader()
{
	return S_OK;
}

HRESULT CLoader::Loading_Loading_Model()
{
	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Texture()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	//============================================================ UI_Game =========================================================================
	// 상점 - base
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Market_Base"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Market/tft_UI_Market_base_center%d.dds"), 1))))
		return E_FAIL;
	// 상점 - level && exp
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Market_LevelExp"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Market/tft_UI_Market_base_LevelExp%d.dds"), 1))))
		return E_FAIL;
	// 상점 - gold
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Market_Gold"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Market/tft_UI_Market_Gold%d.dds"), 1))))
		return E_FAIL;

	// 상점 - XP구매
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Market_LevelUp"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Market/tft_UI_Market_Levelup%d.dds"), 4))))
		return E_FAIL;
	// 상점 - 새로고침
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Market_Reroll"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Market/tft_UI_Market_Reroll%d.dds"), 4))))
		return E_FAIL;


	// 미니맵
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Minimap"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Minimap/UI_InGame_Minimap%d.dds"), 1))))
		return E_FAIL;
	// 미니맵 커버
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_MinimapCover"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Minimap/UI_InGame_Minimap_cover%d.dds"), 1))))
		return E_FAIL;

	// 버튼
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Button"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Button%d.dds"), 1))))
		return E_FAIL;

	
	// 챔피언 UI
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ChampionBorders"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Champion_Borders/tft_set6_%d.dds"), 60))))
		return E_FAIL;

	// 상점 레벨별 챔피언 테두리
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ChampionOutline"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Borders_level/tft_UI_Champion_Border%d.dds"), 5))))
		return E_FAIL;

	// 타일
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_Map"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Texture/Stage/base_hexagon_03.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_Lounge"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Texture/Stage/base_rect_%d.dds"), 1))))
		return E_FAIL;

	// 마우스
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mouse"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Mouse/lol_new_Cursor%d.dds"), 1))))
		return E_FAIL;

	// 아이템
	if (FAILED(Loading_GamePlay_Texture_Item()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Effect_Texture()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	// 타일 이펙트
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Tile"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Tile/%d.dds"), 16))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Object()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	// 플레이어 관련
	lstrcpy(m_szLoading, TEXT("Player를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	// 맵 관련
	lstrcpy(m_szLoading, TEXT("CenterMap을 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CenterMap"),
		CCenterMap::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("PersonalMap을 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PersonalMap"),
		CPersonalMap::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("MapObject를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MapObject"),
		CMapObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;




	// 카메라 관련
	lstrcpy(m_szLoading, TEXT("Camera를 로드중입니다."));

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Default"),
		CCamera_Default::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	// 버튼 관련
	lstrcpy(m_szLoading, TEXT("Button를 로드중입니다."));

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button_MiniMap"),
		CButton_MiniMap::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button_Champion"),
		CButton_Champion::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button_LevelUp"),
		CButton_LevelUp::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button_Reroll"),
		CButton_Reroll::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Button_LockUnlock"),
		CButton_LockUnlock::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	// UI관련
	lstrcpy(m_szLoading, TEXT("UI_GamePlay를 로드중입니다."));
	// UI_GamePlay
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_GamePlay"),
		CUI_GamePlay::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("UI_Ending을 로드중입니다."));
	// UI_Ending
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Ending"),
		CUI_Ending::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	// Market 관련
	lstrcpy(m_szLoading, TEXT("Market을 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Market"),
		CMarket::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	// Champion 관련
	lstrcpy(m_szLoading, TEXT("Champion을 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Champion"),
		CChampion::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	


	// Tile 관련
	lstrcpy(m_szLoading, TEXT("Tile 정보를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tile"),
		CTile::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	// Picking 관련
	lstrcpy(m_szLoading, TEXT("PickingObject 정보를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Picking"),
		CPickingObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	// Mouse 관련
	lstrcpy(m_szLoading, TEXT("Mouse 정보를 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mouse"),
		CMouse::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	///* For.Prototype_GameObject_Sky */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
	//	CSky::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_GameObject_Effect */
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
	//	CEffect::Create(m_pGraphic_Device))))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Shader()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	/* For. Prototype_Component_Shader_VtxNorTex */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxAnim*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnim"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnim.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxAnim_Character*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimCharacter"),
		CShader::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnim_Character.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Model()
{
	// 1코 챔피언
	if (FAILED(Loading_GamePlay_Model_Champiom1Cost()))
		return E_FAIL;

	// 2코 챔피언
	if (FAILED(Loading_GamePlay_Model_Champiom2Cost()))
		return E_FAIL;

	// 3코 챔피언
	if (FAILED(Loading_GamePlay_Model_Champiom3Cost()))
		return E_FAIL;

	// 4코 챔피언
	if (FAILED(Loading_GamePlay_Model_Champiom4Cost()))
		return E_FAIL;

	// 5코 챔피언
	if (FAILED(Loading_GamePlay_Model_Champiom5Cost()))
		return E_FAIL;

	// 플레이어 캐릭터
	if (FAILED(Loading_GamePlay_Model_Character()))
		return E_FAIL;

	// 개인 맵 관련 오브젝트
	if (FAILED(Loading_GamePlay_Model_MapObject_Personal()))
		return E_FAIL;

	// 센터 맵 관련 오브젝트
	if (FAILED(Loading_GamePlay_Model_MapObject_Center()))
		return E_FAIL;

	// 헥사코어 오브젝트
	if (FAILED(Loading_GamePlay_Model_MapObject_Hexacore()))
		return E_FAIL;

	// 아이템
	if (FAILED(Loading_GamePlay_Model_Item()))
		return E_FAIL;

	

	

	
	//lstrcpy(m_szLoading, TEXT("Chogath를 로드 중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Chogath"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Chogath/",
	//		"Chogath.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Corki를 로드 중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Corki"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Corki/",
	//		"Corki.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Darius를 로드 중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Darius"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Darius/",
	//		"Darius.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Draven를 로드 중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Draven"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Draven/",
	//		"Draven.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Echo를 로드 중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Echo"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Echo/",
	//		"Echo.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Ezreal를 로드 중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Ezreal"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Ezreal/",
	//		"Ezreal.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Galio를 로드 중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Galio"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Galio/",
	//		"Galio.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Gangplank를 로드 중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Gangplank"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Gangplank/",
	//		"Gangplank.fbx", PivotMatrix))))
	//	return E_FAIL;


	/*lstrcpy(m_szLoading, TEXT("Gnar를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Gnar"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Gnar/",
			"Gnar.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("GnarBig를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_GnarBig"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/GnarBig/",
			"GnarBig.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Illaoi를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Illaoi"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Illaoi/",
			"Illaoi.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Irelia를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Irelia"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Irelia/",
			"Irelia.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Jarvan4를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Jarvan4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Jarvan4/",
			"Jarvan4.fbx", PivotMatrix))))
		return E_FAIL;*/

	//lstrcpy(m_szLoading, TEXT("Jayce_Close를 로드 중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Jayce_Close"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Jayce/",
	//		"Jayce_Close.fbx", PivotMatrix))))
	//	return E_FAIL;

	//lstrcpy(m_szLoading, TEXT("Jayce_Long를 로드 중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Jayce_Long"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Jayce/",
	//		"Jayce_Long.fbx", PivotMatrix))))
	//	return E_FAIL;

	//lstrcpy(m_szLoading, TEXT("Camille를 로드 중입니다."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Camille"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Camille/",
	//		"Camille.fbx", PivotMatrix))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Component()
{
	// 충돌체 컴포넌트 준비
	if (FAILED(Loading_GamePlay_Component_Collison()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Texture_Item()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/*
	// bf 대검
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 피바라기
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 블루
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 가시갑옷
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 사슬조끼
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 성배
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 죽음의 검
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 얼어붙은 심장
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 거인의 벨트
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 가고일의 심장
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 거인 학살자
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 수호천사
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 구인수의 격노검
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 정의의 손길
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 무한의 대검
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 보석 건틀릿
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 최후의 속삭임
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 솔라리의 펜던트
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 모렐로노미콘
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 쓸데없이 큰 지팡이
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 마법저항의 망토
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 수은
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 라바돈의 죽음모자
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 고속연사포
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 곡궁
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 구원
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 루난의 허리케인
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 침묵의 장막
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 싸움꾼의 장갑
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 뒤집개
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 쇼진의 창
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 태양불꽃 망토
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 여신의 눈물
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 도적의 장갑
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 덫발톱
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 워모그의 벨트
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 서풍
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// 즈롯 차원문
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL; 
	*/;
	
	lstrcpy(m_szLoading, TEXT("Texture_Item를 로드 중입니다."));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_Material"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Material/%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_Spatula"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Spatula/%d.dds"), 19))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_Standard"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/%d.dds"), 38))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Model_Champiom1Cost()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	PivotMatrix = XMMatrixScaling(0.00002f, 0.00002f, 0.00002f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	// 녹턴, 다리우스, 브랜드, 뽀삐, 신지드, 이즈리얼, 일라오이, 자르반4세, 직스, 카밀, 카사딘, 케이틀린, 트위치
	lstrcpy(m_szLoading, TEXT("Nocturne를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Nocturne"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Nocturne/",
			"Nocturne.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Darius를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Darius"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Darius/",
			"Darius.fbx", PivotMatrix)))) 
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Brand를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Brand"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Brand/",
			"Brand.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Poppy를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Poppy"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Poppy/",
			"Poppy.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Singed를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Singed"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Singed/",
			"Singed.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Ezreal를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Ezreal"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Ezreal/",
			"Ezreal.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Illaoi를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Illaoi"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Illaoi/",
			"Illaoi.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Jarvan4를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Jarvan4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Jarvan4/",
			"Jarvan4.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Ziggs를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Ziggs"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Ziggs/",
			"Ziggs.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Camille를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Camille"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Camille/",
			"Camille.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Kassadin를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Kassadin"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Kassadin/",
			"Kassadin.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Caitlyn를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Caitlyn"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Caitlyn/",
			"Caitlyn.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Twitch를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Twitch"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Twitch/",
			"Twitch.fbx", PivotMatrix))))
		return E_FAIL;












/*
	lstrcpy(m_szLoading, TEXT("Ahri를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Ahri"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Ahri/",
			"Ahri.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Alistar를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Alistar"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Alistar/",
			"Alistar.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Ashe를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Ashe"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Ashe/",
			"Ashe.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Blitzcrank를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Blitzcrank"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Blitzcrank/",
			"Blitzcrank.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Brand를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Brand"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Brand/",
			"Brand.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Braum를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Braum"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Braum/",
			"Braum.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Caitlyn를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Caitlyn"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Caitlyn/",
			"Caitlyn.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Camille를 로드 중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Camille"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Camille/",
			"Camille.fbx", PivotMatrix))))
		return E_FAIL;
*/
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Model_Champiom2Cost()
{
	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Model_Champiom3Cost()
{
	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Model_Champiom4Cost()
{
	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Model_Champiom5Cost()
{
	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Model_Character()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;
	PivotMatrix = XMMatrixScaling(0.000015f, 0.000015f, 0.000015f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	// ============================================================ 플레이어 캐릭터 =========================================================================

	lstrcpy(m_szLoading, TEXT("Player_Pet_Sgcat을 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Pet_Sgcat"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Character/petSgcat/",
			"peteSgcat.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Player_Pet_Aoshin을 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Pet_Aoshin"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Character/petAoshin/",
			"petAoshin.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Player_Pet_Choncc을 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Pet_Choncc"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Character/petChoncc/",
			"petChoncc.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Player_Pet_Umbra을 로드중입니다."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Pet_Umbra"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Character/petUmbra/",
			"petUmbra.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CLoader::Loading_GamePlay_Model_MapObject_Personal()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	// ============================================================  개인 맵 =========================================================================

	lstrcpy(m_szLoading, TEXT("개인 맵(Celebrationtomorrow)을 로드중입니다."));
	// 맵 - Celebrationtomorrow 로드
	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	//PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_CelebrationTomorrow"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/Hexbattle_Celebrationtomorrow_Zaunbotanicalgarden/",
			"set6_5_celebrationtomorrow.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("개인 맵(Hexbattle)을 로드중입니다."));
	// 맵 - Hexbattle 로드
	PivotMatrix = XMMatrixScaling(0.95f, 1.2f, 1.02f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Hexbattle"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/Hexbattle_Celebrationtomorrow_Zaunbotanicalgarden/",
			"set6_5_hexbattle.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("개인 맵(ZaunCity)을 로드중입니다."));
	// 맵 - ZaunCity 로드
	PivotMatrix = XMMatrixScaling(1.1f, 1.075f, 1.24f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_ZaunCity"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/ZaunCity/",
			"ZaunCity.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("개인 맵(Jinx)을 로드중입니다."));
	// 맵 - Jinx 로드
	PivotMatrix = XMMatrixScaling(0.49f, 0.47f, 0.47f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Jinx"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/Jinx_Vi/",
			"set6_jinx.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("개인 맵(Vi)을 로드중입니다."));
	// 맵 - Vi 로드
	PivotMatrix = XMMatrixScaling(0.324f, 0.328f, 0.328f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Vi"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/Jinx_Vi/",
			"set6_vi.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("개인 맵(Spiritblossom_kami)을 로드중입니다."));
	// 맵 - Spiritblossom_kami 로드
	PivotMatrix = XMMatrixScaling(0.63f, 0.65f, 0.69f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Spiritblossom_kami"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/SpiritBlossom/",
			"spiritblossom_kami.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("개인 맵(Spiritblossom_reality)을 로드중입니다."));
	// 맵 - Spiritblossom_reality 로드
	PivotMatrix = XMMatrixScaling(0.732f, 0.65f, 0.8f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Spiritblossom_reality"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/SpiritBlossom/",
			"spiritblossom_reality.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("개인 맵(Spiritblossom_yokai)을 로드중입니다."));
	// 맵 - Spiritblossom_yokai 로드
	PivotMatrix = XMMatrixScaling(0.73f, 0.65f, 0.78f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Spiritblossom_yokai"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/SpiritBlossom/",
			"spiritblossom_yokai.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Model_MapObject_Center()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	// ============================================================  공용 맵 =========================================================================
	lstrcpy(m_szLoading, TEXT("공용 맵을 로드중입니다. "));

	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CenterMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Map_Center/", "Carousel.fbx", PivotMatrix))))
		return E_FAIL;
	// ==============================================================================================================================================

	lstrcpy(m_szLoading, TEXT("공용맵 오브젝트를 로드중입니다. "));

	// Carousel_center_Circle 로드
	PivotMatrix = XMMatrixScaling(0.0000165f, 0.00001f, 0.0000165f) * XMMatrixRotationZ(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CenterMap_Circle"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Map_Objects/Carousel/",
			"Carousel_center_Circle.fbx", PivotMatrix))))
		return E_FAIL;

	// Carousel_center_Elevator 로드
	PivotMatrix = XMMatrixScaling(0.0000158f, 0.000016f, 0.0000158f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CenterMap_Elevator"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Map_Objects/Carousel/",
			"Carousel_center_Elevator.fbx", PivotMatrix))))
		return E_FAIL;

	// Carousel_center_Gears 로드
	PivotMatrix = XMMatrixScaling(0.0000155f, 0.0000001f, 0.0000155f) * XMMatrixRotationX(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CenterMap_Gears"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Map_Objects/Carousel/",
			"Carousel_center_Gears.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Component_Collison()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	lstrcpy(m_szLoading, TEXT("콜라이더 컴포넌트 생성중입니다."));
	/* For.Prototype_Component_Collider_AABB*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pDeviceContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Model_MapObject_Hexacore()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix			PivotMatrix;

	lstrcpy(m_szLoading, TEXT("헥사코어를 로드중입니다."));

	PivotMatrix = XMMatrixScaling(0.00002f, 0.00002f, 0.00002f);
	// Hexacore 로드
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Hexacore"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Map_Objects/Hexcore/",
			"Hexcore.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Model_Item()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	
	_matrix			PivotMatrix;

	lstrcpy(m_szLoading, TEXT("아이템을 로드중입니다."));

	PivotMatrix = XMMatrixScaling(0.0001f, 0.0001f, 0.0001f);

	//for (int i = 0; i < 9; ++i) {
	//_tchar			szPrototypeName[MAX_PATH] = TEXT("");
	//	wsprintf(szPrototypeName, TEXT("Prototype_Component_Model_Item_Material_%d"), i);

	//	char			szTextureFilePath[MAX_PATH];
	//	sprintf_s(szTextureFilePath, MAX_PATH, "../Bin/Resources/Meshes/Item/Material/");

	//	char			szFbxName[MAX_PATH];
	//	sprintf_s(szFbxName, MAX_PATH, "Material_%d.fbx", i);

	//	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, szPrototypeName,
	//		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
	//			szTextureFilePath,
	//			szFbxName, PivotMatrix))))
	//		return E_FAIL;
	//}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_Ending_Texture()
{
	return S_OK;
}

HRESULT CLoader::Loading_Ending_Object()
{
	return S_OK;
}

HRESULT CLoader::Loading_Ending_Shader()
{
	return S_OK;
}

HRESULT CLoader::Loading_Ending_Model()
{
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut, LEVEL eLevel)
{
	CLoader*	pInstance = new CLoader(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct(eLevel)))
	{
		MSG_BOX(TEXT("Failed to Created CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	
	DeleteCriticalSection(&m_CriticalSection);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
