#include "stdafx.h"
#include "..\Public\Market.h"
#include "GameInstance.h"
#include "Player.h"

CMarket::CMarket(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CMarket::CMarket(const CMarket & rhs)
	: CGameObject(rhs)
	, m_ProjMatrix(rhs.m_ProjMatrix)
{
	for (int i = 0; i < 5; ++i) {
		m_btChampion[i] = nullptr;
		m_ChampCnt[i] = rhs.m_ChampCnt[i];
	}
}

HRESULT CMarket::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));

	// 상점에 남은 챔피언의 갯수 설정
	for (int i = Nocturne; i <= Twitch; ++i) {
		m_ChampCnt[CChampion::CLEVEL_1].emplace((ChampionName)i, 29);
	}

	for (int i = Rek_Sai; i <= Talon; ++i) {
		m_ChampCnt[CChampion::CLEVEL_2].emplace((ChampionName)i, 22);
	}

	for (int i = Gangplank; i <= Tryndamere; ++i) {
		m_ChampCnt[CChampion::CLEVEL_3].emplace((ChampionName)i, 18);
	}

	for (int i = Draven; i <= Khazix; ++i) {
		m_ChampCnt[CChampion::CLEVEL_4].emplace((ChampionName)i, 12);
	}

	for (int i = Galio; i <= Tahm_Kench; ++i) {
		m_ChampCnt[CChampion::CLEVEL_5].emplace((ChampionName)i, 10);
	}

	return S_OK;
}

HRESULT CMarket::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	m_fX = g_iWinCX * 0.5f;
	m_fY = g_iWinCY * 0.5f;

	m_fSizeX = g_iWinCX;
	m_fSizeY = g_iWinCY;



	// Button 로딩이 끝난 이후 상점이 불려온다.
	// 상점에서 보여줄 수 있는 챔피언 버튼의 정보 설정
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_BUTTON_Champion);

	if (nullptr == pLayer) {
		MSG_BOX(TEXT("챔피언 버튼 로드 실패!"));
		return E_FAIL;
	}

	auto& ObjectList = pLayer->Get_ObjectList();
	for (auto& Object : ObjectList) {
		m_vChampUI.push_back(dynamic_cast<CButton_Champion*>(Object));
	}

	// 리롤을 한번 해서 챔피언 상점 창을 채운다.
	// 1렙 기준으로 리롤한다.
	Reroll(1);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CMarket::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 판매중인 챔피언을 보여줄지 여부 결정, 플레이어의 레벨이 2레벨 이상이면 자동으로 보여진다.
	if (!m_bShow)
		Check_ShowOnSale();

	// 플레이어의 레벨에 따라 제공한 챔피언 정보를 가진 버튼을 누를 수 있는지 확인한다.
	// 버튼의 눌린 여부를 확인하여 눌렸다면, 해당 버튼을 삭제하고, 돈을 뺴고, 플레이어에게 챔피언을 제공한다.
	if (m_bShow) {
		ShowChampionButton();
	}
	
	Check_ClickButtton(); // -> 플레이어 추가 이후 활성 요망
	                                                                                                                                                   
	  


	for (int i = 0; i < 5; ++i) {
		if (nullptr != m_btChampion[i])
			m_btChampion[i]->Tick(TimeDelta);
	}
}

void CMarket::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	for (int i = 0; i < 5; ++i) {
		if (nullptr != m_btChampion[i])
			m_btChampion[i]->LateTick(TimeDelta);
	}
}

