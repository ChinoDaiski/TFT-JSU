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

	// ������ ���� è�Ǿ��� ���� ����
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



	// Button �ε��� ���� ���� ������ �ҷ��´�.
	// �������� ������ �� �ִ� è�Ǿ� ��ư�� ���� ����
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_BUTTON_Champion);

	if (nullptr == pLayer) {
		MSG_BOX(TEXT("è�Ǿ� ��ư �ε� ����!"));
		return E_FAIL;
	}

	auto& ObjectList = pLayer->Get_ObjectList();
	for (auto& Object : ObjectList) {
		m_vChampUI.push_back(dynamic_cast<CButton_Champion*>(Object));
	}

	// ������ �ѹ� �ؼ� è�Ǿ� ���� â�� ä���.
	// 1�� �������� �����Ѵ�.
	Reroll(1);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CMarket::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// �Ǹ����� è�Ǿ��� �������� ���� ����, �÷��̾��� ������ 2���� �̻��̸� �ڵ����� ��������.
	if (!m_bShow)
		Check_ShowOnSale();

	// �÷��̾��� ������ ���� ������ è�Ǿ� ������ ���� ��ư�� ���� �� �ִ��� Ȯ���Ѵ�.
	// ��ư�� ���� ���θ� Ȯ���Ͽ� ���ȴٸ�, �ش� ��ư�� �����ϰ�, ���� ����, �÷��̾�� è�Ǿ��� �����Ѵ�.
	if (m_bShow) {
		ShowChampionButton();
	}
	
	Check_ClickButtton(); // -> �÷��̾� �߰� ���� Ȱ�� ���
	                                                                                                                                                   
	  


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
		1�� 29 * 13
		2�� 22 * 13
		3�� 18 * 13
		4�� 12 * 11
		5�� 10 * 9
	*/

	/*
		1. �켱 ������ Ȯ���Ͽ� � ������ è�Ǿ��� ���� �����Ѵ�. 
		2. ���õ� �������� ������ è�Ǿ��� �����Ѵ�. ���� �ش� è�Ǿ��� ������ 0����� �ٽ� �����Ѵ�.
		3. ���õ� è�Ǿ��� ������ ���� UIâ�� ����.
	*/

	for (int i = 0; i < 5; ++i) {
		// ������ è�Ǿ��� �������� ���
		if (nullptr != m_btChampion[i]) {
			ChampionName ChampName = m_btChampion[i]->Get_Name();
			_uint iChampLevel = m_btChampion[i]->Get_Level();

			// ������ �����·� �ǵ�����.
			++m_ChampCnt[iChampLevel][ChampName];

			// ������ ������Ʈ�� �����Ѵ�.
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
		01��		100		0		0		0		0
		02��		100		0		0		0		0
		03��		75		25		0		0		0
		04��		55		30		15		0		0
		05��		45		33		20		2		0
		06��		25		40		30		5		0
		07��		19		30		35		15		1
		08��		16		20		35		25		4
		09��		9		15		30		30		16
		10��		5		10		20		40		25
	*/

	int iRandomInt = pGameInstance->Get_RandomInt(0, 100);

	switch (iPlayerLevel)
	{
	case 1:
	case 2:
		//	01��		100		0		0		0		0
		//	02��		100		0		0		0		0
		return 1;
		break;
	case 3:
		//	03��		75		25		0		0		0
		if (iRandomInt < 75)
			return 1;
		else
			return 2;
		break;
	case 4:
		//	04��		55		30		15		0		0
		if (iRandomInt < 55)
			return 1;
		else if (iRandomInt < 85)
			return 2;
		else
			return 3;
		break;
	case 5:
		//	05��		45		33		20		2		0
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
		//	06��		25		40		30		5		0
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
		//	07��		19		30		35		15		1
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
		//	08��		16		20		35		25		4
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
		//	09��		9		15		30		30		16
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
		//	10��		5		10		20		40		25
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
		// �ش� �������� ������ è�Ǿ� ����
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

		// ������ è�Ǿ��� �����ִ� ������ 0�� �ƴ϶��
		auto& iter = m_ChampCnt[(CChampion::CHAMPION_LEVEL)iChampionLevel - 1].find((ChampionName)iRandomChamp);
		if (iter->second != 0) {
			// �⹰�� ������ ���̰�
			--iter->second;
			// è�Ǿ��� ��ȯ�Ѵ�.
			return (ChampionName)iRandomChamp;
		}

		// 0�̶�� �ٽ� ����
	}

}

void CMarket::Check_ClickButtton(void)
{
	// �÷��̾��� ������ ������ �´�.
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);
	if (nullptr == pLayer) {
		MSG_BOX(TEXT("No player decteced!"));
		return;
	}

	// �÷��̾��� ��� ������ ����
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(*(pLayer->Get_ObjectList().begin()));
	_uint iGold = pPlayer->Get_Gold();

	// ������ �����ϰ� �ִ� ��ư�� ���ƴٴϸ� ��ư�� ������ è�Ǿ��� ��⿡ ����� ������ Ȯ���Ѵ�. 
	for (int i = 0; i < 5; ++i) {
		if (nullptr != m_btChampion[i]) {
			// ����� ���� �ƴ϶�� ��ü ��Ȱ��ȭ
			//m_btChampion[i]->Check_EnoughMoney(iGold);

			// ��ư�� ���ȴٸ�
			if (m_btChampion[i]->Check_Click()) {
				// ��ư�� ������ �ִ� ������ �����´�.
				ChampionName eName = m_btChampion[i]->Get_Name();
				_uint iLevel = m_btChampion[i]->Get_Level();

				// ��ư�� ������ �´� è�Ǿ��� �����Ѵ�.
				CChampion* pChamp = m_btChampion[i]->Clone_Champion();

				if (nullptr == pChamp) {
					MSG_BOX(TEXT("Failed to Clone Champion From Champion_Button"));
					continue;
				}

				// ������ è�Ǿ��� �÷��̾ �����Ѵ�.
				if (pPlayer->Purchase(pChamp))
					// ��ư ������ ��Ȱ��ȭ�Ѵ�.
					m_btChampion[i] = nullptr;
				else {
					// ���Ű� �����Ͽ����� ������ è�Ǿ� ������ �����Ѵ�.
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
		// ���� ��ư�� Ȱ��ȭ�� ���¶��
		if (nullptr != m_btChampion[i]) {
			m_btChampion[i]->Set_Index(i);

		}
	}
}

void CMarket::Check_ShowOnSale()
{
	// �÷��̾��� ������ 2�̻��̶��, �� �� �ִ� è�Ǿ��� ������ �����ش�.
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// �÷��̾��� ������ ���´�.
	CLayer* pLayer = pGameInstance->Find_Layer(LEVEL_GAMEPLAY, LAYER_GAMEPLAY_PLAYER);

	
	// ������ ���̾ ������� �ʴٸ�
	if (nullptr != pLayer) {
		// �÷��̾��� ����, ���߿��� ���� ������ ���´�.
		list<class CGameObject*> ObjectLst = pLayer->Get_ObjectList();
		CGameObject* pObject = *ObjectLst.begin();

		_uint iPlayerLevel = dynamic_cast<CPlayer*>(pObject)->Get_Level();

		// ������ 2���� �̻��̶�� è�Ǿ��� �����ش�.
		if (iPlayerLevel >= 2) {
			m_bShow = true;
		}
	}


	RELEASE_INSTANCE(CGameInstance);
}
