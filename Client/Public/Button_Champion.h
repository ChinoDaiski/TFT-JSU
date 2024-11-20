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
		BUTTON_TYPE		eType;	// ��ư�� ����, � ��ư���� �˼� ����

		ChampionName	eName;	// è�Ǿ��� �̸�(��ư�� ������ �ִ� è�Ǿ� �ʻ�ȭ ����)
		_uint			iLevel;	// è�Ǿ��� ����, ������ ���� �ٸ� �������� ���� �������.
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
	ChampionName	m_eName;	// è�Ǿ��� �̸�(��ư�� ������ �ִ� è�Ǿ� �ʻ�ȭ ����)
	_uint			m_iLevel;	// è�Ǿ��� �̸�(1 ~ 5 ������ ���� ������ ������, �̿� ���� ���� �������� �������� �޶�����.)

public:
	CChampion*	Clone_Champion(void);

	void	Check_EnoughMoney(_uint iMoney);	// ���ڷ� �÷��̾ ������ �ִ� ���� �޾� è�Ǿ��� ��⿡ ����� ������ �˻��ϴ� �Լ�
												// ���� ���� �����ϸ� ����â ��Ȱ��ȭ(ȸ������)
	void	Set_Index(_uint iIndex);			// MarketUI�� ��ȣ ����(1 ~ 5 ������ ���� ������ ������, �̿� ���� �ٸ����� Render�ȴ�.)

	bool	Check_Click(void) { return m_bClick; }

private:
	bool	m_bActivated = false;				// Ȱ��ȭ ����, Ȱ���� �ȴٸ� Ŭ�� �� ��ȣ�ۿ��� �۵��ȴ�.
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