HRESULT CMarket::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	
	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	_float4x4	ViewMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	_float4x4	ProjMatrixTP;
	XMStoreFloat4x4(&ProjMatrixTP, XMMatrixTranspose(XMLoadFloat4x4(&m_ProjMatrix)));
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &ProjMatrixTP, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;
	
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMarket::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Button"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CMarket * CMarket::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CMarket*	pInstance = new CMarket(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CMarket"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMarket::Clone(void * pArg)
{
	CMarket*	pInstance = new CMarket(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CMarket"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMarket::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}

void CMarket::Reroll(int iPlayerLevel)
{
	/*
		1렙 29 * 13
		2렙 22 * 13
		3렙 18 * 13
		4렙 12 * 11
		5렙 10 * 9
	*/

	/*
		1. 우선 레벨을 확인하여 어떤 레벨의 챔피언을 고를지 선택한다. 
		2. 선택된 레벨에서 랜덤한 챔피언을 선택한다. 만약 해당 챔피언의 갯수가 0개라면 다시 선택한다.
		3. 선택된 챔피언의 정보를 상점 UI창에 띄운다.
	*/

	for (int i = 0; i < 5; ++i) {
		// 기존에 챔피언이 남아있을 경우
		if (nullptr != m_btChampion[i]) {
			ChampionName ChampName = m_btChampion[i]->Get_Name();
			_uint iChampLevel = m_btChampion[i]->Get_Level();

			// 갯수를 원상태로 되돌린다.
			++m_ChampCnt[iChampLevel][ChampName];

			// 복제된 오브젝트를 삭제한다.
			Safe_Release(m_btChampion[i]);
		}
	}

	for (int i = 0; i < 5; ++i) {
		int iSelect_ChampLevel = Select_ChampionLevel(iPlayerLevel);
		ChampionName ChampName = Select_Champion_OnSale(iSelect_ChampLevel);

		CButton_Champion::BtChampBorderDesc ChampBorderDesc;
		ChampBorderDesc.eName = ChampName;
		ChampBorderDesc.eType = CButton::BT_CHAMPION;
		ChampBorderDesc.iLevel = iSelect_ChampLevel;
		m_btChampion[i] = dynamic_cast<CButton_Champion*>(m_vChampUI.at((int)ChampName)->Clone(&ChampBorderDesc));
	}

}

int CMarket::Select_ChampionLevel(int iPlayerLevel)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	/*
				1(29)	2(22)	3(18)	4(12)	5(10)
		01렙		100		0		0		0		0
		02렙		100		0		0		0		0
		03렙		75		25		0		0		0
		04렙		55		30		15		0		0
		05렙		45		33		20		2		0
		06렙		25		40		30		5		0
		07렙		19		30		35		15		1
		08렙		16		20		35		25		4
		09렙		9		15		30		30		16
		10렙		5		10		20		40		25
	*/

	int iRandomInt = pGameInstance->Get_RandomInt(0, 100);

	switch (iPlayerLevel)
	{
	case 1:
	case 2:
		//	01렙		100		0		0		0		0
		//	02렙		100		0		0		0		0
		return 1;
		break;
	case 3:
		//	03렙		75		25		0		0		0
		if (iRandomInt < 75)
			return 1;
		else
			return 2;
		break;
	case 4:
		//	04렙		55		30		15		0		0
		if (iRandomInt < 55)
			return 1;
		else if (iRandomInt < 85)
			return 2;
		else
			return 3;
		break;
	case 5:
		//	05렙		45		33		20		2		0
		if (iRandomInt < 45)
			return 1;
		else if (iRandomInt < 78)
			return 2;
		else if (iRandomInt < 98)
			return 3;
		else
			return 4;
		break;
	case 6:
		//	06렙		25		40		30		5		0
		if (iRandomInt < 25)
			return 1;
		else if (iRandomInt < 65)
			return 2;
		else if (iRandomInt < 95)
			return 3;
		else
			return 4;
		break;
	case 7:
		//	07렙		19		30		35		15		1
		if (iRandomInt < 19)
			return 1;
		else if (iRandomInt < 49)
			return 2;
		else if (iRandomInt < 84)
			return 3;
		else if (iRandomInt < 99)
			return 4;
		else
			return 5;
		break;
	case 8:
		//	08렙		16		20		35		25		4
		if (iRandomInt < 16)
			return 1;
		else if (iRandomInt < 36)
			return 2;
		else if (iRandomInt < 71)
			return 3;
		else if (iRandomInt < 96)
			return 4;
		else
			return 5;
		break;
	case 9:
		//	09렙		9		15		30		30		16
		if (iRandomInt < 9)
			return 1;
		else if (iRandomInt < 24)
			return 2;
		else if (iRandomInt < 54)
			return 3;
		else if (iRandomInt < 84)
			return 4;
		else
			return 5;
		break;
	case 10:
		//	10렙		5		10		20		40		25
		if (iRandomInt < 5)
			return 1;
		else if (iRandomInt < 15)
			return 2;
		else if (iRandomInt < 35)
			return 3;
		else if (iRandomInt < 75)
			return 4;
		else
			return 5;
		break;
	}

	RELEASE_INSTANCE(CGameInstance);

	return -1;
}

ChampionName CMarket::Select_Champion_OnSale(int iChampionLevel)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	int iRandomChamp = 0;

	while (true) {
		// 해당 레벨에서 랜덤한 챔피언 선택
		switch (iChampionLevel)
		{
		case 1:
			iRandomChamp = pGameInstance->Get_RandomInt((int)Nocturne, (int)Twitch);
			break;
		case 2:
			iRandomChamp = pGameInstance->Get_RandomInt((int)Rek_Sai, (int)Talon);
			break;
		case 3:
			iRandomChamp = pGameInstance->Get_RandomInt((int)Gangplank, (int)Tryndamere);
			break;
		case 4:
			iRandomChamp = pGameInstance->Get_RandomInt((int)Draven, (int)Khazix);
			break;
		case 5:
			iRandomChamp = pGameInstance->Get_RandomInt((int)Galio, (int)Tahm_Kench);
			break;
		default:
			break;
		}

		// 선택한 챔피언의 남아있는 갯수가 0이 아니라면
		auto& iter = m_ChampCnt[(CChampion::CHAMPION_LEVEL)iChampionLevel - 1].find((ChampionName)iRandomChamp);
		if (iter->second != 0) {
			// 기물의 갯수를 줄이고
			--iter->second;
			// 챔피언을 반환한다.
			return (ChampionName)iRandomChamp;
		}

		// 0이라면 다시 실행
	}

}

