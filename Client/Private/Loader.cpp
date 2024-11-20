#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"

// ī�޶�
#include "Camera_Default.h"

// �÷��̾�
#include "Player.h"

// �� ����
#include "CenterMap.h"
#include "PersonalMap.h"
#include "MapObject.h"

// UI ����
#include "UI_Logo.h"
#include "UI_Loading.h"
#include "UI_GamePlay.h"
#include "UI_Ending.h"

// ��ư ����
#include "Button_MiniMap.h"
#include "Button_Champion.h"
#include "Button_LevelUp.h"
#include "Button_Reroll.h"
#include "Button_LockUnlock.h"

// ���� ����
#include "Market.h"

// Tile ����
#include "Tile.h"

// Picking ����
#include "PickingObject.h"

// Mouse ����
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
	lstrcpy(m_szLoading, TEXT("�ؽ��ĸ� �ε����Դϴ�. "));
	if (FAILED(Loading_Loading_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���� �ε����Դϴ�. "));
	if (FAILED(Loading_Loading_Model()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���̴��� �ε����Դϴ�. "));
	if (FAILED(Loading_Loading_Shader()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���ӿ�����Ʈ�� �ε����Դϴ�. "));
	if (FAILED(Loading_Loading_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("�ε��� �Ϸ�Ǿ����ϴ�.. "));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	lstrcpy(m_szLoading, TEXT("�ؽ��ĸ� �ε����Դϴ�. "));
	if (FAILED(Loading_GamePlay_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("����Ʈ �ؽ��ĸ� �ε����Դϴ�. "));
	if (FAILED(Loading_GamePlay_Effect_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���� �ε����Դϴ�. "));
	if (FAILED(Loading_GamePlay_Model()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���̴��� �ε����Դϴ�. "));
	if (FAILED(Loading_GamePlay_Shader()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���ӿ�����Ʈ�� �ε����Դϴ�. "));
	if (FAILED(Loading_GamePlay_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("������Ʈ�� �ε����Դϴ�. "));
	if (FAILED(Loading_GamePlay_Component()))
		return E_FAIL;
	
	
	

	lstrcpy(m_szLoading, TEXT("�ε��� �Ϸ�Ǿ����ϴ�.. "));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForEndingLevel()
{
	lstrcpy(m_szLoading, TEXT("�ؽ��ĸ� �ε����Դϴ�. "));
	if (FAILED(Loading_Ending_Texture()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���� �ε����Դϴ�. "));
	if (FAILED(Loading_Ending_Model()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���̴��� �ε����Դϴ�. "));
	if (FAILED(Loading_Ending_Shader()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���ӿ�����Ʈ�� �ε����Դϴ�. "));
	if (FAILED(Loading_Ending_Object()))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("�ε��� �Ϸ�Ǿ����ϴ�.. "));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Loading_Texture()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	// �ε�
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
	// ���� - base
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Market_Base"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Market/tft_UI_Market_base_center%d.dds"), 1))))
		return E_FAIL;
	// ���� - level && exp
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Market_LevelExp"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Market/tft_UI_Market_base_LevelExp%d.dds"), 1))))
		return E_FAIL;
	// ���� - gold
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Market_Gold"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Market/tft_UI_Market_Gold%d.dds"), 1))))
		return E_FAIL;

	// ���� - XP����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Market_LevelUp"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Market/tft_UI_Market_Levelup%d.dds"), 4))))
		return E_FAIL;
	// ���� - ���ΰ�ħ
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Market_Reroll"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Market/tft_UI_Market_Reroll%d.dds"), 4))))
		return E_FAIL;


	// �̴ϸ�
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Minimap"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Minimap/UI_InGame_Minimap%d.dds"), 1))))
		return E_FAIL;
	// �̴ϸ� Ŀ��
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_MinimapCover"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Minimap/UI_InGame_Minimap_cover%d.dds"), 1))))
		return E_FAIL;

	// ��ư
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Button"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/UI_Button%d.dds"), 1))))
		return E_FAIL;

	
	// è�Ǿ� UI
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ChampionBorders"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Champion_Borders/tft_set6_%d.dds"), 60))))
		return E_FAIL;

	// ���� ������ è�Ǿ� �׵θ�
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ChampionOutline"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Borders_level/tft_UI_Champion_Border%d.dds"), 5))))
		return E_FAIL;

	// Ÿ��
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_Map"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Texture/Stage/base_hexagon_03.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Tile_Lounge"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Texture/Stage/base_rect_%d.dds"), 1))))
		return E_FAIL;

	// ���콺
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mouse"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/UI/Mouse/lol_new_Cursor%d.dds"), 1))))
		return E_FAIL;

	// ������
	if (FAILED(Loading_GamePlay_Texture_Item()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Effect_Texture()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	// Ÿ�� ����Ʈ
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Tile"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Textures/Effect/Tile/%d.dds"), 16))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Object()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();

	// �÷��̾� ����
	lstrcpy(m_szLoading, TEXT("Player�� �ε����Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	// �� ����
	lstrcpy(m_szLoading, TEXT("CenterMap�� �ε����Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CenterMap"),
		CCenterMap::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("PersonalMap�� �ε����Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PersonalMap"),
		CPersonalMap::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("MapObject�� �ε����Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MapObject"),
		CMapObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;




	// ī�޶� ����
	lstrcpy(m_szLoading, TEXT("Camera�� �ε����Դϴ�."));

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Default"),
		CCamera_Default::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	// ��ư ����
	lstrcpy(m_szLoading, TEXT("Button�� �ε����Դϴ�."));

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



	// UI����
	lstrcpy(m_szLoading, TEXT("UI_GamePlay�� �ε����Դϴ�."));
	// UI_GamePlay
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_GamePlay"),
		CUI_GamePlay::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("UI_Ending�� �ε����Դϴ�."));
	// UI_Ending
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Ending"),
		CUI_Ending::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	// Market ����
	lstrcpy(m_szLoading, TEXT("Market�� �ε����Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Market"),
		CMarket::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	// Champion ����
	lstrcpy(m_szLoading, TEXT("Champion�� �ε����Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Champion"),
		CChampion::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	


	// Tile ����
	lstrcpy(m_szLoading, TEXT("Tile ������ �ε����Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tile"),
		CTile::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;



	// Picking ����
	lstrcpy(m_szLoading, TEXT("PickingObject ������ �ε����Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Picking"),
		CPickingObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	// Mouse ����
	lstrcpy(m_szLoading, TEXT("Mouse ������ �ε����Դϴ�."));
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
	// 1�� è�Ǿ�
	if (FAILED(Loading_GamePlay_Model_Champiom1Cost()))
		return E_FAIL;

	// 2�� è�Ǿ�
	if (FAILED(Loading_GamePlay_Model_Champiom2Cost()))
		return E_FAIL;

	// 3�� è�Ǿ�
	if (FAILED(Loading_GamePlay_Model_Champiom3Cost()))
		return E_FAIL;

	// 4�� è�Ǿ�
	if (FAILED(Loading_GamePlay_Model_Champiom4Cost()))
		return E_FAIL;

	// 5�� è�Ǿ�
	if (FAILED(Loading_GamePlay_Model_Champiom5Cost()))
		return E_FAIL;

	// �÷��̾� ĳ����
	if (FAILED(Loading_GamePlay_Model_Character()))
		return E_FAIL;

	// ���� �� ���� ������Ʈ
	if (FAILED(Loading_GamePlay_Model_MapObject_Personal()))
		return E_FAIL;

	// ���� �� ���� ������Ʈ
	if (FAILED(Loading_GamePlay_Model_MapObject_Center()))
		return E_FAIL;

	// ����ھ� ������Ʈ
	if (FAILED(Loading_GamePlay_Model_MapObject_Hexacore()))
		return E_FAIL;

	// ������
	if (FAILED(Loading_GamePlay_Model_Item()))
		return E_FAIL;

	

	

	
	//lstrcpy(m_szLoading, TEXT("Chogath�� �ε� ���Դϴ�."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Chogath"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Chogath/",
	//		"Chogath.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Corki�� �ε� ���Դϴ�."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Corki"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Corki/",
	//		"Corki.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Darius�� �ε� ���Դϴ�."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Darius"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Darius/",
	//		"Darius.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Draven�� �ε� ���Դϴ�."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Draven"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Draven/",
	//		"Draven.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Echo�� �ε� ���Դϴ�."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Echo"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Echo/",
	//		"Echo.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Ezreal�� �ε� ���Դϴ�."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Ezreal"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Ezreal/",
	//		"Ezreal.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Galio�� �ε� ���Դϴ�."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Galio"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Galio/",
	//		"Galio.fbx", PivotMatrix))))
	//	return E_FAIL;
	//lstrcpy(m_szLoading, TEXT("Gangplank�� �ε� ���Դϴ�."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Gangplank"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Gangplank/",
	//		"Gangplank.fbx", PivotMatrix))))
	//	return E_FAIL;


	/*lstrcpy(m_szLoading, TEXT("Gnar�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Gnar"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Gnar/",
			"Gnar.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("GnarBig�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_GnarBig"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/GnarBig/",
			"GnarBig.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Illaoi�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Illaoi"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Illaoi/",
			"Illaoi.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Irelia�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Irelia"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Irelia/",
			"Irelia.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Jarvan4�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Jarvan4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Jarvan4/",
			"Jarvan4.fbx", PivotMatrix))))
		return E_FAIL;*/

	//lstrcpy(m_szLoading, TEXT("Jayce_Close�� �ε� ���Դϴ�."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Jayce_Close"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Jayce/",
	//		"Jayce_Close.fbx", PivotMatrix))))
	//	return E_FAIL;

	//lstrcpy(m_szLoading, TEXT("Jayce_Long�� �ε� ���Դϴ�."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Jayce_Long"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Jayce/",
	//		"Jayce_Long.fbx", PivotMatrix))))
	//	return E_FAIL;

	//lstrcpy(m_szLoading, TEXT("Camille�� �ε� ���Դϴ�."));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Camille"),
	//	CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
	//		"../Bin/Resources/Meshes/Champion_fbx/Camille/",
	//		"Camille.fbx", PivotMatrix))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Component()
{
	// �浹ü ������Ʈ �غ�
	if (FAILED(Loading_GamePlay_Component_Collison()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLoader::Loading_GamePlay_Texture_Item()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/*
	// bf ���
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// �ǹٶ��
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ���
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ���ð���
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// �罽����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������ ��
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������ ����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������ ��Ʈ
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// �������� ����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ���� �л���
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ��ȣõ��
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ���μ��� �ݳ��
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������ �ձ�
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������ ���
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ���� ��Ʋ��
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������ �ӻ���
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// �ֶ��� ���Ʈ
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// �𷼷γ����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// �������� ū ������
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ���������� ����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ��ٵ��� ��������
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ��ӿ�����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ���
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// �糭�� �㸮����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ħ���� �帷
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// �ο���� �尩
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������ â
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// �¾�Ҳ� ����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������ ����
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������ �尩
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ������� ��Ʈ
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ��ǳ
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL;

	// ��� ������
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_bf_sword"),
		CTexture::Create(m_pDevice, m_pDeviceContext, TEXT("../Bin/Resources/Meshes/Item/Standard/bf_sword.dds"), 1))))
		return E_FAIL; 
	*/;
	
	lstrcpy(m_szLoading, TEXT("Texture_Item�� �ε� ���Դϴ�."));

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

	// ����, �ٸ��콺, �귣��, �ǻ�, ������, �����, �϶����, �ڸ���4��, ����, ī��, ī���, ����Ʋ��, Ʈ��ġ
	lstrcpy(m_szLoading, TEXT("Nocturne�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Nocturne"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Nocturne/",
			"Nocturne.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Darius�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Darius"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Darius/",
			"Darius.fbx", PivotMatrix)))) 
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Brand�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Brand"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Brand/",
			"Brand.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Poppy�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Poppy"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Poppy/",
			"Poppy.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Singed�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Singed"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Singed/",
			"Singed.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Ezreal�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Ezreal"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Ezreal/",
			"Ezreal.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Illaoi�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Illaoi"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Illaoi/",
			"Illaoi.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Jarvan4�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Jarvan4"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Jarvan4/",
			"Jarvan4.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Ziggs�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Ziggs"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Ziggs/",
			"Ziggs.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Camille�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Camille"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Camille/",
			"Camille.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Kassadin�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Kassadin"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Kassadin/",
			"Kassadin.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Caitlyn�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Caitlyn"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Caitlyn/",
			"Caitlyn.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Twitch�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Twitch"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Twitch/",
			"Twitch.fbx", PivotMatrix))))
		return E_FAIL;












/*
	lstrcpy(m_szLoading, TEXT("Ahri�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Ahri"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Ahri/",
			"Ahri.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Alistar�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Alistar"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Alistar/",
			"Alistar.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Ashe�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Ashe"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Ashe/",
			"Ashe.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Blitzcrank�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Blitzcrank"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Blitzcrank/",
			"Blitzcrank.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Brand�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Brand"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Brand/",
			"Brand.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Braum�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Braum"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Braum/",
			"Braum.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Caitlyn�� �ε� ���Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Champion_Caitlyn"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Champion_fbx/Caitlyn/",
			"Caitlyn.fbx", PivotMatrix))))
		return E_FAIL;
	lstrcpy(m_szLoading, TEXT("Camille�� �ε� ���Դϴ�."));
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

	// ============================================================ �÷��̾� ĳ���� =========================================================================

	lstrcpy(m_szLoading, TEXT("Player_Pet_Sgcat�� �ε����Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Pet_Sgcat"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Character/petSgcat/",
			"peteSgcat.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Player_Pet_Aoshin�� �ε����Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Pet_Aoshin"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Character/petAoshin/",
			"petAoshin.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Player_Pet_Choncc�� �ε����Դϴ�."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player_Pet_Choncc"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Character/petChoncc/",
			"petChoncc.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("Player_Pet_Umbra�� �ε����Դϴ�."));
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

	// ============================================================  ���� �� =========================================================================

	lstrcpy(m_szLoading, TEXT("���� ��(Celebrationtomorrow)�� �ε����Դϴ�."));
	// �� - Celebrationtomorrow �ε�
	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	//PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_CelebrationTomorrow"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/Hexbattle_Celebrationtomorrow_Zaunbotanicalgarden/",
			"set6_5_celebrationtomorrow.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���� ��(Hexbattle)�� �ε����Դϴ�."));
	// �� - Hexbattle �ε�
	PivotMatrix = XMMatrixScaling(0.95f, 1.2f, 1.02f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Hexbattle"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/Hexbattle_Celebrationtomorrow_Zaunbotanicalgarden/",
			"set6_5_hexbattle.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���� ��(ZaunCity)�� �ε����Դϴ�."));
	// �� - ZaunCity �ε�
	PivotMatrix = XMMatrixScaling(1.1f, 1.075f, 1.24f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_ZaunCity"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/ZaunCity/",
			"ZaunCity.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���� ��(Jinx)�� �ε����Դϴ�."));
	// �� - Jinx �ε�
	PivotMatrix = XMMatrixScaling(0.49f, 0.47f, 0.47f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Jinx"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/Jinx_Vi/",
			"set6_jinx.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���� ��(Vi)�� �ε����Դϴ�."));
	// �� - Vi �ε�
	PivotMatrix = XMMatrixScaling(0.324f, 0.328f, 0.328f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Vi"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/Jinx_Vi/",
			"set6_vi.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���� ��(Spiritblossom_kami)�� �ε����Դϴ�."));
	// �� - Spiritblossom_kami �ε�
	PivotMatrix = XMMatrixScaling(0.63f, 0.65f, 0.69f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Spiritblossom_kami"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/SpiritBlossom/",
			"spiritblossom_kami.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���� ��(Spiritblossom_reality)�� �ε����Դϴ�."));
	// �� - Spiritblossom_reality �ε�
	PivotMatrix = XMMatrixScaling(0.732f, 0.65f, 0.8f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_PersonalMap_Spiritblossom_reality"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM,
			"../Bin/Resources/Meshes/Map_Personal/SpiritBlossom/",
			"spiritblossom_reality.fbx", PivotMatrix))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("���� ��(Spiritblossom_yokai)�� �ε����Դϴ�."));
	// �� - Spiritblossom_yokai �ε�
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

	// ============================================================  ���� �� =========================================================================
	lstrcpy(m_szLoading, TEXT("���� ���� �ε����Դϴ�. "));

	PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CenterMap"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Map_Center/", "Carousel.fbx", PivotMatrix))))
		return E_FAIL;
	// ==============================================================================================================================================

	lstrcpy(m_szLoading, TEXT("����� ������Ʈ�� �ε����Դϴ�. "));

	// Carousel_center_Circle �ε�
	PivotMatrix = XMMatrixScaling(0.0000165f, 0.00001f, 0.0000165f) * XMMatrixRotationZ(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CenterMap_Circle"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Map_Objects/Carousel/",
			"Carousel_center_Circle.fbx", PivotMatrix))))
		return E_FAIL;

	// Carousel_center_Elevator �ε�
	PivotMatrix = XMMatrixScaling(0.0000158f, 0.000016f, 0.0000158f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CenterMap_Elevator"),
		CModel::Create(m_pDevice, m_pDeviceContext, CModel::TYPE_ANIM,
			"../Bin/Resources/Meshes/Map_Objects/Carousel/",
			"Carousel_center_Elevator.fbx", PivotMatrix))))
		return E_FAIL;

	// Carousel_center_Gears �ε�
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

	lstrcpy(m_szLoading, TEXT("�ݶ��̴� ������Ʈ �������Դϴ�."));
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

	lstrcpy(m_szLoading, TEXT("����ھ �ε����Դϴ�."));

	PivotMatrix = XMMatrixScaling(0.00002f, 0.00002f, 0.00002f);
	// Hexacore �ε�
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

	lstrcpy(m_szLoading, TEXT("�������� �ε����Դϴ�."));

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
