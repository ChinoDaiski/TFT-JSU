#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "UI.h"
#include "Champion.h"
#include "Button_Champion.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

/*
	상점은 1성부터 5성까지 챔피언들의 정보를 들고 있으며, 각 성별로 가지고 있는 최대 챔피언의 갯수가 달라짐.

	1성은 29마리
	2성은 22마리
	3성은 18마리
	4성은 12마리
	5성은 10마리

	상점은 플레이어에게 챔피언을 제공, 플레이어가 리롤을 할때마다 플레이어가 구매 가능한 새로운 챔피언을 보여줌.
	플레이어의 리롤 -> 상점의 리롤, 상점은 새로운 UI를 플레이어에게 보여줌.

	플레이어가 챔피언 구매시, 상점이 들고 있는 챔피언의 갯수가 줄어들음.
*/

class CMarket final : public CGameObject
{
private:
	explicit CMarket(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CMarket(const CMarket& rhs);
	virtual ~CMarket() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	HRESULT SetUp_Components();

public:
	static CMarket* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	void	Reroll(int iPlayerLevel);	// 판매중인 챔피언의 정보를 갱신하는 함수, 인자로 플레이어의 레벨을 받아 해당하는 코스트의 챔피언을 보여준다.
	int		Select_ChampionLevel(int iPlayerLevel);		// 판매할 챔피언의 레벨을 고르는 함수, 반환값으로 선택된 챔피언 레벨을, 인자로 플레이어의 레벨을 받는다.
	ChampionName	Select_Champion_OnSale(int iChampionLevel);	// 판매할 레벨의 챔피언을 고르는 함수, 반환값으로 선택된 챔피언을, 인자로 챔피언의 레벨을 받는다.
	void	Check_ClickButtton(void);	// 마켓이 제공하고 있는 챔피언 버튼의 정보가 존재한다면 눌렸는지 검사하여 추가 행위를 이어간다.

public:
	void ShowChampionButton(void);
	//void ShowOnSale();

private:
	CButton_Champion* m_btChampion[5];	// 챔피언 버튼 5개, 각 버튼은 챔피언의 정보가 담긴 이미지를 특정 위치에 그리고 있다.
	vector<CButton_Champion*> m_vChampUI;	// 챔피언 UI 모음

	map<ChampionName, int>	m_ChampCnt[CChampion::CLEVEL_END];		// 레벨별 챔피언의 갯수 정보, key가 챔피언의 이름, value가 챔피언의 남은 갯수이다.

private:
	void Check_ShowOnSale();	// 판매하고 있는 챔피언의 정보를 보여줄지 말지 확인하는 함수. 플레이어의 레벨이 2레벨 이상이면 보여준다.

private:
	bool	m_bShow = false; // true;		// 처음엔 false, Check_ShowOnSale 함수에서 이를 보여줄지 유무를 확인하지만, 우선 구현을 위해 꺼놓는다.
	
};

END