void CMarket::Check_ClickButtton(void)
{
	// 플레이어의 정보를 가지고 온다.
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);
	if (nullptr == pLayer) {
		MSG_BOX(TEXT("No player decteced!"));
		return;
	}

	// 플레이어의 골드 정보를 들고와
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(*(pLayer->Get_ObjectList().begin()));
	_uint iGold = pPlayer->Get_Gold();

	// 상점이 제공하고 있는 버튼을 돌아다니며 버튼과 연관된 챔피언을 사기에 충분한 돈인지 확인한다. 
	for (int i = 0; i < 5; ++i) {
		if (nullptr != m_btChampion[i]) {
			// 충분한 돈이 아니라면 자체 비활성화
			//m_btChampion[i]->Check_EnoughMoney(iGold);

			// 버튼이 눌렸다면
			if (m_btChampion[i]->Check_Click()) {
				// 버튼이 가지고 있는 정보를 가져온다.
				ChampionName eName = m_btChampion[i]->Get_Name();
				_uint iLevel = m_btChampion[i]->Get_Level();

				// 버튼의 정보에 맞는 챔피언을 생성한다.
				CChampion* pChamp = m_btChampion[i]->Clone_Champion();

				if (nullptr == pChamp) {
					MSG_BOX(TEXT("Failed to Clone Champion From Champion_Button"));
					continue;
				}

				// 생성된 챔피언을 플레이어가 구매한다.
				if (pPlayer->Purchase(pChamp))
					// 버튼 정보를 비활성화한다.
					m_btChampion[i] = nullptr;
				else {
					// 구매가 실패하였으니 복제한 챔피언 정보를 삭제한다.
					Safe_Release(pChamp);
				}
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CMarket::ShowChampionButton(void)
{
	for (int i = 0; i < 5; ++i) {
		// 만약 버튼이 활성화된 상태라면
		if (nullptr != m_btChampion[i]) {
			m_btChampion[i]->Set_Index(i);

		}
	}
}

void CMarket::Check_ShowOnSale()
{
	// 플레이어의 레벨이 2이상이라면, 살 수 있는 챔피언의 정보를 보여준다.
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 플레이어의 정보를 들고온다.
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);

	
	// 가져온 레이어가 비어있지 않다면
	if (nullptr != pLayer) {
		// 플레이어의 정보, 그중에서 레벨 정보를 들고온다.
		list<class CGameObject*> ObjectLst = pLayer->Get_ObjectList();
		CGameObject* pObject = *ObjectLst.begin();

		_uint iPlayerLevel = dynamic_cast<CPlayer*>(pObject)->Get_Level();

		// 레벨이 2레벨 이상이라면 챔피언을 보여준다.
		if (iPlayerLevel >= 2) {
			m_bShow = true;
		}
	}


	RELEASE_INSTANCE(CGameInstance);
}
