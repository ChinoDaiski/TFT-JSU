#pragma once

#include "Client_Defines.h"
#include "Button.h"
#include "Champion.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CButton_Champion final : public CButton {
public:
	typedef struct _tagBtChampionBorderDesc {
		BUTTON_TYPE		eType;	// 버튼의 정보, 어떤 버튼인지 알수 있음

		ChampionName	eName;	// 챔피언의 이름(버튼이 가지고 있는 챔피언 초상화 정보)
		_uint			iLevel;	// 챔피언의 레벨, 레벨에 따라 다른 프레임의 위에 덧씌운다.
	} BtChampBorderDesc;

private:
	explicit CButton_Champion(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CButton_Champion(const CButton_Champion& rhs);
	virtual ~CButton_Champion() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	_float4x4			m_ProjMatrix;
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;

public:
	ChampionName	Get_Name(void) { return m_eName; }
	_uint			Get_Level(void) { return m_iLevel; }

private:
	ChampionName	m_eName;	// 챔피언의 이름(버튼이 가지고 있는 챔피언 초상화 정보)
	_uint			m_iLevel;	// 챔피언의 이름(1 ~ 5 사이의 값을 가지고 있으며, 이에 따라 위에 입혀지는 프레임이 달라진다.)

public:
	CChampion*	Clone_Champion(void);

	void	Check_EnoughMoney(_uint iMoney);	// 인자로 플레이어가 가지고 있는 돈을 받아 챔피언을 사기에 충분한 돈인지 검사하는 함수
												// 만약 돈이 부족하면 구매창 비활성화(회색으로)
	void	Set_Index(_uint iIndex);			// MarketUI의 번호 순서(1 ~ 5 사이의 값을 가지고 있으며, 이에 따라 다른곳에 Render된다.)

	bool	Check_Click(void) { return m_bClick; }

private:
	bool	m_bActivated = false;				// 활성화 여부, 활성이 된다면 클릭 및 상호작용이 작동된다.
	bool	m_bClick = false;


public:
	HRESULT SetUp_Components();
	virtual void Click(void) override;

public:
	static CButton_Champion* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END