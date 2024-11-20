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
	������ 1������ 5������ è�Ǿ���� ������ ��� ������, �� ������ ������ �ִ� �ִ� è�Ǿ��� ������ �޶���.

	1���� 29����
	2���� 22����
	3���� 18����
	4���� 12����
	5���� 10����

	������ �÷��̾�� è�Ǿ��� ����, �÷��̾ ������ �Ҷ����� �÷��̾ ���� ������ ���ο� è�Ǿ��� ������.
	�÷��̾��� ���� -> ������ ����, ������ ���ο� UI�� �÷��̾�� ������.

	�÷��̾ è�Ǿ� ���Ž�, ������ ��� �ִ� è�Ǿ��� ������ �پ����.
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
	void	Reroll(int iPlayerLevel);	// �Ǹ����� è�Ǿ��� ������ �����ϴ� �Լ�, ���ڷ� �÷��̾��� ������ �޾� �ش��ϴ� �ڽ�Ʈ�� è�Ǿ��� �����ش�.
	int		Select_ChampionLevel(int iPlayerLevel);		// �Ǹ��� è�Ǿ��� ������ ���� �Լ�, ��ȯ������ ���õ� è�Ǿ� ������, ���ڷ� �÷��̾��� ������ �޴´�.
	ChampionName	Select_Champion_OnSale(int iChampionLevel);	// �Ǹ��� ������ è�Ǿ��� ���� �Լ�, ��ȯ������ ���õ� è�Ǿ���, ���ڷ� è�Ǿ��� ������ �޴´�.
	void	Check_ClickButtton(void);	// ������ �����ϰ� �ִ� è�Ǿ� ��ư�� ������ �����Ѵٸ� ���ȴ��� �˻��Ͽ� �߰� ������ �̾��.

public:
	void ShowChampionButton(void);
	//void ShowOnSale();

private:
	CButton_Champion* m_btChampion[5];	// è�Ǿ� ��ư 5��, �� ��ư�� è�Ǿ��� ������ ��� �̹����� Ư�� ��ġ�� �׸��� �ִ�.
	vector<CButton_Champion*> m_vChampUI;	// è�Ǿ� UI ����

	map<ChampionName, int>	m_ChampCnt[CChampion::CLEVEL_END];		// ������ è�Ǿ��� ���� ����, key�� è�Ǿ��� �̸�, value�� è�Ǿ��� ���� �����̴�.

private:
	void Check_ShowOnSale();	// �Ǹ��ϰ� �ִ� è�Ǿ��� ������ �������� ���� Ȯ���ϴ� �Լ�. �÷��̾��� ������ 2���� �̻��̸� �����ش�.

private:
	bool	m_bShow = false; // true;		// ó���� false, Check_ShowOnSale �Լ����� �̸� �������� ������ Ȯ��������, �켱 ������ ���� �����´�.
	
};

END